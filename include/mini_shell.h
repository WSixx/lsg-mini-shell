#ifndef MINI_SHELL_H
#define MINI_SHELL_H

[[nodiscard]] char* read_line();
[[nodiscard]] char** split_line(char* line);
void execute(char** args);

#endif