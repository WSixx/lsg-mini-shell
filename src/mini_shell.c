
#define _POSIX_C_SOURCE 200809L

#include "mini_shell.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
            free(args);
            args = nullptr;

            free(line);
            line = nullptr;
            continue;
        }
        if (strcmp(args[0], "exit") == 0) {
            free(args);
            args = nullptr;

            free(line);
            line = nullptr;
            break;
        }

        execute(args);

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

void execute(char** args) {
    pid_t pid = fork();
    if (pid < 0) {  // falha ao criar
        perror("Error fork");
    } else if (pid == 0) {      // exec dentro processo filho
        execvp(args[0], args);  // rodar outro comando sem encerrar o programa pai

        // Só chega aqui se o execvp falhar
        perror("Error execvp");
        _exit(EXIT_FAILURE);

    } else {                       // exec dentro process pai
        waitpid(pid, nullptr, 0);  // guarda o filho terminar
    }
}