#define _GNU_SOURCE
#include <stdio.h>  // printf getline
#include <stdlib.h> // free
#include <string.h> // strcmp
#include <unistd.h> // write

#define PROMPT "mini_bash$ "

int main()
{
    char *line = NULL; // buffer for user input
    size_t len = 0;    // buffer size

    while (1)
    {
        //  print promptt
        write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

        //   read input line
        if (getline(&line, &len, stdin) == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        // remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // skip empty input
        if (line[0] == '\0')
            continue;

        //  exit
        if (strcmp(line, "exit") == 0)
            break;

        // parse input into tokens
        char *argv[100]; // max 100 tokens
        int argc = 0;

        char *token = strtok(line, " \t"); // split by space or tab
        while (token != NULL && argc < 99)
        {
            argv[argc++] = token;
            token = strtok(NULL, " \t");
        }
        argv[argc] = NULL; // null-terminate for exec

        //  cd
        if (strcmp(argv[0], "cd") == 0)
        {
            if (argc < 2)
            {
                fprintf(stderr, "cd: missing argument\n");
            }
            else
            {
                if (chdir(argv[1]) != 0)
                {
                    perror("cd");
                }
            }
            continue; // skip
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            continue;
        }

        if (pid == 0)
        {
            // try executing command in $HOME
            char *home = getenv("HOME");
            char path[512];

            if (home)
            {
                snprintf(path, sizeof(path), "%s/%s", home, argv[0]);
                execv(path, argv);
                // iffails, try /bin
            }

            snprintf(path, sizeof(path), "/bin/%s", argv[0]);
            execv(path, argv);

            // if here- execution failed
            perror("execv failed");
            exit(1);
        }
        else
        {
            // waiting for child
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                printf("command exited with code %d\n", WEXITSTATUS(status));
            }
        }
    }

    free(line);
    return 0;
}
