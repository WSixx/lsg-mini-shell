#include "mini_shell.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char* line = nullptr;
    char** args = nullptr;

    while (true) {
        printf("> ");
        line = read_line();
        if (line == nullptr) {
            break;
        }
        args = split_line(line);

        // erro de aloc ou vazio
        if (args == nullptr || args[0] == nullptr) {
            continue;
        }

        free(args);
        args = nullptr;

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

[[nodiscard]] char** split_line(char* line) {
    size_t capacity = 8;
    size_t count = 0;
    char** tokens = malloc(capacity * sizeof(char*));
    if (tokens == nullptr) {
        return nullptr;
    }
    char* token = strtok(line, " \t");

    while (token != nullptr) {
        tokens[count++] = token;
        token = strtok(nullptr, " \t");
        if (count >= capacity - 1) {
            capacity *= 2;
            size_t newCapacity = capacity * sizeof(char*);
            char** temp = realloc(tokens, newCapacity);
            if (temp == nullptr) {
                free(tokens);
                return nullptr;
            }
            tokens = temp;
        }
    }
    tokens[count] = nullptr;
    return tokens;
}