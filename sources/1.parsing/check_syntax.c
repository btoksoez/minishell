#include "../../includes/minishell.h"

bool	check_quotation_marks(char *line)
{
	int	n_double;
	int	n_single;

	n_double = 0;
	n_single = 0;
	while (*line)
	{
		if (*line == '\"')
			n_double++;
		else if (*line == '\'')
			n_single++;
		line++;
	}
	if (n_double % 2 != 0 || n_single % 2 != 0)
		return (error_message("parse error: unclosed quote"), true);
	return (false);
}

bool	check_pipes(char *line)
{
	char	*first_pipe;

	first_pipe = ft_strchr(line, '|');
	if (!first_pipe)
		return (false);
	first_pipe++;
	while (*first_pipe == ' ')
		first_pipe++;
	if (*first_pipe == '|')
		return (error_message("parse error: near `||'"), true);
	if (ft_strncmp(line, "|", 1) == 0 || line[ft_strlen(line) - 1] == '|')
		return (error_message("parse error: near `|'"), true);
	return (false);
}

bool	check_redirection(char *line)
{
	while (*line)
	{
		if (*line == '\'')
		{
			line++;
			while (*line && *line != '\'')
				line++;
		}
		if (*line == '\"')
		{
			line++;
			while (*line && *line != '\"')
				line++;
		}
		if (ft_strncmp(line, "<>", 2) == 0)
			return (error_message("parse error: near `>'"), true);
		if (ft_strncmp(line, "><", 2) == 0)
			return (error_message("parse error: near `<'"), true);
		line++;
	}
	return (false);
}

bool	check_syntax_errors(char *line)
{
	if (check_quotation_marks(line))
		return (true);
	if (check_pipes(line))
		return (true);
	if (check_redirection(line))
		return (true);
	return (false);
}
