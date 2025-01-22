#include "../incl/minishell.h"

// Extracts the key part from a "key=value" formatted string.
// Returns an empty string if no characters exist before '='.
char	*extract_key(char *cmd_arg)
{
	size_t	i;
	char	*key;

	i = 0;
	while (cmd_arg && cmd_arg[i])
	{
		if (cmd_arg[i] == '=')
			break ;
		i++;
	}
	key = ft_substr(cmd_arg, 0, i);
	return (key);
}

// Extracts the value part from a "key=value" formatted string.
// If '=' is not found, returns NULL.
char	*extract_value(char *cmd_arg)
{
	size_t	i;
	char	*value;

	i = 0;
	if (!ft_strchr(cmd_arg, '='))
		return (NULL);
	while (cmd_arg && cmd_arg[i])
	{
		if (cmd_arg[i] == '=')
			break ;
		i++;
	}
	if (cmd_arg[i + 1] == '\0')
		return (ft_strdup(""));
	value = ft_substr(cmd_arg, i + 1, (ft_strlen(cmd_arg) - i));
	return (value);
}

// Creates a new env variable pair in the format "key=value" or "key".
// Returns a newly allocated string containing "key=value" or just "key".
// Returns NULL if memory allocation fails.
static char	*create_new_pair(char *key, char *value, bool has_value)
{
	char	*temp;
	char	*new_pair;

	new_pair = NULL;
	if (has_value)
	{
		temp = ft_strjoin(key, "=");
		if (!temp)
			return (NULL);
	}
	else
		temp = key;
	if (value != NULL)
		new_pair = ft_strjoin(temp, value);
	else
		new_pair = ft_strdup(temp);
	if (has_value)
		free_ptr(temp);
	return (new_pair);
}

// Adds a new env variable pair to the env array.
// Reallocates memory for a new env array, including space for the new pair.
// Creates the new key-value pair and appends it to the array.
// Returns SUCCESS, or FAIL if memory allocation fails.
int	add_env_pair(t_mini *mini, char *key, char *value, bool has_value)
{
	size_t	count;
	char	*new_pair;
	char	**new_env;

	count = env_count_variables(mini->env);
	new_env = realloc(mini->env, (count + 2) * sizeof(char **));
	if (!new_env)
		return (FAIL);
	new_pair = create_new_pair(key, value, has_value);
	if (!new_pair)
		return (FAIL);
	new_env[count] = new_pair;
	new_env[count + 1] = NULL;
	mini->env = new_env;
	return (SUCCESS);
}

// Sorts the env variable array in alphabetical order using bubble sort.
void	sort_env(char **env_copy, ssize_t size)
{
	ssize_t	i;
	ssize_t	j;
	char	*temp;
	int		swapped;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		swapped = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				temp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}
