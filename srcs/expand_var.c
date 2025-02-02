#include "../incl/minishell.h"

int	xp_init(t_expansion *xp)
{
	xp->in_single_quote = false;
	xp->in_double_quote = false;
	xp->env_value = NULL;
	xp->expanded_str = ft_strdup("");
	if (!xp->expanded_str)
		return (FAIL);
	return (SUCCESS);
}

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
			break;
		var_name[j++] = input[i++];
	}
	var_name[j] = '\0';
}

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

static int append_str_to_expansion(t_expansion *xp, char *str)
{
	char *temp;

	temp = ft_strjoin(xp->expanded_str, str);
	if (!temp)
	{
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	free_ptr(xp->expanded_str);
	xp->expanded_str = temp;
	return (SUCCESS);
}

static int append_char_to_expansion(t_expansion *xp, char c)
{
	size_t len;
	char *temp;

	len = ft_strlen(xp->expanded_str);
   	temp = ft_realloc(xp->expanded_str, len + 2);
	if (!temp)
	{
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	xp->expanded_str = temp;
	xp->expanded_str[len] = c;
	xp->expanded_str[len + 1] = '\0';
	return (SUCCESS);
}

static int expand_exit_status(t_mini *mini, t_expansion *xp, char **input)
{
	char *exit_code_str;
	char *temp;

	exit_code_str = ft_itoa(mini->exit_code);
	if (!exit_code_str)
	{
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	temp = ft_strjoin(xp->expanded_str, exit_code_str);
	free_ptr(exit_code_str);
	if (!temp)
	{
		free_ptr(xp->expanded_str);
		return (FAIL);
	}
	free_ptr(xp->expanded_str);
	xp->expanded_str = temp;
	*input += 2;
	return (SUCCESS);
}

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

void expand_var(t_mini *mini, char *str, char **expanded)
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
