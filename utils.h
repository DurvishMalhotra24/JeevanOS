#ifndef UTILS_H
#define UTILS_H

/*
 * utils.h - Helper functions for safe input and formatted output.
 */

/* Reads one line from the keyboard into buf (max size-1 characters).
 * Removes the newline. Returns 1 on success, 0 if input has ended (EOF). */
int read_line(char *buf, int size);

/* Asks for an integer between min and max; repeats until the input is valid. */
int read_int(const char *prompt, int min, int max);

/* Asks for a decimal number between min and max; repeats until valid. */
float read_float(const char *prompt, float min, float max);

/* Asks for a non-empty text. The '|' character is replaced by '/'
 * because '|' is used as the separator in the data files. */
void read_text(const char *prompt, char *buf, int size);

/* Asks for a date in DD-MM-YYYY format and validates it.
 * buf must have room for at least 11 characters. */
void read_date(const char *prompt, char *buf);

/* Returns 1 if d-m-y is a real calendar date (year 2000 to 2100), else 0. */
int is_valid_date(int d, int m, int y);

/* Returns 1 if word appears inside text, ignoring upper/lower case. */
int contains_nocase(const char *text, const char *word);

/* Waits for the user to press Enter. */
void pause_screen(void);

/* Prints the character ch, n times, followed by a newline. */
void print_line(char ch, int n);

#endif
