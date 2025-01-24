#include "../incl/minishell.h"

void	sig_handler_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_init(void)
{
	signal(SIGINT, sig_handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}
