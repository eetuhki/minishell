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

int	ft_cd(t_mini *mini, char **cmd_args)
{
	char	*home;
	char	*path;

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
	{
		ft_putstr_fd("mini: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (FAIL);
	}
	update_env_vars(mini);
	return (SUCCESS);
}

int	handle_builtin(t_mini *mini, int i)
{
	char	**cmd_arr;

	if (!mini || !mini->cmds_tbl)
		return (FAIL);
	ft_putendl_fd("inside handle_builtin", 2);

	cmd_arr = mini->cmds_tbl[i];
	if (!cmd_arr || !cmd_arr[0])
		return (FAIL);
	ft_putendl_fd("After cmd_arr init", 2);
	ft_putendl_fd(cmd_arr[0], 2);
	ft_putendl_fd(" <--- cmd_arr[0] @handle_builtin", 2);
	if (ft_strncmp(cmd_arr[0], "cd", 3) == 0)
		return (ft_cd(mini, cmd_arr));
	if (ft_strncmp(cmd_arr[0], "pwd", 4) == 0)
		return (ft_pwd(STDOUT));
	if (ft_strncmp(cmd_arr[0], "env", 4) == 0)
		return (ft_env(mini, STDOUT));
	if (ft_strncmp(cmd_arr[0], "export", 7) == 0)
		return (ft_export(mini, cmd_arr));
	if (ft_strncmp(cmd_arr[0], "exit", 5) == 0)
	{
		ft_putendl_fd("do we strcmp ???", 2);
		ft_exit(mini, cmd_arr);
	}
	if (ft_strncmp(cmd_arr[0], "echo", 5) == 0)
		return (ft_echo(cmd_arr));
	return (SUCCESS);
}
