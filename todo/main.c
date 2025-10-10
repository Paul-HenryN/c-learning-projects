#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCRIPTION_LENGTH 100
#define FILENAME "todo.txt"

typedef struct
{
    char description[MAX_DESCRIPTION_LENGTH];
    int completed;
} Task;

void printAllTasks(Task *pTasks, int tasksCount);
void addTask(Task **pTasks, char description[MAX_DESCRIPTION_LENGTH], int tasksCount);
void toggleTask(Task *pTasks, int taskIndex, int tasksCount);
void checkAll(Task *pTasks, int tasksCount);
void uncheckAll(Task *pTasks, int tasksCount);
void saveTasks(Task *pTasks, int tasksCount);
void readTasks(Task **pTasks, int *tasksCount);

int main()
{
    Task *pTasks = NULL;
    int tasksCount = 0;
    char input[200], action[10], arg[MAX_DESCRIPTION_LENGTH];

    readTasks(&pTasks, &tasksCount);

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

        if (sscanf(input, "%s %[^\n]", action, arg) < 1)
        {
            printf("Invalid input.\n");
            continue;
        }

        if (strncmp(action, "add", 3) == 0)
        {
            addTask(&pTasks, arg, tasksCount);
            tasksCount++;
            saveTasks(pTasks, tasksCount);
            printAllTasks(pTasks, tasksCount);
        }
        else if (strncmp(action, "list", 4) == 0)
        {
            printAllTasks(pTasks, tasksCount);
        }
        else if (strncmp(action, "toggle", 6) == 0)
        {
            int taskIndex;

            if (sscanf(arg, "%d", &taskIndex) != 1)
            {
                printf("Invalid task index: %s\n", arg);
                continue;
            }

            toggleTask(pTasks, taskIndex, tasksCount);
            saveTasks(pTasks, tasksCount);
            printAllTasks(pTasks, tasksCount);
        }
        else if (strncmp(action, "check-all", 8) == 0)
        {
            checkAll(pTasks, tasksCount);
            saveTasks(pTasks, tasksCount);
            printAllTasks(pTasks, tasksCount);
        }
        else if (strncmp(action, "uncheck-all", 10) == 0)
        {
            uncheckAll(pTasks, tasksCount);
            saveTasks(pTasks, tasksCount);
            printAllTasks(pTasks, tasksCount);
        }
        else
        {
            printf("Unsupported action: %s\n", action);
            continue;
        }
    }

    free(pTasks);

    return 0;
}

void printAllTasks(Task *pTasks, int tasksCount)
{
    printf("\n");

    if (tasksCount == 0)
    {
        printf("No task yet.\n");
    }

    for (int i = 0; i < tasksCount; i++)
    {
        printf("[%c] %s (%d)\n", pTasks[i].completed ? 'x' : ' ', pTasks[i].description, i + 1);
    }

    printf("\n");
}

void addTask(Task **pTasks, char description[MAX_DESCRIPTION_LENGTH], int tasksCount)
{
    Task *tmpTasks = realloc(*pTasks, (tasksCount + 1) * sizeof(Task));

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

void toggleTask(Task *pTasks, int taskIndex, int tasksCount)
{
    if (taskIndex < 1 || taskIndex > tasksCount)
    {
        printf("Task index out of bounds: %d\n", taskIndex);
        return;
    }

    int completed = pTasks[taskIndex - 1].completed;

    pTasks[taskIndex - 1].completed = !completed;
}

void checkAll(Task *pTasks, int tasksCount)
{
    for (int i = 0; i < tasksCount; i++)
    {
        pTasks[i].completed = 1;
    }
}

void uncheckAll(Task *pTasks, int tasksCount)
{
    for (int i = 0; i < tasksCount; i++)
    {
        pTasks[i].completed = 0;
    }
}

void saveTasks(Task *pTasks, int tasksCount)
{
    FILE *pFile = fopen(FILENAME, "w");

    if (pFile == NULL)
    {
        printf("Failed to create / open file: %s", FILENAME);
        return;
    }

    for (int i = 0; i < tasksCount; i++)
    {
        Task task = pTasks[i];
        fprintf(pFile, "%d %s\n", task.completed, task.description);
    }

    fclose(pFile);
}

void readTasks(Task **pTasks, int *tasksCount)
{
    FILE *pFile = fopen(FILENAME, "r");
    char buffer[200];

    if (pFile == NULL)
        return;

    while (fgets(buffer, sizeof(buffer), pFile))
    {
        char description[MAX_DESCRIPTION_LENGTH];
        int completed;

        if (sscanf(buffer, "%d %[^\n]", &completed, description) != 2)
            continue;

        Task *tmpTasks = realloc(*pTasks, (*tasksCount + 1) * sizeof(Task));

        if (tmpTasks == NULL)
        {
            printf("Memory reallocation failed.");
            free(*pTasks);
            exit(1);
        }

        *pTasks = tmpTasks;

        strcpy((*pTasks)[*tasksCount].description, description);
        (*pTasks)[*tasksCount].completed = completed;

        *tasksCount = *tasksCount + 1;
    }

    fclose(pFile);
}