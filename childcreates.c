#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: childcreates <iterations>\n");
        return 1;
    }

    int iterations = strtol(argv[1], NULL, 10);

    for (int i = 0; i < iterations; i++) {
        pid_t child = fork();
        if (child < 0) {
            perror("fork");
            return 1;
        }

        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
        fflush(stdout);
        if (child > 0) {
            // This parent has made its one child. Only the child keeps looping.
            int status;
            while (waitpid(child, &status, 0) == -1) {
                if (errno != EINTR) {
                    perror("waitpid");
                    return 1;
                }
            }
            return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
        }
    }

    return 0;
}
