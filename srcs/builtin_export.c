#include "../incl/minishell.h"

// Exports an env't variable in the format "key=value".
// If the key already exists, it updates the value of the existing key.
// If the key does not exist, it adds a new key-value pair to the env't.
static int	export_with_value(t_mini *mini, char *key, char *value,
												int env_var_index)
{
	if (env_var_index == -1)
	{
		if (add_env_pair(mini, key, value, true) == FAIL)
		{
			print_err_builtin("export: ", key, ": Failed to export");
			return (FAIL);
		}
	}
	else
	{
		if (env_set_var(mini, key, value) == FAIL)
		{
			print_err_builtin("export: ", key, ": Failed to update");
			return (FAIL);
		}
	}
	return (SUCCESS);
}

static int	free_return(char **key, char **value, int ret_value)
{
	free_str(key);
	free_str(value);
	return (ret_value);
}

static	int	validate_export_key(char *cmd_arg)
{
	if (!valid_key(cmd_arg, false))
	{
		print_err_builtin("export: `", cmd_arg, "': not a valid identifier");
		return (FAIL);
	}
	return (SUCCESS);
}

// Processes a single export cmd arg by extracting the key-value pair
// and updating or adding it to the environment.
// If the key is invalid, an error is printed.
static int	ft_export_single(t_mini *mini, char *cmd_arg)
{
	char	*key;
	char	*value;
	int		env_var_index;

	if (validate_export_key(cmd_arg) == FAIL)
		return (FAIL);
	key = extract_key(cmd_arg);
	value = extract_value(cmd_arg);
	env_var_index = search_key_in_env(mini, key);
	if (ft_strchr(cmd_arg, '='))
	{
		if (export_with_value(mini, key, value, env_var_index) == FAIL)
			return (free_return(&key, &value, FAIL));
	}
	else if (env_var_index == -1)
	{
		if (add_env_pair(mini, key, value, false) == FAIL)
		{
			print_err_builtin("export: ", key, ": Failed to export");
			return (free_return(&key, &value, FAIL));
		}
	}
	return (free_return(&key, &value, SUCCESS));
}

// Handles the `export` command, which sets or updates environment variables.
// Iterates through the provided arguments and
// calls `ft_export_single()` for each one.
// if export is called with no args,
// print_export() is used to print all the env vars.
int	ft_export(t_mini *mini, char **cmd_args)
{
	int	i;
	int	ret_val;

	i = 1;
	ret_val = SUCCESS;
	if (!cmd_args || !*cmd_args)
		return (FAIL);
	if (*(cmd_args + 1) == NULL)
		return (print_export(mini));
	while (cmd_args[i])
	{
		if (ft_export_single(mini, cmd_args[i]) == FAIL)
			ret_val = FAIL;
		i++;
	}
	return (ret_val);
}
