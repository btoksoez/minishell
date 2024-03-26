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

/*-------------------------------structs----------------------------*/
/*------------------------------------------------------------------*/

typedef enum e_token_type
{
	PIPE,		//0 for pipes
	RE_INPUT,	//1 for '<'
	RE_OUTPUT,	//2 for '>'
	APPEND,		//3 for '>>'
	HEREDOC,	//4 for '<<'
	WORD,		//5 for args and commands
	ENV_VAR		//6 for environment variables
}	t_token_type;

typedef struct	s_redir_node
{
	t_token_type		type;
	char				*file;
	char				*delimiter;
	struct s_redir_node	*next;
}						t_redir_node;

typedef struct	s_tree_node
{
	t_token_type		type;
	char				**cmd;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	struct s_redir_node	*redir_list;

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

/*----------------------------minishell-----------------------------*/
/*------------------------------------------------------------------*/
void		loop(t_shell *shell);

/*--------------------------syntax checking-------------------------*/
/*------------------------------------------------------------------*/
bool		check_syntax_errors(char *line);
bool		check_quotation_marks(char *line);
bool		check_pipes(char *line);
void		clean_up(t_shell *shell);

/*--------------------------close program---------------------------*/
/*------------------------------------------------------------------*/
void		error_message(char *message);
void		exit_error_message(char *message);
void		exit_error_message(char *message);

/*----------------------------tokenization--------------------------*/
/*------------------------------------------------------------------*/
t_tokens	*tokenize(char *line);
t_tokens	*get_tokens(char *line);
char		*handle_single_quotes(char *start, t_tokens *token);
char		*handle_double_quotes(char *start, t_tokens **current);
char		*token_word(char *start, t_tokens *token, char *delimiter);
char		*single_token(char *start, t_tokens *token);
char		*double_token(char *start, t_tokens *token);
char		*token_envp(char *start, t_tokens *token);
void		free_tokens(t_tokens *head);
t_tokens	*add_node_back(t_tokens *previous);
t_tokens	*token_init(void);
char		*ft_strdup_delimiter_char(const char *s, char delimiter);
char		*ft_strdup_delimiter_string(const char *s, char *delimiter);
char		*skip_whitespace_and_empty_quotes(char *line);
char		*skip_whitespace(char *line);

/*----------------------------parsing-------------------------------*/
/*------------------------------------------------------------------*/
t_tree_node	*parse_commandline(t_tokens *tokens_start);
t_tree_node	*parse_execution(t_tokens *tokens_start, t_tokens *tokens_end);
t_tree_node	*add_ast_node(t_tokens *tokens);
t_redir_node	*add_redir_list(t_tokens *tokens_start, t_tokens *tokens_end);
t_redir_node	*add_redir_node(t_tokens *current);
void	redir_add_back(t_redir_node **head_redir_list, t_redir_node *new_node);
int	tokens_len(t_tokens *tokens_start, t_tokens *tokens_end);
t_tree_node *parse_cmd(t_tokens *tokens_start, t_tokens *tokens_end);

/*----------------------------testing-------------------------------*/
/*------------------------------------------------------------------*/
void	print_tokens(t_tokens *head);
void print_ast_tree(t_tree_node *root, int level);
void print_redir_list(t_redir_node *head_redir_list, int level);

#endif
