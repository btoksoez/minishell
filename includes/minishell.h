#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <termcap.h>

# define SINGLE_TOKENS "<>|"
# define WHITESPACE " \t\n\v\f\r"
# define WHITESPACE_DOLLAR " \t\n\v\f\r$"
# define WHITESPACE_QUOTES " \t\n\v\f\r\'\""
# define WHITESPACE_Q_D " \t\n\v\f\r\'\"$?<>|"
# define QUOTE_DELIMITER "$\""
# define TRUE 1
# define FALSE 0
# define WRITE_END 1
# define READ_END 0
# define INPUT 0

#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define BLUE "\x1b[34;1m"
#define CYAN "\x1b[36;1m"
#define RESET "\x1b[0m"

/*-------------------------------structs----------------------------*/
/*------------------------------------------------------------------*/

extern volatile sig_atomic_t g_sig;

typedef enum e_tree_type
{
	PHANTOM,			//0 for phantom
	PIPE_TREE,			//1 for pipe
	CMD,				//2 for command
	BUILTIN				//3 for builtins
}	t_tree_type;

typedef enum e_token_type
{
	PIPE,				//0 for pipes
	RE_INPUT,			//1 for '<'
	RE_OUTPUT,			//2 for '>'
	APPEND,				//3 for '>>'
	HEREDOC,			//4 for '<<'
	WORD,				//5 for args and commands
	ENV_VAR				//6 for environment variables
}	t_token_type;

typedef struct	s_args
{
	int					space;
	char				*arg;
	struct s_args		*next;
}						t_args;

typedef struct	s_redir_list
{
	t_token_type		type;
	char				*file;
	struct s_redir_list	*next;
}						t_redir_list;

typedef struct s_tokens
{
	t_token_type		type;
	int					space;
	char				*value;
	struct s_tokens		*previous;
	struct s_tokens		*next;
}						t_tokens;

typedef struct	s_envps
{
	char				*pwd;
	char				*oldpwd;
	char				*home;
	int					pwd_index;
	int					oldpwd_index;
}						t_envps;

typedef struct s_shell
{
	char				**envp;
	t_envps				*envps;
	pid_t				*id;
	int					**fd;
	char				*line;
	int					infile;
	int					outfile;
	bool				here_doc;
	int					pipe_nbr;
	int					status;
	int					builtin_status;
	int					std_fds[2];
	struct s_tree_node	*tree;
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

/*----------------------------minishell-----------------------------*/
/*------------------------------------------------------------------*/
void			init_shell(t_shell *shell, char **envp);
void			copy_envp(t_shell *shell, char **envp);
void			loop(t_shell *shell);
void			wait_pids(int fds, t_shell *shell);
t_envps			*init_envps(void);

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
void			sigint_handler(int sig);

/*--------------------------close program---------------------------*/
/*------------------------------------------------------------------*/
void			error_message(char *message);
void			exit_error_message(char *message, int exit_code);
void			child_error_message(t_shell *shell, char *message, char *command, int exit_code);

/*----------------------------tokenization--------------------------*/
/*------------------------------------------------------------------*/
t_tokens		*tokenize(t_shell *shell);
t_tokens		*get_tokens(char *line);
char			*handle_single_quotes(char *start, t_tokens *token);
char			*handle_double_quotes(char *start, t_tokens *token);
char			*token_word(char *start, t_tokens *token, char *delimiter);
char			*single_token(char *start, t_tokens *token);
char			*token_envp(char *start, t_tokens *token);
void			free_tokens(t_tokens *head);
t_tokens		*add_node_back(t_tokens *previous);
t_tokens		*token_init(void);
char			*ft_strdup_delimiter_char(const char *s, char delimiter);
char			*ft_strdup_delimiter_string(const char *s, char *delimiter);
char			*skip_whitespace_and_empty_quotes(char *line);
char			*skip_whitespace(char *line);
int				args_len(t_args *args);
char			*token_dollar(char *start, t_tokens *token);
void			del_token(t_tokens **head, t_tokens *node);

/*----------------------------parsing-------------------------------*/
/*------------------------------------------------------------------*/
t_tree_node		*parse_commandline(t_tokens *tokens_start);
t_tree_node		*add_ast_node(void);
void			add_redir_list(t_redir_list **head, t_token_type type, char *filename);
void			add_arg(t_args **args, t_tokens *current);
int				tokens_len(t_tokens *tokens_start, t_tokens *tokens_end);
t_tree_node		*parse_cmd(t_tokens *tokens_start, t_tokens *tokens_end);
void			count_pipes(t_shell *shell);

/*----------------------------expansion-----------------------------*/
/*------------------------------------------------------------------*/
void			expand(t_tokens *tokens);
char			*get_env(char *str);
char			*find_env(char *str);
char			*ft_strdup_until(char *s);
void			free_strs(char *s1, char *s2, char *s3);
/*------------------------------reset-------------------------------*/
/*------------------------------------------------------------------*/
void			reset(t_shell *shell);
void			close_all_fds(t_shell *shell, bool in_out);

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
char			*extend_path(char *path);
int				is_valid_path(char *path);
int				char_arr_len(char **arr);

/*---------------------------execution------------------------------*/
/*------------------------------------------------------------------*/
void			execute(t_shell *shell);
char			*get_path(char *cmd, char **envp);
void			get_path_index(char **envp, int *index);
void			free_and_close_path(int fd, char **paths, char *path, char *path_cmd);
void			prepare_to_execute(t_shell *shell);

/*----------------------------freeing-------------------------------*/
/*------------------------------------------------------------------*/
void			free_all(t_shell *shell);
void			free_tree(t_tree_node *node);
void			free_tokens(t_tokens *head);
void			free_args(t_args *args);
void			free_redir_list(t_redir_list *redir_list);

/*----------------------------testing-------------------------------*/
/*------------------------------------------------------------------*/
void			print_tokens(t_tokens *head);
void			print_spaces(int count);
void			print_tree(t_tree_node *root, int level);
void			print_envp(t_shell *shell);
void			print_path_test(void);
void			print_test_cd(t_shell *s);

#endif
