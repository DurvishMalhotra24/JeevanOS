# JeevanOs

**PPS Semester 1 Capstone Project** | written entirely in the C programming language

## Student Details

| | |
|---|---|
| **Name** | Durvish Malhotra |
| **Roll No** | 29082 |
| **Branch** | CSE |
| **Section** | 2 |
| **Semester** | 1 |
| **Course** | Programming for Problem Solving (PPS) |
| **Project** | JeevanOs - Personal Management System (Capstone Project) |

## About the Project

JeevanOs is a menu-driven application developed using C programming. It helps users organise daily tasks, manage personal expenses and monitor productivity through a single application.

The first version implements **task management** and **expense tracking** using functions, structures, arrays and file handling. Its modular design allows future expansion into database integration, web development, data analytics and AI-powered recommendations.

The project solves a practical problem while demonstrating the fundamental programming concepts of the first semester, and gives a foundation for a scalable software application.

## Features

**Task Management**
- Add tasks with a title, due date and priority (High / Medium / Low)
- View all tasks, or view them sorted by priority (pending tasks first)
- Mark tasks as complete
- Search tasks by keyword (not case sensitive)
- Delete tasks (with confirmation)

**Expense Tracker**
- Add expenses with a category, description, amount and date
- View all expenses with the running total
- Category-wise summary with percentage share
- Monthly report
- Delete expenses (with confirmation)

**Dashboard**
- Total, completed and pending tasks, and a productivity percentage
- Number of expense entries and total amount spent

**General**
- Input validation everywhere: wrong numbers, invalid dates and empty text are rejected and asked again
- Data is saved automatically to text files and loaded when the program starts

## Project Structure

```
JeevanOs/
├── main.c        Main program: dashboard and main menu
├── student.c/h   Student details page
├── task.c/h      Task management module
├── expense.c/h   Expense tracking module
├── utils.c/h     Safe input and output helper functions
└── README.md
```

Each module has a header file (`.h`) with its constants, structures and function declarations, and a source file (`.c`) with the code. `main.c` includes the other `.c` files, so the whole project builds from `main.c` alone.

Saved data is stored in `tasks.txt` and `expenses.txt`, created automatically in the folder where the program is run.

## How to Build and Run

You need a C compiler such as `gcc`.

```bash
gcc main.c -o main
./main          # on Windows PowerShell: .\main
```

### Running in VS Code

1. Install a C compiler (MinGW-w64 / MSYS2 on Windows, `gcc` on Linux, Xcode tools on macOS) and the **C/C++** extension by Microsoft.
2. Open the project folder with **File > Open Folder**.
3. Open `main.c` and click the **Run** button (▶) at the top right, or type `gcc main.c -o main` and then `.\main` in the VS Code terminal.
4. Always compile `main.c` only. Do not compile the other `.c` files separately, because `main.c` already includes them.

## C Concepts Used (as per the syllabus)

| Syllabus topic | Where it is used |
|---|---|
| Data types, variables, type conversion | Throughout; `int`, `float`, `char`, casts in `utils.c` |
| Header files, preprocessors | `#include`, `#define`, include guards in the `.h` files |
| Basic input and output | `printf`, `fgets`, `sscanf` in `utils.c` |
| Conditional and branching statements | `if`, `else`, `switch` for menus and validation |
| Loops | `do-while` menus, `for` loops over arrays, input retry loops |
| Arrays (1-D) and character arrays | Task and expense arrays, strings for titles and dates |
| Functions, call by value, call by reference | Small functions in every module; pointers used in `task_get_stats` and `expense_get_stats` |
| Passing arrays to functions | `print_task_table`, `print_expense_table` |
| Storage classes | `static` variables and functions keep each module's data private |
| Structures and array of structures | `Task` and `Expense` structures stored in arrays |
| Pointers | Output parameters, string handling |
| File handling | `fopen`, `fgets`, `fprintf`, `fclose` in the save and load functions |

## Sample Run

```
============================================
      JeevanOs - PERSONAL MANAGEMENT SYSTEM
============================================
  1. Task Management
  2. Expense Tracker
  3. Dashboard Summary
  4. Student Details
  0. Exit
--------------------------------------------
  Enter your choice:
```

```
--------------------------------------------------------------------
ID   Title                          Due Date    Priority Status
--------------------------------------------------------------------
1    Finish C project               20-11-2026  High     Done
2    Buy notebook                   28-02-2027  Low      Pending
--------------------------------------------------------------------
```

## Future Scope

- Goal setting and goal progress tracking
- Database integration (replace the text files)
- Web version of the application
- Data analytics and charts for spending and productivity
- AI-powered recommendations for planning tasks and saving money
