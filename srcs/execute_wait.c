#include "../incl/minishell.h"

static void	sig_handler_wait(int sig)
{
	if (sig == SIGINT)
		printf("\n");
}

int	wait_single(t_mini *mini, pid_t pid, int *status)
{
	signal(SIGINT, sig_handler_wait);
	waitpid(pid, status, 0);
	mini->exit_code = WEXITSTATUS(*status);
	sig_init();
	return (SUCCESS);
}

int	wait_multi(t_mini *mini)
{
	int	i;
	int	status;
	int	last_exit;

	i = 0;
	status = 0;
	last_exit = 0;
	signal(SIGINT, sig_handler_wait);
	while (i <= mini->pipes)
	{
		waitpid(mini->pids[i], &status, 0);
		last_exit = WEXITSTATUS(status);
		i++;
	}
	mini->exit_code = last_exit;
	sig_init();
	return (SUCCESS);
}
