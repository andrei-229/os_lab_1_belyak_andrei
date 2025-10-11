// #define _GNU_SOURCE
#include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <string.h>

// int main(){
//     int pipe1[2], pipe2[2];
//     pid_t pid;

//     char filename[256];
//     char buffer[1024];

//     if (pipe(pipe1) == -1 || pipe(pipe2) == -1){
//         perror("pipe error");
//         exit(EXIT_FAILURE);
//     }

//     printf("Enter filename: ");
//     if (fgets(filename, sizeof(filename), stdin) == NULL)
//     {
//         perror("fgets");
//         exit(EXIT_FAILURE);
//     }
//     filename[strcspn(filename, "\n")] = 0;

//     pid = fork();

//     if (pid == -1){
//         perror("fork error");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0){
//         close(pipe1[1]);
//         close(pipe2[0]);

//         if (dup2(pipe1[0], STDIN_FILENO) == -1){
//             perror("dup2 pipe1[0]");
//             exit(EXIT_FAILURE);
//         }
//         close(pipe1[0]);
//         execl("./child", "child", filename, NULL);
//         perror("execl error");
//         exit(EXIT_FAILURE);
//     }
//     else{
//         close(pipe1[0]);
//         close(pipe2[1]);
//         printf("Enter numbers separated by spaces: ");
//         while (fgets(buffer, sizeof(buffer), stdin) != NULL){
//             write(pipe1[1], buffer, strlen(buffer));

//             int status;
//                 pid_t res = waitpid(pid, &status, WNOHANG);
//                 if (res != 0){
//                     break;
//                 }

//                 printf("Enter next numbers: ");
//             fflush(stdout);
//         }
//         close(pipe1[1]);
//         close(pipe2[0]);
//         waitpid(pid, NULL, 0);
//     }
    
//     return 0;
// }
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void write_string(int fd, const char *str)
{
    write(fd, str, strlen(str));
}

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;

    char filename[256];
    char buffer[1024];
    ssize_t bytes_read;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        write_string(STDERR_FILENO, "pipe error\n");
        exit(EXIT_FAILURE);
    }

    write_string(STDOUT_FILENO, "Enter filename: ");
    bytes_read = read(STDIN_FILENO, filename, sizeof(filename) - 1);
    if (bytes_read <= 0)
    {
        write_string(STDERR_FILENO, "read error\n");
        exit(EXIT_FAILURE);
    }
    filename[bytes_read - 1] = '\0';

    pid = fork();

    if (pid == -1)
    {
        write_string(STDERR_FILENO, "fork error\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        if (dup2(pipe1[0], STDIN_FILENO) == -1)
        {
            write_string(STDERR_FILENO, "dup2 error\n");
            exit(EXIT_FAILURE);
        }
        close(pipe1[0]);
        execl("./child", "child", filename, NULL);
        write_string(STDERR_FILENO, "execl error\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipe1[0]);
        close(pipe2[1]);

        write_string(STDOUT_FILENO, "Enter numbers separated by spaces: ");
        while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            write(pipe1[1], buffer, bytes_read);

            int status;
            pid_t res = waitpid(pid, &status, WNOHANG);
            if (res != 0)
            {
                break;
            }

            write_string(STDOUT_FILENO, "Enter next numbers: ");
        }
        close(pipe1[1]);
        close(pipe2[0]);
        waitpid(pid, NULL, 0);
    }

    return 0;
}