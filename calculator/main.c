#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
    char input[100];
    float operand1, operand2, result = 0.0;
    char operator;

    printf("Calculator ready. Enter an operation and press enter. Type `quit` to stop.\n");

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

        if (sscanf(input, "%f %c %f", &operand1, &operator, &operand2) != 3)
        {
            printf("Invalid input.\n");
            continue;
        }

        switch (operator)
        {
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        case '/':
            if (operand2 == 0.0)
            {
                printf("Fatal error: Division by zero\n");
                continue;
            }

            result = operand1 / operand2;
            break;
        case '^':
            result = pow(operand1, operand2);
            break;
        default:
            printf("Unsupported operator: %c.\n", operator);
            continue;
        }

        printf("= %.2f\n", result);
    }

    return 0;
}