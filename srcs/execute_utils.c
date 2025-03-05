#include "../incl/minishell.h"

void	handle_fds(t_mini *mini, pid_t pid, int i)
{
	mini->pids[i] = pid;
	close_fd(&mini->fd[1]); // Parent closes write end immediately
	if (mini->in_fd != STDIN) // Close the old read end
		close_fd(&mini->in_fd);
	mini->in_fd = mini->fd[0]; // Store the read end for the next iteration
	close_fd(&mini->cmds[i]->in_file);
	close_fd(&mini->cmds[i]->out_file);
}

int	check_pid(pid_t pid)
{
	if (pid == -1)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd("fork failed", 2);
		ft_putstr_fd("\n", 2);
	}
	return (pid);
}

int	is_there_type(t_mini *mini, t_type type, int i)
{
	int	j;

	if (mini->cmds[i])
	{
		j = 0;
		while (mini->cmds[i]->tokens[j].content)
		{
			if (mini->cmds[i]->tokens[j].type == type)
				return (true);
			j++;
		}
	}
	return (false);
}

int	builtin_only(char *cmd)
{
	if (!cmd)
		return (false);
	//$VAR after expansion is cat but type = arg
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"))
	{
		return (true);
	}
	return (false);
 	/* if (cmd->tokens[0].content)
	{
		printf("cmd->tokens[i].content")
		if (cmd->tokens[i].type != BUILTIN && cmd->tokens[i].type != ARG
			&& cmd->cmd_found)
			return (false);
		i++;
	}
	return (true); */
}

int	cmd_table_size(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds_tbl[i])
		i++;
	return (i);
}
