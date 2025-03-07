#include "../incl/minishell.h"

void	handle_fds(t_mini *mini, pid_t pid, int i)
{
	mini->pids[i] = pid;
	close_fd(&mini->fd[1]);
	if (mini->in_fd != STDIN)
		close_fd(&mini->in_fd);
	mini->in_fd = mini->fd[0];
	close_fd(&mini->cmds[i]->in_file);
	close_fd(&mini->cmds[i]->out_file);
}

int	check_pid(pid_t pid, t_mini *mini)
{
	if (pid == -1)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd("fork failed", 2);
		ft_putstr_fd("\n", 2);
		mini->exit_code = 1;
	}
	return (pid);
}

int	builtin_only(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"))
	{
		return (true);
	}
	return (false);
}

int	cmd_table_size(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds_tbl[i])
		i++;
	return (i);
}

void	close_inherited_fds(t_mini *mini, int pipe_i)
{
	int	i;

	i = 0;
	while (mini && mini->cmds[i])
	{
		if (i != pipe_i)
		{
			close_fd(&mini->cmds[i]->in_file);
			close_fd(&mini->cmds[i]->out_file);
		}
		i++;
	}
}
