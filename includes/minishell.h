#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <termcap.h>

/*-----------------------defenitions----------------------*/
/*--------------------------------------------------------*/
# define WHITE_SPACES " \t\n\v\f\r"
# define TOKENS "<>|"

/*-------------------------structs------------------------*/
/*--------------------------------------------------------*/
typedef struct s_shell
{
	char	*line;
}			t_shell;

typedef enum e_token_type
{
	PIPE,
	WORD,
	RE_INPUT,
	RE_OUTPUT,
	RE_APPEND,
	RE_HERE_DOC,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	struct s_token	*next;
}					t_token;

/*-----------------------Minishell------------------------*/
/*--------------------------------------------------------*/
void	loop(t_shell *shell);

/*---------------------syntax checking--------------------*/
/*--------------------------------------------------------*/
bool	check_syntax_errors(char *line);
bool	check_quotation_marks(char *line);
bool	check_pipes(char *line);

/*----------------------close program---------------------*/
/*--------------------------------------------------------*/
void	clean_up(t_shell *shell);
void	error_message(char *message);
void	exit_error_message(char *message);

/*----------------------tokenization----------------------*/
/*--------------------------------------------------------*/
t_token	*tokenize(char *line);

#endif
