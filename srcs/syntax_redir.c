#include "../incl/minishell.h"

// find special characters for given redirection type < or >
char	*redir_special_chars(char direction)
{
	char	*specials;

	if (direction == '<')
		specials = ">|";
	else
		specials = "<|";
	return (specials);
}

// check for invalid characters following a redirection
int	invalid_chars(t_mini *mini, size_t i, char direction)
{
	char	*specials;

	specials = redir_special_chars(direction);
	if (mini->input[i] == '|' && !mini->redir_open)
		mini->redir_count = 0;
	if (mini->redir_open && ft_strchr(specials, mini->input[i]))
		return (FAIL);
	if (ft_isprint(mini->input[i]) && mini->redir_open && mini->input[i] != '|'
		&& mini->input[i] != direction && !ft_isspace(mini->input[i]))
	{
		mini->redir_count = 0;
		mini->redir_open = 0;
	}
	return (SUCCESS);
}

// validates redirection characters in more detail, allowing for << or >>, 
// redirection not allowed at end of input 
// or followed by invalid characters, return SUCCESS or FAIL
int	validate_redir(t_mini *mini, size_t i, char direction)
{
	if (mini->input[i] == direction)
	{
		if (mini->redir_count > 0 && mini->input[i - 1] != direction)
			return (FAIL);
		mini->redir_count++;
		if (mini->redir_count > 2)
			return (FAIL);
		mini->redir_open = 1;
	}
	else if (invalid_chars(mini, i, direction))
		return (FAIL);
	if (mini->redir_open && mini->input[i + 1] == '\0')
		return (FAIL);
	return (SUCCESS);
}

// validates redirection syntax by ignoring quotes and calling the 
// validate_redir function.
// return SUCCESS or FAIL (while printing error via syntax_print_error)
int	syntax_redir(t_mini *mini, char direction)
{
	size_t	i;
	size_t	skip;

	i = 0;
	skip = 0;
	while (mini->input[i])
	{
		skip = skip_quotes(&mini->input[i]);
		if (skip)
			i += skip;
		if (validate_redir(mini, i, direction) == FAIL)
			return (syntax_print_error(direction));
		i++;
	}
	return (SUCCESS);
}
