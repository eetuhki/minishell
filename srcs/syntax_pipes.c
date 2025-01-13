#include "../incl/minishell.h"

// validates syntax for pipes: pipe can't be the first character or followed by another pipe ('||')
// or at the end, returns SUCCESS or FAIL (while printing syntax error message)
int	syntax_pipes(t_mini *mini)
{
	size_t	i;
	size_t	skip;

	if (mini->input[0] == '|')
		return (syntax_print_error(mini->input[0]));
	i = 0;
	skip = 0;
	while (mini->input[i])
	{
		skip = skip_quotes(&mini->input[i]);
		if (skip)
			i += skip;
		if (mini->input[i] == '|')
		{
			if (mini->input[i + 1] == '|' || mini->input[i + 1] == '\0')
				return (syntax_print_error('|'));
		}
		i++;
	}
	return (SUCCESS);
}
