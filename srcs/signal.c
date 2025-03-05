#include "../incl/minishell.h"

static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	sig_handler_heredoc(int sig)
{
	if (sig == SIGINT)
		g_sig = 1;
}

// init signals for the main/parent
void	sig_init(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}

// restore default signal behaviour for child process
void	sig_init_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// init signals for heredoc
void	sig_init_heredoc(void)
{
	signal(SIGINT, sig_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
