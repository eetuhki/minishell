#include "minishell.h"

void	ft_env(t_mini *mini, int fd)
{
	char	**env;

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

void	ft_cd(t_mini *mini, char *path)
{
	char	*home;

	if (!path || *path == '\0')
	{
		home = env_get_var(mini, "HOME");
		if (!home)
		{
			ft_putstr_fd("mini: cd: HOME not set\n", 2);
			return ;
		}
		path = home;
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("mini: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	update_env_vars(mini);
}

void	handle_builtin(t_mini *mini)
{
	char	**cmd_arr;

	if (!mini || !mini->input || *(mini->input) == '\0')
		return ;
	cmd_arr = ft_split(mini->input, ' ');
	if (!cmd_arr || !cmd_arr[0])
	{
		free_arr(cmd_arr);
		return ;
	}
	if (ft_strncmp(cmd_arr[0], "cd", 3) == 0)
		return (ft_cd(mini, cmd_arr[1]));
	if (ft_strncmp(cmd_arr[0], "pwd", 4) == 0)
		return (ft_pwd(STDOUT));
	if (ft_strncmp(cmd_arr[0], "env", 4) == 0)
		return (ft_env(mini, STDOUT));
	if (ft_strncmp(cmd_arr[0], "export", 7) == 0)
		return (ft_export(mini, cmd_arr[1]));
	if (ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		return (ft_exit(mini, cmd_arr));
}
