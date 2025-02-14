#include "../incl/minishell.h"

char	*get_filename(t_cmd *cmd)
{
	char	*suffix;
	char	*tmp;
	char	*filename;

	if (cmd->heredoc_name)
	{
		unlink(cmd->heredoc_name);
		return (cmd->heredoc_name);
	}
	suffix = ft_itoa(cmd->heredoc_index);
	tmp = ft_strjoin(TMP_PATH, suffix);
	free_ptr(suffix);
	filename = ft_strjoin(tmp, TMP_EXT);
	free_ptr(tmp);
	unlink(filename);
	return (filename);
}

int	get_heredoc(t_mini *mini, t_cmd *cmd, t_token *token)
{
	char	*line;

	cmd->hd_fd = open(cmd->heredoc_name, O_RDWR | O_EXCL | O_CREAT, 0600);
	check_fd(cmd->hd_fd);
	while (1)
	{
		write(0, "> ", 2);
		line = readline(STDIN);
		if (!line || !ft_strcmp(line, token->content))
		{
			free_ptr(line);
			break ;
		}
		if (mini->heredoc_expand == true)
			expand_variables(mini, &line);
		ft_putstr_fd(line, cmd->hd_fd);
		free_ptr(line);
	}
	mini->heredoc_expand = false;
	return (SUCCESS);
}

int	handle_single_heredoc(t_mini *mini, t_cmd *cmd, t_token *token)
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
	j = 0;
	while (mini->cmds[i])
	{
		while (mini->cmds[i]->tokens[j].content)
		{
			if (mini->cmds[i]->tokens[j].type == LIMITER)
			{
				mini->cmds[i]->heredoc_index++;
				if (handle_single_heredoc(mini, mini->cmds[i], &mini->cmds[i]->tokens[j]) == FAIL)
					return (FAIL);
			}
			j++;
		}
		i++;
	}
	return (SUCCESS);
}
