#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"
#include "utils.h"

/*
 * student.c - Student details page.
 */

void show_student_page(void)
{
    printf("\n");
    print_line('=', 60);
    printf("              STUDENT DETAILS\n");
    print_line('=', 60);
    printf("  Name      : %s\n", STUDENT_NAME);
    printf("  Roll No   : %s\n", STUDENT_ROLL_NO);
    printf("  Branch    : %s\n", STUDENT_BRANCH);
    printf("  Section   : %s\n", STUDENT_SECTION);
    printf("  Semester  : %s\n", STUDENT_SEMESTER);
    print_line('-', 60);
    printf("  Course    : %s\n", STUDENT_COURSE);
    printf("  Project   : %s\n", PROJECT_TITLE);
    printf("  Type      : %s Semester %s Capstone Project\n", "PPS", STUDENT_SEMESTER);
    print_line('=', 60);
}
