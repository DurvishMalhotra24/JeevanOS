#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/*
 * utils.c - Helper functions for safe input and formatted output.
 */

int read_line(char *buf, int size)
{
    size_t len;
    int c;

    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return 0;
    }

    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        /* The line was longer than the buffer: discard the extra characters. */
        while ((c = getchar()) != '\n' && c != EOF) {
            ;
        }
    }
    return 1;
}

static void exit_on_eof(void)
{
    printf("\nInput closed. Exiting program.\n");
    exit(0);
}

int read_int(const char *prompt, int min, int max)
{
    char buf[64];
    char *end;
    long value;

    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, (int)sizeof(buf))) {
            exit_on_eof();
        }

        value = strtol(buf, &end, 10);
        if (end == buf || *end != '\0') {
            printf("  Invalid number. Please try again.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("  Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

float read_float(const char *prompt, float min, float max)
{
    char buf[64];
    char *end;
    double value;

    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, (int)sizeof(buf))) {
            exit_on_eof();
        }

        value = strtod(buf, &end);
        if (end == buf || *end != '\0') {
            printf("  Invalid number. Please try again.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("  Please enter a value between %.2f and %.2f.\n", min, max);
            continue;
        }
        return (float)value;
    }
}

void read_text(const char *prompt, char *buf, int size)
{
    int i;

    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, size)) {
            exit_on_eof();
        }
        if (buf[0] == '\0') {
            printf("  This field cannot be empty.\n");
            continue;
        }
        for (i = 0; buf[i] != '\0'; i++) {
            if (buf[i] == '|') {
                buf[i] = '/';
            }
        }
        return;
    }
}

int is_valid_date(int d, int m, int y)
{
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leap;

    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) {
        return 0;
    }

    leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (m == 2 && leap) {
        return d <= 29;
    }
    return d <= days_in_month[m - 1];
}

void read_date(const char *prompt, char *buf)
{
    char line[32];
    int d, m, y;

    for (;;) {
        printf("%s", prompt);
        if (!read_line(line, (int)sizeof(line))) {
            exit_on_eof();
        }
        if (sscanf(line, "%d-%d-%d", &d, &m, &y) == 3 && is_valid_date(d, m, y)) {
            snprintf(buf, 11, "%02d-%02d-%04d", d, m, y);
            return;
        }
        printf("  Invalid date. Use DD-MM-YYYY (example: 25-12-2026).\n");
    }
}

int contains_nocase(const char *text, const char *word)
{
    size_t i, j;
    size_t text_len = strlen(text);
    size_t word_len = strlen(word);

    if (word_len == 0) {
        return 1;
    }

    for (i = 0; i + word_len <= text_len; i++) {
        for (j = 0; j < word_len; j++) {
            if (tolower((unsigned char)text[i + j]) != tolower((unsigned char)word[j])) {
                break;
            }
        }
        if (j == word_len) {
            return 1;
        }
    }
    return 0;
}

void pause_screen(void)
{
    char tmp[8];

    printf("\nPress Enter to continue...");
    read_line(tmp, (int)sizeof(tmp));
}

void print_line(char ch, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        putchar(ch);
    }
    putchar('\n');
}
