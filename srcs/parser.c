#include "../incl/minishell.h"

// count the number of pipes within the input prompt (if unquoted)
static int	count_pipes(t_mini *mini)
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

// init values for command struct elements
static void	init_cmd_elements(t_cmd *cmd)
{
	cmd->cmd_name = NULL;
	cmd->heredoc_name = NULL;
	cmd->hd_fd = -1;
	cmd->og_str = NULL;
	cmd->cmd_num = 0;
	cmd->i = 0;
	cmd->token_count = 0;
	cmd->cmd_found = false;
	cmd->in_file = -1;
	cmd->out_file = -1;
	cmd->eof_exit = false;
}

// allocates memory and initialises command structs for each command
static int	init_cmd_structs(t_mini *mini)
{
	int	i;
	int	cmd_count;

	i = 0;
	cmd_count = count_pipes(mini) + 1;
	if (mini->cmds)
		free_cmds(mini);
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
	process_cmd_files(mini);
	if (setup_heredocs_redir(mini))
		return (FAIL);
	return (SUCCESS);
}
