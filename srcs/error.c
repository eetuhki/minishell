#include "../incl/minishell.h"

void err_cmd_is_dir(char *cmd)
{
	if (cmd)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("Is a directory\n", 2);
	}
}

void	err_cmd_not_found(char *cmd)
{
	if (cmd)
	{
		ft_putstr_fd("Command '", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd("' not found\n", 2);
	}
}

int	cd_path_error(char *path)
{
	ft_putstr_fd("mini: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (FAIL);
}
