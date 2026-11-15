#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "task.h"
#include "utils.h"

/*
 * task.c - Task management module.
 * Tasks are kept in an array of structures and saved to tasks.txt.
 */

#define TASK_FILE "tasks.txt"

static Task tasks[MAX_TASKS];
static int  task_count = 0;
static int  task_next_id    = 1;

/* ---------- helpers ---------- */

static const char *priority_name(int priority)
{
    switch (priority) {
    case 1:  return "High";
    case 2:  return "Medium";
    default: return "Low";
    }
}

/* Returns the array index of the task with this id, or -1 if not found. */
static int find_task_index(int id)
{
    int i;

    for (i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            return i;
        }
    }
    return -1;
}

static void print_task_table(const Task *list, int n)
{
    int i;

    print_line('-', 68);
    printf("%-4s %-30s %-11s %-8s %-9s\n", "ID", "Title", "Due Date", "Priority", "Status");
    print_line('-', 68);
    for (i = 0; i < n; i++) {
        printf("%-4d %-30.30s %-11s %-8s %-9s\n",
               list[i].id, list[i].title, list[i].due_date,
               priority_name(list[i].priority),
               list[i].completed ? "Done" : "Pending");
    }
    print_line('-', 68);
}

/* ---------- file handling ---------- */

static void task_save(void)
{
    FILE *fp;
    int i;

    fp = fopen(TASK_FILE, "w");
    if (fp == NULL) {
        printf("  Warning: could not save tasks to %s.\n", TASK_FILE);
        return;
    }

    for (i = 0; i < task_count; i++) {
        fprintf(fp, "%d|%s|%s|%d|%d\n",
                tasks[i].id, tasks[i].title, tasks[i].due_date,
                tasks[i].priority, tasks[i].completed);
    }
    fclose(fp);
}

void task_load(void)
{
    FILE *fp;
    char line[160];
    Task t;

    task_count = 0;
    task_next_id = 1;

    fp = fopen(TASK_FILE, "r");
    if (fp == NULL) {
        return;   /* first run: no saved data yet */
    }

    while (task_count < MAX_TASKS && fgets(line, (int)sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d|%59[^|]|%10[^|]|%d|%d",
                   &t.id, t.title, t.due_date, &t.priority, &t.completed) == 5) {
            tasks[task_count++] = t;
            if (t.id >= task_next_id) {
                task_next_id = t.id + 1;
            }
        }
    }
    fclose(fp);
}

/* ---------- features ---------- */

static void add_task(void)
{
    Task t;

    if (task_count >= MAX_TASKS) {
        printf("  Task list is full (%d tasks).\n", MAX_TASKS);
        return;
    }

    t.id = task_next_id;
    read_text("  Title            : ", t.title, TASK_TITLE_LEN);
    read_date("  Due date (DD-MM-YYYY): ", t.due_date);
    t.priority = read_int("  Priority (1=High, 2=Medium, 3=Low): ", 1, 3);
    t.completed = 0;

    tasks[task_count++] = t;
    task_next_id++;
    task_save();
    printf("  Task added with ID %d.\n", t.id);
}

/* Shows all tasks. If by_priority is 1, pending tasks come first, High to Low. */
static void view_tasks(int by_priority)
{
    Task sorted[MAX_TASKS];
    Task tmp;
    int i, j;

    if (task_count == 0) {
        printf("  No tasks yet. Add one from the menu.\n");
        return;
    }

    for (i = 0; i < task_count; i++) {
        sorted[i] = tasks[i];
    }

    if (by_priority) {
        /* Bubble sort: pending before done, then by priority, then by id. */
        for (i = 0; i < task_count - 1; i++) {
            for (j = 0; j < task_count - 1 - i; j++) {
                int swap = 0;
                if (sorted[j].completed > sorted[j + 1].completed) {
                    swap = 1;
                } else if (sorted[j].completed == sorted[j + 1].completed) {
                    if (sorted[j].priority > sorted[j + 1].priority) {
                        swap = 1;
                    } else if (sorted[j].priority == sorted[j + 1].priority &&
                               sorted[j].id > sorted[j + 1].id) {
                        swap = 1;
                    }
                }
                if (swap) {
                    tmp = sorted[j];
                    sorted[j] = sorted[j + 1];
                    sorted[j + 1] = tmp;
                }
            }
        }
    }

    print_task_table(sorted, task_count);
}

static void mark_complete(void)
{
    int id, index;

    if (task_count == 0) {
        printf("  No tasks to update.\n");
        return;
    }

    id = read_int("  Enter task ID to mark complete: ", 1, 1000000);
    index = find_task_index(id);
    if (index < 0) {
        printf("  No task found with ID %d.\n", id);
    } else if (tasks[index].completed) {
        printf("  Task %d is already completed.\n", id);
    } else {
        tasks[index].completed = 1;
        task_save();
        printf("  Task %d marked as completed.\n", id);
    }
}

static void search_tasks(void)
{
    char keyword[TASK_TITLE_LEN];
    Task found[MAX_TASKS];
    int count = 0;
    int i;

    read_text("  Search keyword: ", keyword, TASK_TITLE_LEN);

    for (i = 0; i < task_count; i++) {
        if (contains_nocase(tasks[i].title, keyword)) {
            found[count++] = tasks[i];
        }
    }

    if (count == 0) {
        printf("  No tasks match \"%s\".\n", keyword);
    } else {
        print_task_table(found, count);
    }
}

static void delete_task(void)
{
    int id, index, i;

    if (task_count == 0) {
        printf("  No tasks to delete.\n");
        return;
    }

    id = read_int("  Enter task ID to delete: ", 1, 1000000);
    index = find_task_index(id);
    if (index < 0) {
        printf("  No task found with ID %d.\n", id);
        return;
    }

    if (read_int("  Are you sure? (1 = Yes, 0 = No): ", 0, 1) == 0) {
        printf("  Delete cancelled.\n");
        return;
    }

    /* Shift the remaining tasks one position to the left. */
    for (i = index; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    task_count--;
    task_save();
    printf("  Task %d deleted.\n", id);
}

void task_get_stats(int *total, int *completed)
{
    int i;

    *total = task_count;
    *completed = 0;
    for (i = 0; i < task_count; i++) {
        if (tasks[i].completed) {
            (*completed)++;
        }
    }
}

void task_menu(void)
{
    int choice;

    do {
        printf("\n");
        print_line('=', 40);
        printf("          TASK MANAGEMENT\n");
        print_line('=', 40);
        printf("  1. Add a task\n");
        printf("  2. View all tasks\n");
        printf("  3. View tasks by priority\n");
        printf("  4. Mark a task as complete\n");
        printf("  5. Search tasks\n");
        printf("  6. Delete a task\n");
        printf("  0. Back to main menu\n");
        print_line('-', 40);
        choice = read_int("  Enter your choice: ", 0, 6);
        printf("\n");

        switch (choice) {
        case 1: add_task();        break;
        case 2: view_tasks(0);     break;
        case 3: view_tasks(1);     break;
        case 4: mark_complete();   break;
        case 5: search_tasks();    break;
        case 6: delete_task();     break;
        default:                   break;
        }

        if (choice != 0) {
            pause_screen();
        }
    } while (choice != 0);
}
