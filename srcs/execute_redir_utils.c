#include "../incl/minishell.h"

int	handle_infile_redir(t_cmd *cmd)
{
	if (cmd->in_file != -1)
	{
		if (dup2(cmd->in_file, STDIN) < 0)
		{
			ft_putstr_fd("mini : dup2 input redirection error\n", 2);
			return (FAIL);
		}
		close_fd(&cmd->in_file);
	}
	return (SUCCESS);
}

int	handle_outfile_redir(t_cmd *cmd)
{
	if (cmd->out_file != -1)
	{
		if (dup2(cmd->out_file, STDOUT) < 0)
		{
			ft_putstr_fd("mini : dup2 output redirection error\n", 2);
			return (FAIL);
		}
		close_fd(&cmd->out_file);
	}
	return (SUCCESS);
}
