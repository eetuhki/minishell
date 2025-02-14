#include "../incl/minishell.h"

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

int	is_there_type(t_mini *mini, int type)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (mini->cmds[i])
	{
		while (mini->cmds[i]->tokens[j].content)
		{
			if (mini->cmds[i]->tokens[j].type == type)
				return (true);
			j++;
		}
		i++;
	}
	return (false);
}

int	builtin_only(t_mini *mini, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->tokens[i].content)
	{
		if (cmd->tokens[i].type != BUILTIN && cmd->tokens[i].type != ARG
			&& cmd->cmd_found)
			return (false);
		i++;
	}
	return (true);
}

int	cmd_table_size(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds_tbl[i])
		i++;
	return (i);
}
