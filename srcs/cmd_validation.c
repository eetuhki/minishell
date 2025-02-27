#include "../incl/minishell.h"

void print_error(char *cmd, char *msg)
{
    ft_putstr_fd("mini: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(msg, 2);
}

int	check_executable(char *cmd)
{
	if (access(cmd, X_OK) != 0)
	{
		print_error(cmd, "Permission denied");
		return (FAIL);
	}
	return (SUCCESS);
}

int	check_file_exists(char *cmd, struct stat *path_stat, int *exit_code)
{
	*exit_code = 0;
	if (stat(cmd, path_stat) == -1)
	{
		if (!ft_strchr(cmd, '/') && (access(cmd, F_OK) != 0))
		{
			print_error(cmd, "command not found");
			*exit_code = 127;
		}
		else
		{
			print_error(cmd, strerror(errno));
			if (errno == EACCES || errno == ENOTDIR)
				*exit_code = 126;
			else
				*exit_code = 127;
		}
		return (FAIL);
	}
	return (SUCCESS);
}

int	check_directory(char *cmd, struct stat *path_stat, int *exit_code)
{
	*exit_code = 0;
	if (S_ISDIR(path_stat->st_mode))
	{
		if (ft_strchr(cmd, '/'))
		{
			print_error(cmd, "Is a directory");
			*exit_code = 126;
		}
		else
		{
			print_error(cmd, "command not found");
			*exit_code = 127;
		}
		return (FAIL);
	}
	return (SUCCESS);
}

int	check_cmd_validity(char *cmd)
{
	struct stat path_stat;
	int			exit_code;

	exit_code = 0;
	if (builtin_only(cmd))
		return (0);
	if (check_file_exists(cmd, &path_stat, &exit_code) == FAIL)
		return (exit_code);
	if (check_directory(cmd, &path_stat, &exit_code) == FAIL)
		return (exit_code);
	if (!ft_strchr(cmd, '/'))
	{
		print_error(cmd, "command not found");
		return (127);
	}
	if (check_executable(cmd) == FAIL)
		return (126);
	return (0);
}

void	validate_cmd_access(t_mini *mini, char *cmd)
{
	int	exit_code;

	if (cmd && ft_strcmp(cmd, ".") == 0)
	{
		print_error(".", "filename argument required");
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		mini->exit_code = 2;
		free_and_exit(mini);
	}
	exit_code = check_cmd_validity(cmd);
	if (exit_code != 0)
	{
		mini->exit_code = exit_code;
		free_and_exit(mini);
	}
}
