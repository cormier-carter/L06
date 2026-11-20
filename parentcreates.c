#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: parentcreates <iterations>\n");
        return 1;
    }

    int iterations = strtol(argv[1], NULL, 10);
    int status = 0;

    for (int i = 0; i < iterations; i++) {
        pid_t child = fork();
        if (child < 0) {
            perror("fork");
            status = 1;
            break;
        }

        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
        if (child == 0) {
            return 0;
        }
        // The next child would inherit any output still in the parent's buffer.
        fflush(stdout);
    }

    while (wait(NULL) != -1 || errno == EINTR) {
        // Reap all the children after the last fork.
    }
    return status;
}
