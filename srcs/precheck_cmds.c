#include "../incl/minishell.h"

int	check_executable(char *cmd)
{
	if (access(cmd, X_OK) != 0)
	{
		ft_putstr_fd("@check_executable\n", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(" \n", 2);
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (FAIL);
	}
	return (SUCCESS);
}

int	check_file_exists(char *cmd, struct stat *path_stat, int *exit_code)
{
	if (stat(cmd, path_stat) == -1)
	{
		ft_putstr_fd("CMD INSIDE STAT -1\n", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd("  \n", 2);
		if (!ft_strchr(cmd, '/') && (access(cmd, F_OK) != 0))
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Command not found\n", 2);
			*exit_code = 127;
		}
		else
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			if (errno == EACCES)
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
	if (S_ISDIR(path_stat->st_mode))
	{
		if (ft_strchr(cmd, '/'))
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			*exit_code = 126;
			return (FAIL);
		}
	}
	return (SUCCESS);
}

int	validate_cmd_access(char *cmd)
{
	struct stat path_stat;
	int			exit_code;

	exit_code = 0;
	printf("cmd here [%s] \n", cmd);
	if (builtin_only(cmd))
	{
		printf("cmd is BUILTIN\n");
		return (0);
	}

	if (check_file_exists(cmd, &path_stat, &exit_code) == FAIL)
		return (exit_code);
	if (check_directory(cmd, &path_stat, &exit_code) == FAIL)
		return (exit_code);
	if (!ft_strchr(cmd, '/'))
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Command not found\n", 2);
		exit_code = 127;
		return (exit_code);
	}
	if (check_executable(cmd) == FAIL)
		return (126);
	return (0);
}

void	precheck_cmds(t_mini *mini, char *cmd)
{
	//int i;
	//int	j;
	int	exit_code;

	//i = 0;
	//while (mini && mini->cmds_tbl[i])
	//{
		//j = 0;
		//if (mini->cmds_tbl[i][j] && !input_is_whitespace(mini->cmds_tbl[i][j]))
		//{
		exit_code = validate_cmd_access(cmd);
		if (exit_code != 0)
		{
			mini->exit_code = exit_code;
			ft_putstr_fd("PRECHECK exit\n", 2);
			ft_putnbr_fd(mini->exit_code, 2);
			ft_putstr_fd("\n", 2);
			exit(mini->exit_code);
		}

		//}
		//j++;
		//i++;
	//}
}

/* void	precheck_cmds(t_mini *mini)
{
	int i;
	int	j;

	i = 0;
	while (mini && mini->cmds_tbl[i])
	{
		j = 0;
		if (mini->cmds_tbl[i][j] && !input_is_whitespace(mini->cmds_tbl[i][j]))
		{
			validate_cmd_access(mini->cmds_tbl[i][j]);
		}
		j++;
		i++;
	}
} */

