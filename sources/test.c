#include "minishell.h"

int main(void)
{
    char *input;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        if (*input)
            add_history(input);
        printf("Input: %s\n", input);
        free(input);
    }
    return 0;
}
