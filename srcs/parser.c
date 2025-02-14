#include "../incl/minishell.h"

/*
int	parse_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds[i])
	{
		if (parse_individual_cmd(mini, mini->cmds[i]) == FAIL)
		{
			mini->exit_code = 1;
			return (FAIL);
		}
		expand_variables(mini, mini->cmds[i]->og_str, &mini->cmds[i]->expd_str);
		if (mini->cmds[i]->expd_str != NULL && mini->cmds[i]->expd_str[0] != '\0')
			printf("The expanded var = %s\n", mini->cmds[i]->expd_str);
		i++;
	}
	return (SUCCESS);
}*/

// init values for command struct elements
void	init_cmd_elements(t_cmd *cmd)
{
	cmd->cmd_name = NULL;
	cmd->heredoc_name = NULL;
	cmd->heredoc_index = 0;
	cmd->hd_fd = -1;
	cmd->og_str = NULL;
	cmd->cmd_num = 0;
	cmd->i = 0;
	cmd->token_count = 0;
	cmd->cmd_found = false;
	cmd->in_file = -1;
	cmd->out_file = -1;
}

// allocates memory and initialises command structs for each command
int	init_cmd_structs(t_mini *mini)
{
	int	i;
	int	cmd_count;

	i = 0;
	cmd_count = count_pipes(mini) + 1;
	mini->cmds = ft_calloc(cmd_count + 1, sizeof(t_cmd));
	if (!mini->cmds)
		return (FAIL);
	while (i < cmd_count)
	{
		mini->cmds[i] = malloc(sizeof(t_cmd));
		if (!mini->cmds[i])
			return (free_ptr_fail(mini));
		init_cmd_elements(mini->cmds[i]);
		i++;
	}
	mini->cmds[cmd_count] = NULL;
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
	if (parse_cmds(mini))
		return (FAIL);
	if (handle_heredocs(mini))
		return (FAIL);
	if (process_cmd_files(mini))
		return(FAIL);
	return (SUCCESS);
}
