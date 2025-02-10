#include "../incl/minishell.h"

void	syntax_fail(t_mini *mini)
{
	free_ptr(mini->input);
	ft_putendl_fd("program exited from function: syntax_fail", 2);
	exit (SUCCESS);
}

// validate input syntax before proceeding to tokenization, 
// return SUCCESS or FAIL
void	syntax_check(t_mini *mini)
{
	if (!mini->input)
		syntax_fail(mini);
	if (ft_strpbrk(mini->input, DELIMITERS))
	{
		if (input_is_whitespace(mini->input))
			ft_printf("input is whitespace\n");
		if (ft_strchr(mini->input, '|') && syntax_pipes(mini))
			syntax_fail(mini);
		if (ft_strpbrk(mini->input, QUOTES) && syntax_quotes(mini))
			syntax_fail(mini);
		if (ft_strchr(mini->input, '<') && syntax_redir(mini, '<'))
			syntax_fail(mini);
		if (ft_strchr(mini->input, '>') && syntax_redir(mini, '>'))
			syntax_fail(mini);
	}
}
