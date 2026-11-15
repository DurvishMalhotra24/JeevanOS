/*
 * JeevanOs - Personal Management System
 * PPS Semester 1 Capstone Project
 *
 * Student : Durvish Malhotra
 * Roll No : 29082
 * Branch  : CSE        Section : 2        Semester : 1
 *
 * A menu-driven application written in C that helps organise daily tasks
 * and track personal expenses. Data is saved in text files (tasks.txt and
 * expenses.txt) so it is still there the next time the program runs.
 *
 * Project files:
 *   main.c      - dashboard and main menu (compile this file only)
 *   utils.c/h   - safe input and output helper functions
 *   student.c/h - student details page
 *   task.c/h    - task management module
 *   expense.c/h - expense tracking module
 *
 * Build and run:   gcc main.c -o main
 *
 * The module files below are pulled in with #include so that this one
 * command (and the VS Code Run button) builds the whole project.
 * Do not compile the other .c files separately.
 */
#include <stdio.h>

#include "utils.h"
#include "student.h"
#include "task.h"
#include "expense.h"

#include "utils.c"
#include "student.c"
#include "task.c"
#include "expense.c"

static void show_dashboard(void)
{
    int total_tasks, done_tasks, pending_tasks;
    int expense_count;
    float expense_total;
    float percent = 0.0f;

    task_get_stats(&total_tasks, &done_tasks);
    expense_get_stats(&expense_count, &expense_total);
    pending_tasks = total_tasks - done_tasks;
    if (total_tasks > 0) {
        percent = done_tasks * 100.0f / total_tasks;
    }

    print_line('=', 44);
    printf("          DASHBOARD SUMMARY\n");
    print_line('=', 44);
    printf("  Tasks\n");
    printf("    Total            : %d\n", total_tasks);
    printf("    Completed        : %d\n", done_tasks);
    printf("    Pending          : %d\n", pending_tasks);
    printf("    Productivity     : %.1f%%\n", percent);
    print_line('-', 44);
    printf("  Expenses\n");
    printf("    Entries          : %d\n", expense_count);
    printf("    Total spent      : Rs. %.2f\n", expense_total);
    print_line('=', 44);
}

int main(void)
{
    int choice;

    task_load();
    expense_load();

    show_student_page();
    pause_screen();

    do {
        printf("\n");
        print_line('=', 44);
        printf("      %s\n", "JeevanOs - PERSONAL MANAGEMENT SYSTEM");
        print_line('=', 44);
        printf("  1. Task Management\n");
        printf("  2. Expense Tracker\n");
        printf("  3. Dashboard Summary\n");
        printf("  4. Student Details\n");
        printf("  0. Exit\n");
        print_line('-', 44);
        choice = read_int("  Enter your choice: ", 0, 4);

        switch (choice) {
        case 1:
            task_menu();
            break;
        case 2:
            expense_menu();
            break;
        case 3:
            printf("\n");
            show_dashboard();
            pause_screen();
            break;
        case 4:
            show_student_page();
            pause_screen();
            break;
        default:
            break;
        }
    } while (choice != 0);

    printf("\nThank you for using JeevanOs. Goodbye!\n");
    return 0;
}
