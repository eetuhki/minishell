#include "minishell.h"

//updates env variables PWD and OLDPWD after executing the cd command
void	update_env_vars(t_mini *mini)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = env_get_var(mini, "PWD");
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	if (old_pwd)
	{
		if (env_set_var(mini, "OLDPWD", old_pwd) == FAIL)
			ft_putstr_fd("mini: cd: Failed to update OLDPWD\n", 2);
		if (env_set_var(mini, "PWD", new_pwd) == FAIL)
			ft_putstr_fd("mini: cd: Failed to update PWD\n", 2);
		free_ptr(new_pwd);
	}
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

int	search_key_in_env(t_mini *mini, char *var)
{
	int		i;
	char	*path;
	char	*key;

	path = ft_strdup(var);
	if (!path)
		return (-1);
	i = 0;
	while (mini->env[i])
	{
		key = extract_key(mini->env[i]);
		if (!key)
			return(-1);
		if (ft_strcmp(path, key) == 0)
		{
			free_str(&path);
			free_str(&key);
			return (i);
		}
		free_str(&key);
		i++;
	}
	free_str(&key);
	free_str(&path);
	return (-1);
}
