#include "minishell.h"

int	ft_env(t_mini *mini, int fd)
{
	char	**env;

	if (!mini || !mini->env)
		return (FAIL);
	env = mini->env;
	while (*env)
	{
		if (ft_strchr(*env, '='))
		{
			ft_putstr_fd(*env++, fd);
			ft_putstr_fd("\n", fd);
		}
		else
			env++;
	}
	return (SUCCESS);
}

int	ft_pwd(int fd)
{
	char	*curr_dir;

	curr_dir = NULL;
	curr_dir = getcwd(curr_dir, 0);
	if (!curr_dir)
		return (FAIL);
	ft_putendl_fd(curr_dir, fd);
	free(curr_dir);
	return (SUCCESS);
}

static int	cd_too_many_args(t_mini *mini)
{
	mini->exit_code = 1;
	ft_putendl_fd("mini: cd: too many arguments", 2);
	return (FAIL);
}

int	ft_cd(t_mini *mini, char **cmd_args)
{
	char	*home;
	char	*path;

	if (cmd_args[1] && cmd_args[2])
		return (cd_too_many_args(mini));
	if (!cmd_args[1] || *cmd_args[1] == '\0')
	{
		home = env_get_var(mini, "HOME");
		if (!home)
		{
			ft_putstr_fd("mini: cd: HOME not set\n", 2);
			return (FAIL);
		}
		path = home;
	}
	else
		path = cmd_args[1];
	if (chdir(path) != 0)
		return (cd_path_error(path));
	update_env_vars(mini);
	return (SUCCESS);
}

int	handle_builtin(t_mini *mini, int i)
{
	char	**cmd_arr;

	if (!mini || !mini->cmds_tbl)
		return (FAIL);
	cmd_arr = mini->cmds_tbl[i];
	if (!cmd_arr || !cmd_arr[0])
		return (FAIL);
	if (ft_strncmp(cmd_arr[0], "cd", 3) == 0)
		return (ft_cd(mini, cmd_arr));
	if (ft_strncmp(cmd_arr[0], "pwd", 4) == 0)
		return (ft_pwd(STDOUT));
	if (ft_strncmp(cmd_arr[0], "env", 4) == 0)
		return (ft_env(mini, STDOUT));
	if (ft_strncmp(cmd_arr[0], "export", 7) == 0)
		return (ft_export(mini, cmd_arr));
	if (ft_strncmp(cmd_arr[0], "exit", 5) == 0)
		return (ft_exit(mini, cmd_arr));
	if (ft_strncmp(cmd_arr[0], "echo", 5) == 0)
		return (ft_echo(cmd_arr));
	return (SUCCESS);
}
