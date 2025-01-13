#include "../incl/minishell.h"

// prints syntax error message, returns FAIL
int	syntax_print_error(char token)
{
	ft_putstr_fd("mini: syntax error near unexpected token '", 2);
	if (!token)
		ft_putstr_fd("newline", 2);
	ft_putchar_fd(token, 2);
	ft_putchar_fd('\'', 2);
	ft_putchar_fd('\n', 2);
	return (FAIL);
}
