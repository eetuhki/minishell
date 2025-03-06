#include "../incl/minishell.h"

static char	*get_filename(t_cmd *cmd)
{
	char	*filename;

	if (cmd->heredoc_name)
	{
		free_ptr(cmd->heredoc_name);
		cmd->heredoc_name = NULL;
	}
	filename = ft_strdup(TMP_PATH);
	unlink(filename);
	return (filename);
}

static int	process_heredoc(t_mini *mini, t_cmd *cmd, t_token *token)
{
	char	*line;

	sig_init_heredoc();
	if (g_sig)
		return (FAIL);
	line = readline("> ");
	if (!line)
	{
		cmd->eof_exit = 1;
		return (FAIL);
	}
	add_history(line);
	if (!ft_strcmp(line, token->content))
	{
		free_ptr(line);
		return (FAIL);
	}
	if (mini->heredoc_expand == true)
		expand_variables(mini, token, &line);
	ft_putstr_fd(line, cmd->hd_fd);
	ft_putstr_fd("\n", cmd->hd_fd);
	free_ptr(line);
	return (SUCCESS);
}

static int	get_heredoc(t_mini *mini, t_cmd *cmd, t_token *token)
{
	cmd->hd_fd = open(cmd->heredoc_name, O_RDWR | O_EXCL | O_CREAT, 0600);
	check_fd(cmd->hd_fd);
	rl_event_hook = heredoc_sigint_hook;
	while (1)
	{
		if (process_heredoc(mini, cmd, token) == FAIL)
			break ;
		if (g_sig)
			break ;
	}
	rl_event_hook = NULL;
	close_fd(&cmd->hd_fd);
	cmd->hd_fd = -1;
	if (g_sig)
		return (handle_early_exit(mini, cmd));
	g_sig = 0;
	if (cmd->eof_exit)
	{
		ft_putstr_fd("mini: warning: here-document delimited by ", 2);
		ft_putstr_fd("by end-of-file (wanted `", 2);
		ft_putstr_fd(token->content, 2);
		ft_putendl_fd("')", 2);
	}
	return (SUCCESS);
}

static int	handle_single_heredoc(t_mini *mini, t_cmd *cmd, t_token *token)
{
	int		i;

	i = 0;
	while (token->content[i])
	{
		if (token->content[i] == SQUOTE || token->content[i] == DQUOTE)
			mini->heredoc_expand = false;
		i++;
	}
	trim_limiter(token);
	cmd->heredoc_name = get_filename(cmd);
	if (get_heredoc(mini, cmd, token) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

int	handle_heredocs(t_mini *mini)
{
	int	i;
	int	j;

	i = 0;
	while (mini->cmds[i])
	{
		j = 0;
		while (j < mini->cmds[i]->token_count)
		{
			if (mini->cmds[i]->tokens[j].type == LIMITER)
			{
				if (handle_single_heredoc(mini, mini->cmds[i],
						&mini->cmds[i]->tokens[j]) == FAIL)
					return (FAIL);
			}
			j++;
		}
		i++;
	}
	return (SUCCESS);
}
