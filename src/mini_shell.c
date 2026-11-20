#include "mini_shell.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char* line = nullptr;

    while (true) {
        printf("> ");
        line = read_line();
        if (line == nullptr) {
            break;
        }
        free(line);
        line = nullptr;
    }

    return 0;
}

[[nodiscard]] char* read_line() {
    size_t capacity = 64;
    size_t length = 0;
    char* buffer = malloc(capacity);

    if (buffer == nullptr) {
        return nullptr;
    }

    int c = 0;
    while (true) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            break;
        }
        buffer[length] = c;
        length++;
        if (length >= capacity - 1) {
            capacity = capacity * 2;
            char* tempP = realloc(buffer, capacity);
            if (tempP == nullptr) {
                free(buffer);
                return nullptr;
            }
            buffer = tempP;
        }
    }
    buffer[length] = '\0';
    return buffer;
}