#include "../incl/minishell.h"

int	allocate_pids(t_mini *mini)
{
	if (mini->pids)
		free_ptr(mini->pids);
	mini->pids = ft_calloc((size_t)(mini->pipes + 1), sizeof(pid_t));
	if (!mini->pids)
		return (FAIL);
	return (SUCCESS);
}

int	create_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("mini: pipe failed");
		return (FAIL);
	}
	return (SUCCESS);
}

// If not first command, use previous pipe as stdin
// If it's NOT the last command, redirect stdout to pipe
void	pipe_redirect(t_mini *mini, int i)
{
	if (mini->in_fd != STDIN)
	{
		if (dup2(mini->in_fd, STDIN) < 0)
		{
			ft_putstr_fd("mini : dup2 failed\n", 2);
			mini->exit_code = 1;
			free_and_exit(mini);
		}
		close_fd(&mini->in_fd);
	}
	if (i < mini->pipes)
	{
		if (dup2(mini->fd[1], STDOUT) < 0)
		{
			ft_putstr_fd("mini : dup2 failed\n", 2);
			mini->exit_code = 1;
			free_and_exit(mini);
		}
		close_fd(&mini->fd[1]);
	}
}
