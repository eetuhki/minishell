#include "../incl/minishell.h"

// Sorts the env variable array in alphabetical order using bubble sort.
static void	sort_env(char **env_copy, ssize_t size)
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

// Prints an env't variable in the format => declare -x key="value"
// If the env var has a value (indicated by '='), it's value will be
// printed in double quotes. if not only the key is printed.
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
		{
			if (env_var[j] == '"' || env_var[j] == '$')
				printf("%s", "\\");
			printf("%c", env_var[j++]);
		}
		printf("\"");
	}
	printf("\n");
}

// Creates a sorted copy of the env't variables and prints them.
// Copies the env't variables into a new array.
// Sorts the copied env't variables alphabetically.
// Iterates through the sorted array, skipping the special "_=" variable.
// Prints each env variable using the print_variable function.
int	print_export(t_mini *mini)
{
	int		i;
	size_t	count;
	char	**env_copy;

	count = env_count_variables(mini->env);
	env_copy = copy_env(mini->env, count);
	if (!env_copy)
		return (FAIL);
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
	return (SUCCESS);
}
