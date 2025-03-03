#include "../incl/minishell.h"

void	clean_hd_temps(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds && mini->cmds[i])
	{
		if (mini->cmds[i]->heredoc_name)
		{
			unlink(mini->cmds[i]->heredoc_name);
			free_ptr(mini->cmds[i]->heredoc_name);
			mini->cmds[i]->heredoc_name = NULL;
		}
		unlink(TMP_PATH);
		i++;
	}
}

void	check_fd(int fd)
{
	if (fd == -1)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
}

void	trim_limiter(t_token *token)
{
	char	*trimmed;

	trimmed = ft_strtrim(token->content, "\'\"\0");
	free_ptr(token->content);
	token->content = trimmed;
}
