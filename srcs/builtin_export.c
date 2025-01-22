#include "../incl/minishell.h"

// Checks if the string has a valid env variable key.
// A valid key must start with an underscore or a letter,
// and can only include alphanumeric characters or underscores.
static int	valid_key(char *cmd_arg)
{
	int		i;
	char	*equals_sign;

	i = 0;
	if ((cmd_arg[i] != '_') && !ft_isalpha(cmd_arg[i]))
		return (0);
	i++;
	equals_sign = ft_strchr(cmd_arg, '=');
	while (cmd_arg[i] && (equals_sign == NULL || &cmd_arg[i] < equals_sign))
	{
		if (cmd_arg[i] != '_' && !ft_isalnum(cmd_arg[i]))
			return (0);
		i++;
	}
	return (1);
}

// Prints an env't variable in the format => declare -x key="value"
// If the env var has a value (indicated by '='), it's value will be printed in double quotes.
// if not only the key is printed.
static void	print_variable(char *env_var)
{
	int	j;

	j = 0;
	printf("declare -x ");
	while (env_var[j] && env_var[j] != '=')
		printf("%c", env_var[j++]);
	if (env_var[j] == '=')
	{
		printf("=\"");
		j++;
		while (env_var[j] != '\0')
			printf("%c", env_var[j++]);
		printf("\"");
	}
	printf("\n");
}

// Creates a sorted copy of the env't variables and prints them.
// Copies the env't variables into a new array.
// Sorts the copied env't variables alphabetically.
// Iterates through the sorted array, skipping the special "_=" variable.
// Prints each env variable using the print_variable function.
static	void	print_export(t_mini *mini)
{
	int		i;
	size_t	count;
	char	**env_copy;

	count = env_count_variables(mini->env);
	env_copy = copy_env(mini->env, count);
	sort_env(env_copy, count);
	i = 0;
	while (env_copy && env_copy[i] != NULL)
	{
		if (strncmp(env_copy[i], "_=", 2) == 0)
		{
			i++;
			continue ;
		}
		print_variable(env_copy[i]);
		i++;
	}
	free_arr(env_copy);
}

// Exports an env't variable in the format "key=value".
// If the key already exists, it updates the value of the existing key.
// If the key does not exist, it adds a new key-value pair to the env't.
static void	export_with_value(t_mini *mini, char *key, char *value,
												int env_var_index)
{
	if (env_var_index == -1)
	{
		if (add_env_pair(mini, key, value, true) == FAIL)
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

// Handles the `export` command, which sets or updates environment variables.
// if export is called with no args, print_export() is used to print all the env vars.
void	ft_export(t_mini *mini, char *cmd_arg)
{
	char	*key;
	char	*value;
	int		env_var_index;

	if (!cmd_arg || *cmd_arg == '\0')
		return (print_export(mini));
	if (!valid_key(cmd_arg))
	{
		printf("mini: export: %s: not a valid identifier\n", cmd_arg);
		return ;
	}
	key = extract_key(cmd_arg);
	value = extract_value(cmd_arg);
	env_var_index = env_find_index(mini, key);
	if (ft_strchr(cmd_arg, '='))
		export_with_value(mini, key, value, env_var_index);
	else if (env_var_index == -1)
	{
		if (add_env_pair(mini, key, value, false) == FAIL)
			printf("mini: export: Failed to export %s\n", key);
	}
	free_ptr(key);
	free_ptr(value);
}
