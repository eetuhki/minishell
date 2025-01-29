#include "../incl/minishell.h"

// check if quotes are open at given index (limiter)
int	check_quotes(char *input, int limiter)
{
	int	sq_open;
	int	dq_open;
	int	i;

	sq_open = 0;
	dq_open = 0;
	i = 0;
	while (input[i] && i < limiter)
	{
		if (input[i] == SQUOTE && !dq_open)
			sq_open = !sq_open;
		else if (input[i] == DQUOTE && !sq_open)
			dq_open = !dq_open;
		i++;
	}
	if (sq_open || dq_open)
		return (FAIL);
	return (SUCCESS);
}
