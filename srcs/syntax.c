#include "../incl/minishell.h"

// validate input syntax before proceeding to tokenization, 
// return SUCCESS or FAIL
int	syntax_check(t_mini *mini)
{
	if (!mini->input)
		return (FAIL);
	if (ft_strpbrk(mini->input, DELIMITERS))
	{
		if (input_is_whitespace(mini->input))
			ft_printf("mini: input is whitespace\n");
		if (ft_strchr(mini->input, '|') && syntax_pipes(mini))
			return (FAIL);
		if (ft_strpbrk(mini->input, QUOTES) && syntax_quotes(mini))
			return (FAIL);
		if (ft_strchr(mini->input, '<') && syntax_redir(mini, '<'))
			return (FAIL);
		if (ft_strchr(mini->input, '>') && syntax_redir(mini, '>'))
			return (FAIL);
	}
	return (SUCCESS);
}
