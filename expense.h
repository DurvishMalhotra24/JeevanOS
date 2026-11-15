#ifndef EXPENSE_H
#define EXPENSE_H

/*
 * expense.h - Expense tracking module.
 */

#define MAX_EXPENSES    500
#define EXPENSE_DESC_LEN 50
#define NUM_CATEGORIES  6

/* One expense entry. category is an index into the category list. */
typedef struct {
    int   id;
    int   category;
    char  description[EXPENSE_DESC_LEN];
    float amount;
    char  date[11];          /* DD-MM-YYYY */
} Expense;

/* Loads expenses from expenses.txt (does nothing if the file does not exist yet). */
void expense_load(void);

/* Shows the expense menu and handles the user's choices. */
void expense_menu(void);

/* Gives the number of expense entries and the total amount spent. */
void expense_get_stats(int *count, float *total);

#endif
