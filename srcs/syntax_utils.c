#include "../incl/minishell.h"

// check if input is only whitespace or not, returns 1 if nothing but whitespace, else 0
int	input_is_whitespace(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (0);
		i++;
	}
	return (1);
}

// skips single or double quotes, returns index
size_t	skip_quotes(char *input)
{
	size_t	i;

	i = 0;
	if (input[i] == SQUOTE)
	{
		i++;
		while (input[i] && input[i] != SQUOTE)
			i++;
	}
	else if (input[i] == DQUOTE)
	{
		i++;
		while (input[i] && input[i] != DQUOTE)
			i++;
	}
	return (i);
}
