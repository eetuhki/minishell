#include "../incl/minishell.h"

void err_cmd_is_dir(char *cmd)
{
	if (cmd)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("Is a directory\n", 2);
	}
}
