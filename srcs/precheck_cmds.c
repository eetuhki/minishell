#include "../incl/minishell.h"

int	check_file_exists(char *cmd, struct stat *path_stat)
{
	if (stat(cmd, path_stat) == -1)
	{
		if (ft_strchr(cmd, '/'))
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
		else
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Command not found\n", 2);
		}
		return (FAIL);
	}
	return (SUCCESS);
}

int	check_directory(char *cmd, struct stat *path_stat)
{
	if (S_ISDIR(path_stat->st_mode))
	{
		if (ft_strchr(cmd, '/'))
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
		}
		else
		{
			ft_putstr_fd("mini: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Command not found\n", 2);
		}
		return (FAIL);
	}
	return (SUCCESS);
}

void check_executable(char *cmd)
{
	if (access(cmd, X_OK) != 0)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
}
void validate_cmd_access(char *cmd)
{
	struct stat path_stat;

	printf("cmd here [%s] \n", cmd);
	if (builtin_only(cmd))
	{
		printf("cmd is BUILTIN\n");
		return ;
	}
	
	if (check_file_exists(cmd, &path_stat) == FAIL)
		return ;
	if (check_directory(cmd, &path_stat) == FAIL)
		return ;
	check_executable(cmd);
}


void	precheck_cmds(t_mini *mini)
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
}
