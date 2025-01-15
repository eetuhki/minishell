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

void	ft_cd(t_mini *mini, char *path)
{
	char *home;

	if (!path || *path == '\0')
	{
		home = env_get_var(mini, "HOME");
		if (!home)
		{
			ft_putstr_fd("mini: cd: HOME not set\n", 2);
			return;
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

int valid_key(char *cmd_arg)
{
	int	i;
	char *equals_sign;

	i = 0;
	if ((cmd_arg[i] != '_') && !ft_isalpha(cmd_arg[i]))
		return (0);
	i++;
	equals_sign = ft_strchr(cmd_arg, '=');
	while (cmd_arg[i] && (equals_sign == NULL || &cmd_arg[i] < equals_sign))
	{
		if(cmd_arg[i] != '_' && !ft_isalnum(cmd_arg[i]))
			return (0);
		i++;
	}
	return(1);
}

void ft_export(t_mini *mini, char *cmd_arg)
{
	char	*key;
	char	*value;

	if (!cmd_arg || *cmd_arg == '\0')
		printf("env in alphabetical order coming soon...\n");
	else if (!valid_key(cmd_arg))
		printf("mini: export: %s: not a valid identifier\n", cmd_arg);
	else
	{
		key = extract_key(cmd_arg);
		value = extract_value(cmd_arg);
		if (env_find_index(mini, key) == -1)
		{
			if (add_env_pair(mini, key, value) == FAIL)
				printf("mini: export: Failed to export %s\n", key);
		}
		else
		{
			if (env_set_var(mini, key, value) == FAIL)
			{
				ft_putstr_fd("mini: export: Failed to update", 2);
				ft_putstr_fd(key, 2);
				ft_putstr_fd("\n", 2);
			}
		}
	}
}

void	handle_builtin(t_mini *mini)
{
	if (!mini || !mini->input || *(mini->input) == '\0')
		return;

	char **cmd_arr;
	cmd_arr = ft_split(mini->input, ' ');
	if (!cmd_arr || !cmd_arr[0])
	{
		free_arr(cmd_arr);
		return;
	}
	if (ft_strncmp(cmd_arr[0], "cd", 3) == 0)
		return(ft_cd(mini, cmd_arr[1]));
	if (ft_strncmp(cmd_arr[0], "pwd", 4) == 0)
		return(ft_pwd(STDOUT));
	if (ft_strncmp(cmd_arr[0], "env", 4) == 0)
		return(ft_env(mini, STDOUT));
	if (ft_strncmp(cmd_arr[0], "export", 7) == 0)
		return(ft_export(mini, cmd_arr[1]));
	if (ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		return(ft_exit(mini, cmd_arr));
}
