#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: child <filename>\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "w");
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        float numbers[100];
        int count = 0;

        char *token = strtok(line, " \n");
        while (token != NULL && count < 100)
        {
            numbers[count++] = atof(token);
            token = strtok(NULL, " \n");
        }

        if (count < 2)
        {
            fprintf(stderr, "Error: Need at least 2 numbers\n");
            continue;
        }

        float dividend = numbers[0];
        for (int i = 1; i < count; i++)
        {
            if (numbers[i] == 0.0f)
            {
                fprintf(file, "Division by zero detected. Exiting.\n");
                fflush(file);
                int status_err = -1;
                fclose(file);
                exit(EXIT_FAILURE);
            }
            float result = dividend / numbers[i];
            fprintf(file, "%.2f / %.2f = %.2f\n", dividend, numbers[i], result);
        }
        fflush(file);
    }

    fclose(file);
    return 0;
}