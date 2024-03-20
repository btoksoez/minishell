#include "../includes/minishell.h"

void loop(void)
{
	char	*line;
	// char	**args;
	// int		status;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break;
		if (*line)
			add_history(line);
		printf("Input: %s\n", line);
		if (check_errors(line))
			return ;
		// args = parse(line);
		// status = execute(args);
		free(line);
	}
}
