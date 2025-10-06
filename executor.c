#include "executor.h"

/**
 * Execute a command using the fork-exec-wait pattern
 *
 * This function demonstrates the fundamental process management pattern
 * used by all Unix shells:
 * 1. Fork a child process
 * 2. Child calls exec to transform into the target command
 * 3. Parent waits for child to complete
 *
 * @param command The command to execute (e.g., "ls", "pwd", "echo")
 * @param args Array of arguments: [command, arg1, arg2, ..., NULL]
 *             Example: {"ls", "-l", NULL} or {"echo", "Hello", NULL}
 * @return Exit status of the command (0=success, non-zero=failure, -1=error)
 */
int execute_command(char *command, char **args) {
    pid_t pid;
    int status;

    pid = fork(); // Forks a child process
    if (pid < 0) {
        perror("fork failed");
        return -1; // returns -1 if fork failed
    }

    // checks if we are in the child process
    if (pid == 0) {
        execvp(command, args);

        // If execvp returns, it failed. Prints an error and exits with 1.
        perror("exec() has failed");
        exit(1);
    }

    waitpid(pid, &status, 0);

    // If the child has exited normally, return the exit status. Else, print an error and return -1.
    if (WIFEXITED(status)) {
        return (WEXITSTATUS(status));
    } else {
        perror("child did not exit normally");
        return -1;
    }
}