#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCRIPTION_LENGTH 100

struct Task
{
    char description[MAX_DESCRIPTION_LENGTH];
    int completed;
};

void printAllTasks(struct Task *pTasks, int tasksCount);
void addTask(struct Task **pTasks, char description[MAX_DESCRIPTION_LENGTH], int tasksCount);

int main()
{
    struct Task *pTasks = NULL;
    int tasksCount = 0;
    char input[200], action[10], arg[MAX_DESCRIPTION_LENGTH];

    while (1)
    {
        printf("> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            continue;

        if (strncmp(input, "quit", 4) == 0)
        {
            printf("Goodbye !\n");
            break;
        }

        if (sscanf(input, "%s %[^\n]", action, arg) != 2)
        {
            printf("Invalid input.\n");
            continue;
        }

        if (strncmp(action, "add", 3) == 0)
        {
            addTask(&pTasks, arg, tasksCount);
            tasksCount++;
        }
        else
        {
            printf("Unsupported action: %s\n", action);
            continue;
        }

        printAllTasks(pTasks, tasksCount);
    }

    free(pTasks);

    return 0;
}

void printAllTasks(struct Task *pTasks, int tasksCount)
{
    printf("\n");

    for (int i = 0; i < tasksCount; i++)
    {
        printf("[%c] %s (%d)\n", pTasks[i].completed ? 'x' : ' ', pTasks[i].description, i + 1);
    }

    printf("\n");
}

void addTask(struct Task **pTasks, char description[MAX_DESCRIPTION_LENGTH], int tasksCount)
{
    struct Task *tmpTasks = realloc(*pTasks, (tasksCount + 1) * sizeof(struct Task));

    if (tmpTasks == NULL)
    {
        printf("Memory reallocation failed.");
        free(*pTasks);
        exit(1);
    }

    *pTasks = tmpTasks;

    strncpy((*pTasks)[tasksCount].description, description, MAX_DESCRIPTION_LENGTH - 1);
    (*pTasks)[tasksCount].description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    (*pTasks)[tasksCount].completed = 0;

    printf("Added task #%d\n", tasksCount + 1);
}