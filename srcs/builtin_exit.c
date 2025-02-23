#include "../incl/minishell.h"

// validate that the argument consists of numbers only
static int	num_only(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}

// print error message if argument is not num (or overflowing LONG)
static void	arg_not_num(t_mini *mini, char *arg)
{
	ft_putstr_fd("mini: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("numeric argument required\n", 2);
	free_ptr(mini);
	clear_history();
	exit(2);
}

// convert the char arg to long, if atol overflows send to arg_not_num
static long	check_exit_code(t_mini *mini, char *arg)
{
	long	exit_code;

	exit_code = ft_atol(arg);
	if ((exit_code == 0 || exit_code == -1)
		&& ft_strcmp(arg, ft_itoa(exit_code)))
		arg_not_num(mini, arg);
	return (exit_code);
}

// handle built-in exit, check if there's more than 1 argument
void	ft_exit(t_mini *mini, char **args)
{
	long	exit_code;

	exit_code = 0;
	ft_putstr_fd("exit\n", 0);
	if (!args[1])
		exit_code = mini->exit_code;
	ft_putendl_fd("inside ft_exit", 2);
	ft_putnbr_fd(mini->exit_code, 2);
	ft_putendl_fd("  <- exit_code ", 2);
	ft_putstr_fd(args[1], 2);
	ft_putendl_fd("  <- args[1] ", 2);
	if (args[1] && num_only(args[1]) == FAIL)
		arg_not_num(mini, args[1]);
	else if (args[1] && args[2])
	{
		ft_putstr_fd("mini: exit: too many arguments\n", 2);
		mini->exit_code = 1;
		return ;
	}
	else if (args[1] && !args[2])
		exit_code = check_exit_code(mini, args[1]);
	clear_history();
	mini->exit_code = exit_code;
	ft_putendl_fd("mini->exitcode at FT_EXIT",2);
	ft_putnbr_fd(mini->exit_code, 2);
	exit(mini->exit_code);
}
