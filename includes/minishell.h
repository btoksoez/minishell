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
# define TRUE 1
# define FALSE 0

/*-------------------------------structs----------------------------*/
/*------------------------------------------------------------------*/

typedef struct	s_tree_node t_tree_node;

typedef enum e_tree_type
{
	PHANTOM,	//0 for phantom
	PIPE_TREE,	//1 for pipe
	CMD,		//2 for command
	BUILTIN		//3 for builtins
}	t_tree_type;

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

typedef struct	s_args
{
	char				*arg;
	struct s_args		*next;
}						t_args;

typedef struct	s_redir_list
{
	t_token_type		type;
	char				*file;	//removed delimiter, if type == HEREDOC, then file is delimiter
	struct s_redir_list	*next;
}						t_redir_list;

typedef struct s_tokens
{
	t_token_type		type;
	char				*value;
	struct s_tokens		*previous;
	struct s_tokens		*next;
}						t_tokens;

typedef struct s_shell
{
	char				**envp;
	pid_t				*id;
	int					**fd;
	char				*line;
	int					pipe_nbr;
	int					status;
	int					env_status;
	t_tree_node			*tree;
	t_tokens			*tokens;
}						t_shell;

typedef struct	s_tree_node
{
	t_tree_type			type;
	char				*cmd;
	int					(*builtin)(t_shell *shell, struct s_tree_node *cmd_node);	//stores builtin function pointer
	struct s_args		*args;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	struct s_redir_list	*redir_list;
}						t_tree_node;

extern volatile sig_atomic_t g_sig;

/*----------------------------minishell-----------------------------*/
/*------------------------------------------------------------------*/
void			init_shell(t_shell *shell, char **envp);
void			copy_envp(t_shell *shell, char **envp);
void			loop(t_shell *shell);

/*--------------------------syntax checking-------------------------*/
/*------------------------------------------------------------------*/
bool			check_syntax_errors(char *line);
bool			check_quotation_marks(char *line);
bool			check_pipes(char *line);
void			clean_up(t_shell *shell);
bool			check_tokens(t_tokens *tokens);

/*--------------------------syntax checking-------------------------*/
/*------------------------------------------------------------------*/
void			signals(void);

/*--------------------------close program---------------------------*/
/*------------------------------------------------------------------*/
void			error_message(char *message);
void			exit_error_message(char *message);
void			exit_error_message(char *message);

/*----------------------------tokenization--------------------------*/
/*------------------------------------------------------------------*/
t_tokens		*tokenize(t_shell *shell);
t_tokens		*get_tokens(char *line);
char			*handle_single_quotes(char *start, t_tokens *token);
char			*handle_double_quotes(char *start, t_tokens **current);
char			*token_word(char *start, t_tokens *token, char *delimiter);
char			*single_token(char *start, t_tokens *token);
char			*double_token(char *start, t_tokens *token);
char			*token_envp(char *start, t_tokens *token);
void			free_tokens(t_tokens *head);
t_tokens		*add_node_back(t_tokens *previous);
t_tokens		*token_init(void);
char			*ft_strdup_delimiter_char(const char *s, char delimiter);
char			*ft_strdup_delimiter_string(const char *s, char *delimiter);
char			*skip_whitespace_and_empty_quotes(char *line);
char			*skip_whitespace(char *line);

/*----------------------------parsing-------------------------------*/
/*------------------------------------------------------------------*/
t_tree_node		*parse_commandline(t_tokens *tokens_start);
t_tree_node		*add_ast_node(void);
void			add_redir_list(t_redir_list **head, t_token_type type, char *filename);
void			add_arg(t_args **args, t_tokens *current);
int				tokens_len(t_tokens *tokens_start, t_tokens *tokens_end);
t_tree_node		*parse_cmd(t_tokens *tokens_start, t_tokens *tokens_end);

/*----------------------------expansion-----------------------------*/
/*------------------------------------------------------------------*/
void			expand(t_tokens *tokens);

/*----------------------------builtins------------------------------*/
/*------------------------------------------------------------------*/
int				(*builtin_arr(char *str))(t_shell *shell, struct s_tree_node *cmd_node);

int				mini_cd(t_shell *shell, t_tree_node *tree);
int				mini_echo(t_shell *shell, t_tree_node *cmd_node);
int				mini_export(t_shell *shell, t_tree_node *tree);
int				mini_env(t_shell *shell, t_tree_node *tree);
int				mini_exit(t_shell *shell, t_tree_node *tree);
int				mini_pwd(t_shell *shell, t_tree_node *tree);
int				mini_unset(t_shell *shell, t_tree_node *tree);

/*---------------------------execution------------------------------*/
/*------------------------------------------------------------------*/
void			execute(t_shell *shell);
char			*get_path(char *cmd, char **envp);
void			get_path_index(char **envp, int *index);
void			free_and_close_path(int fd, char **paths, char *path, char *path_cmd);

/*----------------------------testing-------------------------------*/
/*------------------------------------------------------------------*/
void			print_tokens(t_tokens *head);
void			print_spaces(int count);
void			print_tree(t_tree_node *root, int level);


#endif
