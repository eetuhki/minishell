#include "../incl/minishell.h"

int	syntax_quotes(t_mini *mini)
{
	size_t	i;

	i = 0;
	while (mini->input[i])
	{
		if (mini->input[i] == SQUOTE)
		{
			i++;
			while (mini->input[i] != SQUOTE && mini->input[i])
				i++;
			if (mini->input[i] == '\0')
				return (syntax_print_error(mini->input[i]));
		}
		if (mini->input[i] == DQUOTE)
		{
			i++;
			while (mini->input[i] != DQUOTE && mini->input[i])
				i++;
			if (mini->input[i] == '\0')
				return (syntax_print_error(mini->input[i]));
		}
		i++;
	}
	return (SUCCESS);
}
