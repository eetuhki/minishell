#include "../incl/minishell.h"

// check whether arguments first character is '-' and
// while it's followed by 'n' characters the flag is valid
// returns: true if valid -n flag, false if not
static bool	n_flag(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] != '-')
		return (false);
	while (arg[++i])
	{
		if (arg[i] != 'n')
			return (false);
	}
	return (true);
}

// checks whether the argument following echo is the -n option
// if -n flag: set the flag to true, write the next arguments and don't print '\n'
// else: print out arguments one-bb
// followed by the newline
void	ft_echo(char **args)
{
	int	i;
	int	flag;

	i = 0;
	flag = false;
	while (args[++i] && n_flag(args[i]))
		flag = true;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!flag)
		printf("\n");
}
