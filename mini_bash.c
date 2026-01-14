#define _GNU_SOURCE
#include <stdio.h>    // printf getline
#include <stdlib.h>   // free
#include <string.h>   // strcmp
#include <unistd.h>   // write

#define PROMPT "mini_bash$ "

int main() {
    char *line = NULL;     // buffer for user input
    size_t len = 0;        // buffer size

    while (1) {
        // 1. Print prompt
        write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

        // 2. Read input line
        if (getline(&line, &len, stdin) == -1) {
            // EOF (Ctrl+D)
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // 3. Internal command: exit
        if (strcmp(line, "exit") == 0) {
            break;
        }
    }

    free(line);
    return 0;
}
