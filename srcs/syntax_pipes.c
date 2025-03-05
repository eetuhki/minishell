#include "../incl/minishell.h"

// validates syntax for pipes: pipe can't be the first character or 
// followed by another pipe ('||'), consist of whitespace only
// or at the end, returns SUCCESS or FAIL
// (while printing syntax error message)
int	syntax_pipes(t_mini *mini)
{
	size_t	i;

	if (mini->input[0] == '|')
		return (syntax_print_error(mini->input[0]));
	i = 0;
	while (mini->input[i])
	{
		if (mini->input[i] && mini->input[i] == '|'
			&& !check_quotes(mini->input, i))
		{
			if (mini->input[i + 1] == '\0' || mini->input[i + 1] == '|'
				|| pipe_is_whitespace(mini->input, i))
				return (syntax_print_error('|'));
		}
		i++;
	}
	return (SUCCESS);
}
