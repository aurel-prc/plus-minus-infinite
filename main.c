#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void) {
    printf("Enter numbers with + or - between them:");

    char last_char = '0';
    int sign = 1;
    long curr_n = 0;
    long sum = 0;
    char c;

    while ((c = (char)getchar()) != '\n') {
        if (isdigit(c)) {
            curr_n = curr_n * 10 + (c - '0');
        } else if (c == '+' || c == '-') {
            if (last_char == '+' || last_char == '-') {
                fprintf(stderr, "Error: Multiple signs in a row\n");
                return -1;
            }

            sum += curr_n * sign;
            curr_n = 0;

            if (c == '+') {
                sign = 1;
            } else {
                sign = -1;
            }
        } else if (!isspace(c)) {
            fprintf(stderr, "Error: Invalid character %c\n", c);
            return -1;
        }

        last_char = c;
    }

    sum += curr_n * sign;

    printf("Result: %ld", sum);

    return 0;
}
