#include "../incl/minishell.h"

static bool	is_heredoc_last(t_cmd *cmd)
{
	int		i;
	bool	is_heredoc_last;

	i = 0;
	is_heredoc_last = false;
	while (cmd && cmd->tokens && cmd->tokens[i].content)
	{
		if (cmd->tokens[i].type == INFILE)
		{
			is_heredoc_last = false;
		}
		else if (cmd->tokens[i].type == LIMITER)
		{
			is_heredoc_last = true;
		}
		i++;
	}
	return (is_heredoc_last);
}

int	setup_heredocs_redir(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini && mini->cmds[i] != NULL)
	{
		if (is_heredoc_last(mini->cmds[i]) && mini->cmds[i]->in_file != -2)
		{
			if (mini->cmds[i]->in_file != -1)
				close_fd(&mini->cmds[i]->in_file);
			mini->cmds[i]->in_file = open(mini->cmds[i]->heredoc_name,
					O_RDONLY);
			if (mini->cmds[i]->in_file == -1)
			{
				err_opening_file(mini->cmds[i]->heredoc_name, errno);
				return (FAIL);
			}
		}
		i++;
	}
	return (SUCCESS);
}
