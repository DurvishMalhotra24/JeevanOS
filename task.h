#ifndef TASK_H
#define TASK_H

/*
 * task.h - Task management module.
 */

#define MAX_TASKS      100
#define TASK_TITLE_LEN 60
#define DATE_LEN       11   /* DD-MM-YYYY plus the end-of-string character */

/* One task entry. Priority: 1 = High, 2 = Medium, 3 = Low. */
typedef struct {
    int  id;
    char title[TASK_TITLE_LEN];
    char due_date[DATE_LEN];
    int  priority;
    int  completed;          /* 0 = pending, 1 = done */
} Task;

/* Loads tasks from tasks.txt (does nothing if the file does not exist yet). */
void task_load(void);

/* Shows the task menu and handles the user's choices. */
void task_menu(void);

/* Gives the total number of tasks and how many are completed. */
void task_get_stats(int *total, int *completed);

#endif
