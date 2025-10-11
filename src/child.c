#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         fprintf(stderr, "Usage: child <filename>\n");
//         exit(EXIT_FAILURE);
//     }

//     FILE *file = fopen(argv[1], "w");
//     if (file == NULL)
//     {
//         perror("fopen");
//         exit(EXIT_FAILURE);
//     }

//     char line[1024];
//     while (fgets(line, sizeof(line), stdin) != NULL)
//     {
//         float numbers[100];
//         int count = 0;

//         char *token = strtok(line, " \n");
//         while (token != NULL && count < 100)
//         {
//             numbers[count++] = atof(token);
//             token = strtok(NULL, " \n");
//         }

//         if (count < 2)
//         {
//             fprintf(stderr, "Error: Need at least 2 numbers\n");
//             continue;
//         }

//         float dividend = numbers[0];
//         for (int i = 1; i < count; i++)
//         {
//             if (numbers[i] == 0.0f)
//             {
//                 fprintf(file, "Division by zero detected. Exiting.\n");
//                 fflush(file);
//                 int status_err = -1;
//                 fclose(file);
//                 exit(EXIT_FAILURE);
//             }
//             float result = dividend / numbers[i];
//             fprintf(file, "%.2f / %.2f = %.2f\n", dividend, numbers[i], result);
//         }
//         fflush(file);
//     }

//     fclose(file);
//     return 0;
// }
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void write_string(int fd, const char *str)
{
    write(fd, str, strlen(str));
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write_string(STDERR_FILENO, "Usage: child <filename>\n");
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1)
    {
        write_string(STDERR_FILENO, "open error\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    ssize_t bytes_read;
    char line[1024];
    int line_pos = 0;

    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        for (int i = 0; i < bytes_read; i++)
        {
            if (buffer[i] == '\n')
            {
                line[line_pos] = '\0';

                float numbers[100];
                int count = 0;
                char *token = strtok(line, " ");
                while (token && count < 100)
                {
                    numbers[count++] = atof(token);
                    token = strtok(NULL, " ");
                }

                if (count < 2)
                {
                    buffer[1024];
                    line[1024];
                    line_pos = 0;
                    write_string(1, "Error: Need at least 2 numbers\n");
                    continue;
                }
                else
                {
                    float dividend = numbers[0];
                    for (int j = 1; j < count; j++)
                    {
                        if (numbers[j] == 0.0f)
                        {
                            write_string(file, "Division by zero detected. Exiting.\n");
                            write_string(1, "Division by zero detected. Exiting.\n");
                            close(file);
                            exit(EXIT_FAILURE);
                        }
                        char result[64];
                        float res = dividend / numbers[j];
                        int len = snprintf(result, sizeof(result), "%.2f / %.2f = %.2f\n", dividend, numbers[j], res);
                        write(file, result, len);
                    }
                }
                line_pos = 0;
            }
            else
            {
                if (line_pos < sizeof(line) - 1)
                {
                    line[line_pos++] = buffer[i];
                }
            }
        }
    }

    close(file);
    return 0;
}