#include "minishell.h"

void	print_err_builtin(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("mini: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (arg)
		ft_putstr_fd(arg, 2);
	if (msg)
		ft_putendl_fd(msg, 2);
}

// Checks if the string has a valid env variable key.
// A valid key must start with an underscore or a letter,
// and can only include alphanumeric characters or underscores.
// A valid unset key can not include '=' sign.
int	valid_key(char *cmd_arg, bool is_unset)
{
	int		i;
	char	*equals_sign;

	i = 0;
	if ((cmd_arg[i] != '_') && !ft_isalpha(cmd_arg[i]))
		return (0);
	i++;
	equals_sign = ft_strchr(cmd_arg, '=');
	if (is_unset && ft_strchr(cmd_arg, '='))
		return (0);
	while (cmd_arg[i] && (equals_sign == NULL || &cmd_arg[i] < equals_sign))
	{
		if (cmd_arg[i] != '_' && !ft_isalnum(cmd_arg[i]))
			return (0);
		i++;
	}
	return (1);
}

//updates env variables PWD and OLDPWD after executing the cd command
void	update_env_vars(t_mini *mini)
{
	char	*pwd_var;
	char	*new_pwd;
	char	*oldpwd_var;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	pwd_var = env_get_var(mini, "PWD");
	oldpwd_var = env_get_var(mini, "OLDPWD");
	if (pwd_var && oldpwd_var)
	{
		if (env_set_var(mini, "OLDPWD", pwd_var) == FAIL)
			print_err_builtin("cd: ", NULL, "Failed to update OLDPWD");
	}
	if (pwd_var)
	{
		if (env_set_var(mini, "PWD", new_pwd) == FAIL)
			print_err_builtin("cd: ", NULL, "Failed to update PWD");
	}
	if (new_pwd)
		free_ptr(new_pwd);
}

//returns a copy of the env str_array or NULL incase of error
char	**copy_env(char **env, ssize_t count)
{
	char	**env_copy;
	size_t	i;

	i = 0;
	env_copy = ft_calloc(count + 1, sizeof(char **));
	if (!env_copy)
		return (NULL);
	while (env[i])
	{
		env_copy[i] = ft_strdup(env[i]);
		if (!env_copy[i])
		{
			free_arr(env_copy);
			return (NULL);
		}
		i++;
	}
	return (env_copy);
}

int	search_key_in_env(t_mini *mini, char *key)
{
	int		i;
	char	*current_key;

	i = 0;
	while (mini->env[i])
	{
		current_key = extract_key(mini->env[i]);
		if (!current_key)
			return (-1);
		if (ft_strcmp(key, current_key) == 0)
		{
			free_str(&current_key);
			return (i);
		}
		free_str(&current_key);
		i++;
	}
	return (-1);
}
