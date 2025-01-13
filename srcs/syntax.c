#include "../incl/minishell.h"

// validate input syntax before proceeding to tokenization, return SUCCESS or FAIL
int	syntax_check(char *input)
{
	if (!input)
		return (FAIL);
	if (input_is_whitespace(input))
		return (FAIL);
	return (SUCCESS);
}
