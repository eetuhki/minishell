#include "../incl/minishell.h"

int	wait_single(t_mini *mini, pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	mini->exit_code = WEXITSTATUS(*status);
	return (SUCCESS);
}

int	wait_multi(t_mini *mini)
{
	int	i;
	int	status;

	i = 0;
	while (i < mini->pipes)
	{
		waitpid(mini->pids[i], &status, 0);
		mini->exit_code = WEXITSTATUS(status);
		i++;
	}
	return (SUCCESS);
}
