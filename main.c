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

typedef struct {
    char *chars;
    size_t length;
    size_t capacity;
} String;

String string_create() {
    String str;

    str.capacity = 30;
    str.length = 0;
    str.chars = malloc(str.capacity * sizeof(char));

    return str;
}

// Returns `false` if the list could not be reallocated.
bool string_push(String *str, const char c) {
    const size_t length = str->length;
    const size_t capacity = str->capacity;

    if (length >= capacity) {
        const size_t new_capacity = (capacity * 2) + 1;
        char *temp_ptr = realloc(str->chars, new_capacity * sizeof(char));

        if (temp_ptr == nullptr) {
            return false;
        }

        str->capacity = new_capacity;
        str->chars = temp_ptr;
    }

    str->chars[length] = c;
    str->length++;

    return true;
}

// Returns `false` if the index is out of bounds.
bool string_remove(String *str, const size_t i) {
    if (i >= str->length) {
        return false;
    }

    for (size_t j = i; j < str->length - 1; j++) {
        str->chars[j] = str->chars[j + 1];
    }

    str->length--;
    return true;
}

bool string_contains(const String *str, char c) {
    for (int i = 0; i < str->length; i++) {
        if (str->chars[i] == c) {
            return true;
        }
    }

    return false;
}

// Returns `false` if there was a conversion error.
bool string_to_long(const String *str, long *l) {
    errno = 0;
    char *end_ptr;

    *l = strtol(str->chars, &end_ptr, 10);

    errno = 0;

    if (errno != 0) {
        return false;
    }

    return true;
}

void string_destroy(const String *str) {
    free(str->chars);
}

typedef enum {
    GET_LONGS_SUCCESS,
    // Non-whitespace, non-digit, non-sign
    GET_LONGS_INVALID_CHARACTER_ENCOUNTERED,
    GET_LONGS_STRING_TO_LONG_ERROR,
    GET_LONGS_MULTIPLE_SIGNS_IN_A_ROW,
} GetLongsError;

// Returns `false` if something else other than whitespace, a digit, or a +/- sign was encountered.
GetLongsError get_longs(LongList *long_list) {
    String curr_str = string_create();
    char last_char = '0';

    while (true) {
        const char c = getchar();

        if (c == '\n') {
            break;
        }

        if (isdigit(c)) {
            string_push(&curr_str, c);
        } else if (c == '+' || c == '-') {
            if (last_char == '+' || last_char == '-') {
                return GET_LONGS_MULTIPLE_SIGNS_IN_A_ROW;
            }

            long l;

            if (!string_to_long(&curr_str, &l)) {
                return GET_LONGS_STRING_TO_LONG_ERROR;
            }

            string_destroy(&curr_str);
            long_list_push(long_list, l);

            curr_str = string_create();
            string_push(&curr_str, c);
        } else if (!isspace(c)) {
            return GET_LONGS_INVALID_CHARACTER_ENCOUNTERED;
        }

        last_char = c;
    }

    long l;

    if (!string_to_long(&curr_str, &l)) {
        return GET_LONGS_STRING_TO_LONG_ERROR;
    }

    string_destroy(&curr_str);
    long_list_push(long_list, l);

    for (size_t i = 0; i < long_list->length; i++) {
        printf("n[%zu] %d\n", i, long_list->longs[i]);
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

    printf("Success. Result: %d", sum);
    long_list_destroy(&long_list);

    return 0;
}
