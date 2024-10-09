#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    long *longs;
    size_t length;
    size_t capacity;
} LongList;

LongList long_list_create() {
    LongList long_list;

    long_list.capacity = 30;
    long_list.length = 0;
    long_list.longs = malloc(long_list.capacity * sizeof(long));

    return long_list;
}

// Returns `false` if the list could not be reallocated.
bool long_list_push(LongList *long_list, const long n) {
    const size_t length = long_list->length;
    const size_t capacity = long_list->capacity;

    if (length >= capacity) {
        const size_t new_capacity = (capacity * 2) + 1;
        long *temp_ptr = realloc(long_list->longs, new_capacity * sizeof(long));

        if (temp_ptr == nullptr) {
            return false;
        }

        long_list->capacity = new_capacity;
        long_list->longs = temp_ptr;
    }

    long_list->longs[length] = n;
    long_list->length++;

    return true;
}

void long_list_destroy(const LongList *long_list) {
    free(long_list->longs);
}

typedef enum {
    GET_LONGS_SUCCESS,
    // Non-whitespace, non-digit, non-sign
    GET_LONGS_INVALID_CHARACTER_ENCOUNTERED,
    GET_LONGS_STRING_TO_LONG_ERROR,
    GET_LONGS_MULTIPLE_SIGNS_IN_A_ROW,
} GetLongsError;

GetLongsError get_longs(LongList *long_list) {
    char last_char = '0';
    int sign = 1;
    long curr_n = 0;

    while (true) {
        const char c = (char)getchar();

        if (c == '\n' || c == EOF) {
            break;
        }

        if (isdigit(c)) {
            curr_n = curr_n * 10 + (c - '0');
        } else if (c == '+' || c == '-') {
            if (last_char == '+' || last_char == '-') {
                return GET_LONGS_MULTIPLE_SIGNS_IN_A_ROW;
            }

            long_list_push(long_list, curr_n * sign);
            curr_n = 0;

            if (c == '+') {
                sign = 1;
            } else {
                sign = -1;
            }
        } else if (!isspace(c)) {
            return GET_LONGS_INVALID_CHARACTER_ENCOUNTERED;
        }

        last_char = c;
    }

    long_list_push(long_list, curr_n);

    for (size_t i = 0; i < long_list->length; i++) {
        printf("n[%zu] %ld\n", i, long_list->longs[i]);
    }

    return GET_LONGS_SUCCESS;
}

int main(void) {
    LongList long_list = long_list_create();

    switch (get_longs(&long_list)) {
        case GET_LONGS_SUCCESS:
            break;
        case GET_LONGS_INVALID_CHARACTER_ENCOUNTERED:
            printf("Invalid character encountered\n");
            return -1;
        case GET_LONGS_STRING_TO_LONG_ERROR:
            printf("Couldn't convert to number\n");
            return -1;
        case GET_LONGS_MULTIPLE_SIGNS_IN_A_ROW:
            printf("Multiple signs (+ or -) in a row.\n");
            return -1;
    }

    long sum = 0;

    for (size_t i = 0; i < long_list.length; i++) {
        sum += long_list.longs[i];
    }

    printf("Success. Result: %ld", sum);
    long_list_destroy(&long_list);

    return 0;
}
