#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>	// readline, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>	// rl_clear_history, add_history
# include <stdio.h>				// printf, perror
# include <stdlib.h>			// malloc, free, exit, getenv
# include <unistd.h>			// write, access, read, close, fork, getcwd, chdir, unlink, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <fcntl.h>				// open
# include <sys/wait.h>			// wait, waitpid, wait3, wait4
# include <signal.h>  			// signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/stat.h>			// stat, lstat, fstat
# include <dirent.h>  			// opendir, readdir, closedir
# include <string.h>			// strerror
# include <sys/ioctl.h>			// ioctl
# include <termios.h>			// tcsetattr, tcgetattr
# include <termcap.h>			// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs


/* structs */

typedef struct s_shell
{
	char	*line;
}	t_shell;


/* functions */
void loop(t_shell *shell);

/* error checking*/
int		check_errors(char *line);
int		check_quotation_marks(char *line);
int		check_pipes(char *line);
void	error_message(char *message);


#endif
