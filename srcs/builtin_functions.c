#include "minishell.h"

void	ft_env(t_mini *mini, int fd)
{
	char	**env;
	
	env = mini->env;
	while (*env)
	{
		ft_putstr_fd(*env++, fd);
		ft_putstr_fd("\n", fd);
	}
}

void	ft_pwd(int fd)
{
	char	*curr_dir;

	curr_dir = NULL;
	curr_dir = getcwd(curr_dir, 0);
	if (curr_dir != NULL)
	{
		ft_putstr_fd(curr_dir, fd);
		ft_putchar_fd('\n', fd);
		free(curr_dir);
	}
}
