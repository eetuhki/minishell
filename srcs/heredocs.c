#include "../incl/minishell.h"

int	handle_single_heredoc(t_token *token)
{
	int	i;

	i = 0;
	while (token->content[i])
	{
		if (token->content[i])
			return (FAIL);
	}
	return (SUCCESS);
}

int	handle_heredocs(t_mini *mini)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (mini->cmds[i])
	{
		while (mini->cmds[i]->tokens[j].content)
		{
			if (mini->cmds[i]->tokens[j].type == HEREDOC
				&& handle_single_heredoc(&mini->cmds[i]->tokens[j]) == FAIL)
				return (FAIL);
			j++;
		}
		i++;
	}
	return (SUCCESS);
}

