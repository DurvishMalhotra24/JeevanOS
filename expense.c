#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expense.h"
#include "utils.h"

/*
 * expense.c - Expense tracking module.
 * Expenses are kept in an array of structures and saved to expenses.txt.
 */

#define EXPENSE_FILE "expenses.txt"

static const char *category_names[NUM_CATEGORIES] = {
    "Food", "Travel", "Study", "Entertainment", "Health", "Other"
};

static Expense expenses[MAX_EXPENSES];
static int     expense_count = 0;
static int     expense_next_id       = 1;

/* ---------- helpers ---------- */

static int find_expense_index(int id)
{
    int i;

    for (i = 0; i < expense_count; i++) {
        if (expenses[i].id == id) {
            return i;
        }
    }
    return -1;
}

static void print_expense_table(const Expense *list, int n)
{
    int i;

    print_line('-', 74);
    printf("%-4s %-11s %-14s %-28s %10s\n", "ID", "Date", "Category", "Description", "Amount");
    print_line('-', 74);
    for (i = 0; i < n; i++) {
        printf("%-4d %-11s %-14s %-28.28s %10.2f\n",
               list[i].id, list[i].date, category_names[list[i].category],
               list[i].description, list[i].amount);
    }
    print_line('-', 74);
}

/* ---------- file handling ---------- */

static void expense_save(void)
{
    FILE *fp;
    int i;

    fp = fopen(EXPENSE_FILE, "w");
    if (fp == NULL) {
        printf("  Warning: could not save expenses to %s.\n", EXPENSE_FILE);
        return;
    }

    for (i = 0; i < expense_count; i++) {
        fprintf(fp, "%d|%d|%s|%.2f|%s\n",
                expenses[i].id, expenses[i].category, expenses[i].description,
                expenses[i].amount, expenses[i].date);
    }
    fclose(fp);
}

void expense_load(void)
{
    FILE *fp;
    char line[160];
    Expense e;

    expense_count = 0;
    expense_next_id = 1;

    fp = fopen(EXPENSE_FILE, "r");
    if (fp == NULL) {
        return;   /* first run: no saved data yet */
    }

    while (expense_count < MAX_EXPENSES && fgets(line, (int)sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d|%d|%49[^|]|%f|%10[^|]",
                   &e.id, &e.category, e.description, &e.amount, e.date) == 5
            && e.category >= 0 && e.category < NUM_CATEGORIES) {
            expenses[expense_count++] = e;
            if (e.id >= expense_next_id) {
                expense_next_id = e.id + 1;
            }
        }
    }
    fclose(fp);
}

/* ---------- features ---------- */

static void add_expense(void)
{
    Expense e;
    int i;

    if (expense_count >= MAX_EXPENSES) {
        printf("  Expense list is full (%d entries).\n", MAX_EXPENSES);
        return;
    }

    e.id = expense_next_id;
    printf("  Categories:\n");
    for (i = 0; i < NUM_CATEGORIES; i++) {
        printf("    %d. %s\n", i + 1, category_names[i]);
    }
    e.category = read_int("  Choose category: ", 1, NUM_CATEGORIES) - 1;
    read_text("  Description          : ", e.description, EXPENSE_DESC_LEN);
    e.amount = read_float("  Amount (Rs.)         : ", 0.01f, 10000000.0f);
    read_date("  Date (DD-MM-YYYY)    : ", e.date);

    expenses[expense_count++] = e;
    expense_next_id++;
    expense_save();
    printf("  Expense added with ID %d.\n", e.id);
}

static void view_expenses(void)
{
    float total = 0.0f;
    int i;

    if (expense_count == 0) {
        printf("  No expenses recorded yet.\n");
        return;
    }

    print_expense_table(expenses, expense_count);
    for (i = 0; i < expense_count; i++) {
        total += expenses[i].amount;
    }
    printf("  Total spent: Rs. %.2f\n", total);
}

static void category_summary(void)
{
    float totals[NUM_CATEGORIES] = {0};
    float grand_total = 0.0f;
    int i;

    if (expense_count == 0) {
        printf("  No expenses recorded yet.\n");
        return;
    }

    for (i = 0; i < expense_count; i++) {
        totals[expenses[i].category] += expenses[i].amount;
        grand_total += expenses[i].amount;
    }

    print_line('-', 44);
    printf("%-16s %12s %10s\n", "Category", "Total (Rs.)", "Share");
    print_line('-', 44);
    for (i = 0; i < NUM_CATEGORIES; i++) {
        printf("%-16s %12.2f %9.1f%%\n",
               category_names[i], totals[i], totals[i] * 100.0f / grand_total);
    }
    print_line('-', 44);
    printf("%-16s %12.2f\n", "Grand total", grand_total);
}

static void monthly_report(void)
{
    Expense found[MAX_EXPENSES];
    int count = 0;
    int month, year, d, m, y, i;
    float total = 0.0f;

    month = read_int("  Month (1-12): ", 1, 12);
    year  = read_int("  Year (YYYY) : ", 2000, 2100);

    for (i = 0; i < expense_count; i++) {
        if (sscanf(expenses[i].date, "%d-%d-%d", &d, &m, &y) == 3 &&
            m == month && y == year) {
            found[count++] = expenses[i];
            total += expenses[i].amount;
        }
    }

    printf("\n");
    if (count == 0) {
        printf("  No expenses found for %02d-%04d.\n", month, year);
    } else {
        print_expense_table(found, count);
        printf("  Total for %02d-%04d: Rs. %.2f\n", month, year, total);
    }
}

static void delete_expense(void)
{
    int id, index, i;

    if (expense_count == 0) {
        printf("  No expenses to delete.\n");
        return;
    }

    id = read_int("  Enter expense ID to delete: ", 1, 1000000);
    index = find_expense_index(id);
    if (index < 0) {
        printf("  No expense found with ID %d.\n", id);
        return;
    }

    if (read_int("  Are you sure? (1 = Yes, 0 = No): ", 0, 1) == 0) {
        printf("  Delete cancelled.\n");
        return;
    }

    /* Shift the remaining entries one position to the left. */
    for (i = index; i < expense_count - 1; i++) {
        expenses[i] = expenses[i + 1];
    }
    expense_count--;
    expense_save();
    printf("  Expense %d deleted.\n", id);
}

void expense_get_stats(int *count, float *total)
{
    int i;

    *count = expense_count;
    *total = 0.0f;
    for (i = 0; i < expense_count; i++) {
        *total += expenses[i].amount;
    }
}

void expense_menu(void)
{
    int choice;

    do {
        printf("\n");
        print_line('=', 40);
        printf("          EXPENSE TRACKER\n");
        print_line('=', 40);
        printf("  1. Add an expense\n");
        printf("  2. View all expenses\n");
        printf("  3. Category-wise summary\n");
        printf("  4. Monthly report\n");
        printf("  5. Delete an expense\n");
        printf("  0. Back to main menu\n");
        print_line('-', 40);
        choice = read_int("  Enter your choice: ", 0, 5);
        printf("\n");

        switch (choice) {
        case 1: add_expense();      break;
        case 2: view_expenses();    break;
        case 3: category_summary(); break;
        case 4: monthly_report();   break;
        case 5: delete_expense();   break;
        default:                    break;
        }

        if (choice != 0) {
            pause_screen();
        }
    } while (choice != 0);
}
