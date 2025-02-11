#include "../incl/minishell.h"

// Initialize the expansion variables.
// allocate an empty string for the expansion result.
// Returns SUCCESS or FAIL on malloc failure.
int	xp_init(t_expansion *xp)
{
	xp->in_single_quote = false;
	xp->in_double_quote = false;
	xp->env_value = NULL;
	xp->expanded_str = ft_strdup("");
	if (!xp->expanded_str)
	{
		ft_putstr_fd("mini: expansion: memory allocation failed\n", 2);
		return (FAIL);
	}
	return (SUCCESS);
}

// Extract the variable name from the input string
// The variable name is copied into the char *var_name
// up to (size - 1) characters and null terminated.
// If no valid variable name was found, var_name[0]='\0'
void	extract_env_var_name(char *input, char *var_name, int size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i] && input[i] != '$')
		i++;
	if (input[i] == '$')
		i++;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
	{
		if (j >= size - 1)
		{
			ft_putstr_fd("mini: expansion: environment variable name too long. \
						truncating.\n", 2);
			break ;
		}
		var_name[j++] = input[i++];
	}
	var_name[j] = '\0';
}

// Toggle the quote flags depending on the input char.
// Returns true if the char was a single or double quote
// and handled.
bool	update_quote_flags(t_expansion *xp, char input)
{
	if (input == SQUOTE && !xp->in_double_quote)
	{
		xp->in_single_quote = !xp->in_single_quote;
		return (true);
	}
	else if (input == DQUOTE && !xp->in_single_quote)
	{
		xp->in_double_quote = !xp->in_double_quote;
		return (true);
	}
	return (false);
}

// Handle the special '$?' expansion
// Appends the exit code (converted to string) to the expanded_str
// Advances the input pointer past "$?"
int	expand_exit_status(t_mini *mini, t_expansion *xp, char **input)
{
	char	*exit_code_str;
	char	*temp;

	exit_code_str = ft_itoa(mini->exit_code);
	if (!exit_code_str)
	{
		ft_putstr_fd("mini: expansion: memory allocation failed\n", 2);
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	temp = ft_strjoin(xp->expanded_str, exit_code_str);
	free_ptr(exit_code_str);
	if (!temp)
	{
		ft_putstr_fd("mini: expansion: memory allocation failed\n", 2);
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	free_ptr(xp->expanded_str);
	xp->expanded_str = temp;
	*input += 2;
	return (SUCCESS);
}

//copies expantion result to the str pointer
void copy_expantion_res(char **str, t_expansion xp)
{
	if (*str)
		free_ptr(*str);
    *str = xp.expanded_str;
}