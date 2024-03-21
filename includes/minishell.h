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

# define SINGLE_TOKENS "<>|"
# define WHITESPACE " \t\n\v\f\r"
# define QUOTE_DELIMITER "$\""

/*--------------------------structs-----------------------*/
/*--------------------------------------------------------*/

typedef enum e_token_type
{
	PIPE,		//for pipes
	RE_INPUT,	//for '<'
	RE_OUTPUT,	//for '>'
	APPEND,		//for '>>'
	HEREDOC,	//for '<<'
	WORD,		//for args and commands
	ENV_VAR		//for environment variables
}	t_token_type;

typedef struct	s_tree_node
{
	t_token_type		type;
	char				*args;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
}						t_tree_node;

typedef struct s_tokens
{
	t_token_type		type;
	char				*value;
	struct s_tokens		*previous;
	struct s_tokens		*next;
}						t_tokens;


typedef struct s_shell
{
	char		*line;
	t_tree_node	*tree;
}				t_shell;

/*-----------------------minishell------------------------*/
/*--------------------------------------------------------*/
void loop(t_shell *shell);

/*-------------------syntax checking----------------------*/
/*--------------------------------------------------------*/
bool	check_syntax_errors(char *line);
bool	check_quotation_marks(char *line);
bool	check_pipes(char *line);
void	clean_up(t_shell *shell);

/*---------------------close program----------------------*/
/*--------------------------------------------------------*/
void	error_message(char *message);
void	exit_error_message(char *message);
void	exit_error_message(char *message);

/*----------------------tokenization----------------------*/
/*--------------------------------------------------------*/
t_tokens	*tokenize(char *line);

/*-----------------------------utils----------------------*/
/*--------------------------------------------------------*/
char	*ft_strdup_delimiter_char(const char *s, char delimiter);
char	*ft_strdup_delimiter_string(const char *s, char *delimiter);
char	*skip_whitespace(char *line);

#endif
