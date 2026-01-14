#define _GNU_SOURCE
#include <stdio.h>    // printf, getline
#include <stdlib.h>   // free, exit
#include <string.h>   // strcmp, strcspn, strtok
#include <unistd.h>   // write, chdir, access
#include <sys/types.h> // pid_t

#define PROMPT "mini_bash$ "

int main() {
    char *line = NULL;
    size_t len = 0;

    while (1) {
        // print prompt
        write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

        //read input line
        if (getline(&line, &len, stdin) == -1) {
            write(STDOUT_FILENO, "\n", 1);
            break; 
        }

        // remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        //skip empty input
        if (line[0] == '\0') continue;

        // internal command: exit
        if (strcmp(line, "exit") == 0) break;

        // parse input into tokens
        char *argv[100]; // max 100 tokens
        int argc = 0;
        char *token = strtok(line, " \t");
        while (token != NULL && argc < 99) {
            argv[argc++] = token;
            token = strtok(NULL, " \t");
        }
        argv[argc] = NULL;

        // cd
        if (strcmp(argv[0], "cd") == 0) {
            if (argc < 2) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                if (chdir(argv[1]) != 0) {
                    perror("cd");
                }
            }
            continue; // skip 
        }

        // external command execution
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            continue;
        }

        if (pid == 0) {
            // child process
            char *home = getenv("HOME");
            char path[512];

            // trying $HOME/command
            if (home) {
                snprintf(path, sizeof(path), "%s/%s", home, argv[0]);
                if (access(path, X_OK) == 0) {
                    execv(path, argv);
                    perror("execv failed");
                    exit(1);
                }
            }

            // trying bin/command
            snprintf(path, sizeof(path), "/bin/%s", argv[0]);
            if (access(path, X_OK) == 0) {
                execv(path, argv);
                perror("execv failed");
                exit(1);
            }

            // not found
            fprintf(stderr, "%s: unknown command\n", argv[0]);
            exit(127);
        } else {
            // parent process
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("command exited with code %d\n", WEXITSTATUS(status));
            }
        }
    }

    free(line);
    return 0;
}
