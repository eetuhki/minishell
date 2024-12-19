#include "../incl/minishell.h"

// returns the values for a given env variable, returns NULL if fail
char	*env_get_var(t_mini *mini, char *var)
{
	int		i;
	char	*path;

	if (!mini->env || !var)
		return (NULL);
	path = ft_strjoin(var, "=");
	if (!path)
		return (NULL);
	i = 0;
	while (mini->env[i])
	{
		if (!(ft_strncmp(path, mini->env[i], ft_strlen(path))))
		{
			free_ptr(path);
			return (ft_strchr(mini->env[i], '=') + 1);
		}
		i++;
	}
	free_ptr(path);
	return (NULL);
}

// returns the index/location of given env key, return index or -1 if fail
int	env_find_index(t_mini *mini, char *var)
{
	int		i;
	char	*path;

	path = ft_strjoin(var, "=");
	if (!path)
		return (-1);
	i = 0;
	while (mini->env[i])
	{
		if (!(ft_strncmp(path, mini->env[i], ft_strlen(path))))
		{
			free_ptr(path);
			return (i);
		}
		i++;
	}
	free_ptr(path);
	return (-1);
}

//reallocate memory for new size env, return new **env or NULL if fail
char	**env_realloc(t_mini *mini, int size)
{
	int		i;
	char	**new_env;

	new_env = ft_calloc(size + 1, sizeof * new_env);
	if (!new_env)
		return (NULL);
	i = 0;
	while (mini->env[i] && i < size)
	{
		new_env[i] = ft_strdup(mini->env[i]);
		free_ptr(mini->env[i]);
		i++;
	}
	free_arr(mini->env);
	return (new_env);
}

// modifies an env variable, return: SUCCESS or FAIL
int	env_set_var(t_mini *mini, char *var, char *new_val)
{
	int		i;
	char	*path;

	if (!new_val)
		new_val = "";
	i = env_find_index(mini, var);
	path = ft_strjoin("=", new_val);
	if (!path)
		return (FAIL);
	if (i != -1 && mini->env[i])
	{
		free_ptr(mini->env[i]);
		mini->env[i] = ft_strjoin(var, path);
	}
	else
	{
		i = env_count_variables(mini->env);
		mini->env = env_realloc(mini, (i + 1));
		if (!mini->env)
			return (FAIL);
		mini->env[i] = ft_strjoin(var, path);
	}
	free_ptr(path);
	return (SUCCESS);
}
