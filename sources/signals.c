#include "../includes/minishell.h"

/*
when not in heredoc:
[works] ctrl + c: display new prompt (has to work always, even if line not empty)
[works] ctrl + d: exit shell gracefully (cleaning + freeing) (only when line empty)
[works] ctrl + /: nothing

when in heredoc:
[should work] ctrl + c: stop heredoc without saving (display new prompt, has to work also when line not empty)
[need to implement] ctrl + d: same as EOF (only when pressed on empty line); however we implement EOF, we just need to check for g_sig == 3 too
[works] ctrl + /: nothing
*/

/* global variable to store signal;
volatile = value may change unexpectedly, so compiler should not optimize its accesses
sig_atomic_t: special type to guarantee access to it
extern: declared in .h file, but defined here */
volatile sig_atomic_t g_sig = 0;

/* sets g_sig and depending on sig code, makes new prompt*/
void	sigint_handler(int sig)
{
	g_sig = sig;
	// ft_putnbr_fd(sig, 1);
	if (sig == SIGINT)
	{
		// rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

/* receives signals and calls sigint handler with a certain int sig */
void	signals(void)
{
	struct sigaction	sa;
	struct sigaction	sb;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sb.sa_handler = sigint_handler;
	sb.sa_flags = 0;
	sigemptyset(&sb.sa_mask);
	sigaction(SIGQUIT, &sb, NULL);
}

// void	handle_ctrl_c(int a)
// {
// 	(void)a;
// 	g_sig = 1;
// 	// rl_replace_line("", 0);
// 	write(1, "\n", 1);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

// void	signals(void)
// {
// 	signal(SIGINT, handle_ctrl_c);
// 	signal(SIGQUIT, SIG_IGN);
// }
