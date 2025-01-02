#include "../incl/minishell.h"

// print syntax error message
void	syntax_print_error(char token)
{
	ft_putstr_fd("mini: syntax error near unexpected token ", 2);
	ft_putchar_fd(token, 2);
	ft_putstr_fd("\'", 2);
	ft_putstr_fd("\n", 2);
}
