#include "../incl/minishell.h"

/* void	syntax_fail(t_mini *mini)
{
	(void)mini;
	//free_ptr(mini->input);
	//ft_putendl_fd("program exited from function: syntax_fail", 2);
	return ;
} */

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
