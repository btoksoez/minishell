#include "../includes/minishell.h"

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
		return (error_message("syntax error: unclosed quote"), true);
	return (false);
}

bool	check_pipes(char *line)
{
	char	*first_pipe;

	if (ft_strncmp(line, "|", 1) == 0)
		return (error_message("syntax error: no command before pipe"), true);
	if (line[ft_strlen(line) - 1] == '|')
		return (error_message("syntax error: no command after pipe"), true);
	first_pipe = ft_strchr(line, '|');
	if (!first_pipe)
		return (false);
	first_pipe++;
	while (*first_pipe == ' ')
		first_pipe++;
	if (*first_pipe == '|')
		return (error_message("syntax error: duplicate pipe"), true);
	return (false);
}

bool	check_syntax_errors(char *line)
{
	if (check_quotation_marks(line) || check_pipes(line))
	{
		free(line);
		return (true);
	}
	return (false);
}


/*
    Misuse of Pipes (|):
        Double pipe ||: Incorrect syntax, typically used in conditional logic (&& and || are for logical operators, not for pipes).
        Missing command after pipe: command1 | without specifying a command to receive the piped output.
        Extra pipe: command1 || | command2 with an extra | after ||.
        Incorrect pipe placement: | command2 without a preceding command to provide output.

    Misuse of Input Redirection (<):
        Missing input file: command < without specifying an input file.
        Invalid input file path: command < non_existing_file where non_existing_file does not exist.
        Incorrect placement: < input_file command where < is placed after the command.

		works:
		< valid file
		cmd < 

		doesnt work:
		< invalid file
		cmd <

		Note: if on the right always assumes it is a file
		NOte: if on the left always assumes it is a cmd
		Doesnt need both (file is mandatory, cmd is optional)

    Misuse of Output Redirection (>, >>):
        Missing output file: command > or command >> without specifying an output file.
        Invalid output file path: command > non_existing_file or command >> non_existing_file where non_existing_file does not exist (for writing).
        Incorrect placement: command > output_file command or command >> output_file command where > or >> is placed after the command.
        Misuse of append (>>) with incorrect syntax: command >>| output_file or command >| output_file.

    Misuse of Error Redirection (2>):
        Missing error output file: command 2> without specifying an error output file.
        Invalid error output file path: command 2> non_existing_error_file where non_existing_error_file does not exist (for error redirection).
        Incorrect placement: command 2> error_file command where 2> is placed after the command.

    Combining Redirections:
        Combining multiple redirections incorrectly: command > output_file < input_file or command 2> error_file > output_file.
        Misplaced redirections: command < input_file > output_file where redirections are not in the correct order.

    Misuse of Compound Commands:
        Incorrect compound command syntax with redirections and pipes: command1 && command2 | command3, command1 | command2 && command3, etc.
        Misuse of parentheses or braces with incorrect redirections: (command1 > output_file) | command2, { command1 > output_file; } | command2.
		*/
