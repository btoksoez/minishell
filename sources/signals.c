#include "../includes/minishell.h"

volatile sig_atomic_t g_sig = 0;

void	sigint_handler(int sig)
{
	g_sig = sig;
	ft_putnbr_fd(sig, 1);
	if (sig == 2)
	{
	// rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
	}
	else if (sig == 3)
	{
		

	}
	return ;
}

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
