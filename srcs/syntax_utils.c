#include "../incl/minishell.h"

// check if input is only whitespace or not. return: 1 if nothing but whitespace, else: 0
int	input_is_whitespace(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
	}
	if (input[i] == '\0')
		return (1);
	return (0);
}
