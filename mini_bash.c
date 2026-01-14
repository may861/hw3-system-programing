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
    //  print promptt
    write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

    //   ead input line
    if (getline(&line, &len, stdin) == -1) {
        write(STDOUT_FILENO, "\n", 1);
        break;
    }

    // remove trailing newline
    line[strcspn(line, "\n")] = '\0';

    // skip empty input
    if (line[0] == '\0') continue;

    // internal command: exit
    if (strcmp(line, "exit") == 0) break;

    // parse input into tokens
    char *argv[100]; // max 100 tokens
    int argc = 0;

    char *token = strtok(line, " \t"); // split by space or tab
    while (token != NULL && argc < 99) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }
    argv[argc] = NULL; // null-terminate for exec

    //print tokens
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
}


    free(line);
    return 0;
}
