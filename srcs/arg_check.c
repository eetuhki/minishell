#include "../incl/minishell.h"

void	arg_check(int ac, char **av)
{
	if (ac != 1 || av[1])
	{
		printf("Please run minishell without any additional arguments\n");
		exit(0);
	}
}
