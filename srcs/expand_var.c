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
			ft_putstr_fd("mini: expansion: environment variable name too long. truncating.\n", 2);
			break;
		}
		var_name[j++] = input[i++];
	}
	var_name[j] = '\0';
}

// Toggle the quote flags depending on the input char.
// Returns true if the char was a single or double quote
// and handled.
bool update_quote_flags(t_expansion *xp, char input)
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

// Append the given string to the expanded_str.
// Returns SUCCESS or FAIL on allocation failure.
static int append_str_to_expansion(t_expansion *xp, char *str)
{
	char *temp;

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

// Append a single char to the expanded_str.
// Returns SUCCESS or FAIL on allocation failure.
static int append_char_to_expansion(t_expansion *xp, char c)
{
	size_t len;
	char *temp;

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
	return (SUCCESS);
}

// Handle the special '$?' expansion
// Appends the exit code (converted to string) to the expanded_str
// Advances the input pointer past "$?"
static int expand_exit_status(t_mini *mini, t_expansion *xp, char **input)
{
	char *exit_code_str;
	char *temp;

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
		ft_putstr_fd("mini: exp: memory allocation failed\n", 2);
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	free_ptr(xp->expanded_str);
	xp->expanded_str = temp;
	*input += 2;
	return (SUCCESS);
}

// Process a variable expansion
// It extracts the variable name, looks up its value from
// the environment and appends it to the str being expanded.
// Advances the input pointer by var_len
// If no valid variable name was found, it appends only '$'
static int expand_env_variable(t_mini *mini, t_expansion *xp, char **input)
{
	char var[256];
	size_t var_len;

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
static int handle_dollar_sign(t_mini *mini, t_expansion *xp, char **input)
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
// The expanded result is copied to the expanded pointer.
void expand_variables(t_mini *mini, char *str, char **expanded)
{
	t_expansion xp;
	char *input;

	if (xp_init(&xp) == FAIL)
		return;
	input = str;
	while (*input)
	{
		if (update_quote_flags(&xp, *input))
		{
			input++;
			continue;
		}
		if (*input == '$' && !xp.in_single_quote)
		{
			if (handle_dollar_sign(mini, &xp, &input) == FAIL)
				return;
			continue;
		}
		if (append_char_to_expansion(&xp, *input) == FAIL)
			return;
		input++;
	}
	*expanded = xp.expanded_str;
}
