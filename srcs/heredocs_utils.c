#include "../incl/minishell.h"

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
