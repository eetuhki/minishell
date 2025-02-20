#include "../incl/minishell.h"

// Append a single char to the expanded_str.
// Returns SUCCESS or FAIL on allocation failure.
static int	append_char_to_expansion(t_expansion *xp, char c)
{
	size_t	len;
	char	*temp;

	len = ft_strlen(xp->expanded_str);
	temp = ft_realloc(xp->expanded_str, len + 2);
	if (!temp)
	{
		ft_putstr_fd("mini: expansion: memory allocation failed\n", 2);
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	xp->expanded_str = temp;
	xp->expanded_str[len] = c;
	xp->expanded_str[len + 1] = '\0';
	// free_ptr(temp);
	return (SUCCESS);
}

// Append the given string to the expanded_str.
// Returns SUCCESS or FAIL on allocation failure.
static int	append_str_to_expansion(t_expansion *xp, char *str)
{
	char	*temp;

	temp = ft_strjoin(xp->expanded_str, str);
	if (!temp)
	{
		ft_putstr_fd("mini: expansion: memory allocation failed\n", 2);
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	free_ptr(xp->expanded_str);
	xp->expanded_str = temp;
	return (SUCCESS);
}

// Process a variable expansion
// It extracts the variable name, looks up its value from
// the environment and appends it to the str being expanded.
// Advances the input pointer by var_len
// If no valid variable name was found, it appends only '$'
static int	expand_env_variable(t_mini *mini, t_expansion *xp, char **input)
{
	char	var[256];
	size_t	var_len;

	extract_env_var_name(*input, var, sizeof(var));
	if (var[0] == '\0')
	{
		if (append_char_to_expansion(xp, '$') == FAIL)
			return (FAIL);
		(*input)++;
		return (SUCCESS);
	}
	(*input)++;
	var_len = ft_strlen(var);
	xp->env_value = env_get_var(mini, var);
	if (!xp->env_value)
		xp->env_value = "";
	if (append_str_to_expansion(xp, xp->env_value) == FAIL)
		return (FAIL);
	(*input) += var_len;
	return (SUCCESS);
}

// Process an expansion when a '$' is encountered
// If the character following '$' is '?', expand the exit status
// otherwise, expand an environment variable
static int	handle_dollar_sign(t_mini *mini, t_expansion *xp, char **input)
{
	if ((*(*input + 1)) == '?')
	{
		if (expand_exit_status(mini, xp, input) == FAIL)
			return (FAIL);
	}
	else if (expand_env_variable(mini, xp, input) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

// Expands environment variables in the given string while handling quotes.
// Single quotes prevent expansion, while double quotes allow it.
// Tracks nested and multiple quotes properly.
// The expanded result is copied to the str pointer.
int	expand_variables(t_mini *mini, char **str)
{
	t_expansion	xp;
	char		*input;

	if (xp_init(&xp) == FAIL)
		return (FAIL);
	input = *str;
	while (*input)
	{
		if (update_quote_flags(&xp, *input))
		{
			input++;
			continue ;
		}
		if (*input == '$' && !xp.in_single_quote)
		{
			if (handle_dollar_sign(mini, &xp, &input) == FAIL)
				return (FAIL);
			continue ;
		}
		if (append_char_to_expansion(&xp, *input) == FAIL)
			return (FAIL);
		input++;
	}
	copy_expantion_res(str, xp);
	return (SUCCESS);
}
