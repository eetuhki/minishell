#include "../incl/minishell.h"

int	err_exec_malloc(void)
{
	ft_putendl_fd("mini: exec: memory allocation failed\n", 2);
	return (FAIL);
}

void	err_cmd_is_dir(char *cmd)
{
	if (cmd)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("Is a directory\n", 2);
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

void	exec_fail(t_mini *mini, char *cmd)
{
	ft_putstr_fd("mini: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	mini->exit_code = 1;
}

void	print_error(char *cmd, char *msg)
{
	if (dup2(STDERR, STDOUT) < 0)
	{
		ft_putstr_fd("mini: dup2 failed\n", 2);
	}
	printf("mini: %s: %s\n", cmd, msg);
}
