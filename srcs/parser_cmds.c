#include "../incl/minishell.h"

int	tokenize_cmd_string(t_mini *mini, t_cmd *cmd)
{
	int	i;

	i = -1;
	if (!cmd || !cmd->og_str)
		return (FAIL);
	if (init_tokens(cmd))
		return (FAIL);
	if (split_tokens(cmd))
		return (FAIL);
	while (++i < cmd->token_count)
	{
		if (cmd->tokens[i].content && !cmd->tokens[i].type)
		{
			tokenize_redir(&cmd->tokens[i]);
			if (i == 0 && !cmd->tokens[i].type)
			{
				check_for_builtins(&cmd->tokens[i]);
				tokenize_cmd(mini, &cmd->tokens[i]);
			}
			else if (!cmd->tokens[i].type)
				cmd->tokens[i].type = ARG;
		}
	}
	return (SUCCESS);
}

int	parse_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds[i])
	{
		if (tokenize_cmd_string(mini, mini->cmds[i]) == FAIL)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}
