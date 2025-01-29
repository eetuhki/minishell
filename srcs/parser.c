#include "../incl/minishell.h"

// init values for command struct elements
void	init_cmd_elements(t_cmd *cmd)
{
	cmd->cmd_name = NULL;
	cmd->og_str = NULL;
}

// allocates memory and initialises command structs for each command
int	init_cmd_structs(t_mini *mini)
{
	int	i;

	i = 0;
	mini->cmd_count = count_pipes(mini) + 1;
	mini->cmds = ft_calloc(mini->cmd_count, sizeof(t_cmd));
	if (!mini->cmds)
		return (FAIL);
	while (i < mini->cmd_count)
	{
		mini->cmds[i] = malloc(sizeof(t_cmd));
		if (!mini->cmds[i])
		{
			free_ptr(mini);
			return (FAIL);
		}
		init_cmd_elements(mini->cmds[i]);
		i++;
	}
	mini->cmds[mini->cmd_count] = NULL;
	return (SUCCESS);
}

// count the number of pipes within the input prompt (if unquoted)
int	count_pipes(t_mini *mini)
{
	int	i;

	i = -1;
	mini->pipes = 0;
	while (mini->input[++i])
	{
		if (mini->input[i] == '|' && !check_quotes(mini->input, i))
			mini->pipes++;
	}
	return (mini->pipes);
}

// calls functions that init parsing
int	parser(t_mini *mini)
{
	if (init_cmd_structs(mini))
		return (FAIL);
	if (split_cmds(mini))
		return (FAIL);
	return (SUCCESS);
}
