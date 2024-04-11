#include "../../includes/minishell.h"

 volatile sig_atomic_t g_sig = 0;

/* replaces line but doesn't show new prompt */
void	sigint_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();

}

/* replaces line and shows prompt (is called if no prompt printed yet)*/
void	sigint_handler_prompt(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	child_handler(int sig)
{
	// g_sig = sig;
	// ft_putnbr_fd(g_sig, 2);
	kill(getpid(), sig);
}

/* receives signals and calls sigint handler with a certain int sig */
void	signals(int n)
{

	signal(SIGQUIT, SIG_IGN);	//ignore sigquit
	if (n == MAIN)
		signal(SIGINT, sigint_handler);
	else if (n == MAIN_PROMPT)
		signal(SIGINT, sigint_handler_prompt);
	else if (n == CHILD)
	{
		signal(SIGINT, child_handler);
		signal(SIGQUIT, child_handler);
	}
	else if (n == IGN)
		signal(SIGINT, SIG_IGN);
}
