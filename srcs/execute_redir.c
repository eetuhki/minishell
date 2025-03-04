#include "../incl/minishell.h"

static int	handle_file_err(t_mini *mini, bool in_pipe)
{
	mini->exit_code = 1;
	if (in_pipe)
	{
		free_and_exit(mini);
	}
	return (FAIL);
}

int	handle_redirs(t_cmd *cmd, bool in_pipe, t_mini *mini)
{
	if (cmd->out_file == -2 || cmd->in_file == -2)
		return (handle_file_err(mini, in_pipe));
	if (handle_infile_redir(cmd) == FAIL)
		return (FAIL);
	if (handle_outfile_redir(cmd) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

void	reset_std_fds(t_mini *mini)
{
	if (mini->std_fds[1] != -1)
	{
		if (dup2(mini->std_fds[1], STDOUT) == -1)
			ft_putstr_fd("mini : dup2 failed\n", 2);
		close_fd(&mini->std_fds[1]);
	}
	if (mini->std_fds[0] != -1)
	{
		if (dup2(mini->std_fds[0], STDIN) == -1)
			ft_putstr_fd("mini : dup2 failed\n", 2);
		close_fd(&mini->std_fds[0]);
	}
}

static int	save_std_fds(t_mini *mini)
{
	mini->std_fds[1] = dup(STDOUT);
	if (mini->std_fds[1] == -1)
	{
		ft_putstr_fd("mini : dup failed\n", 2);
		return (FAIL);
	}
	mini->std_fds[0] = dup(STDIN);
	if (mini->std_fds[0] == -1)
	{
		ft_putstr_fd("mini : dup failed\n", 2);
		close_fd(&mini->std_fds[1]);
		return (FAIL);
	}
	return (SUCCESS);
}

int	setup_redirs(t_mini *mini, t_cmd *cmd)
{
	if (cmd->out_file == -2 || cmd->in_file == -2)
	{
		mini->exit_code = 1;
		return (FAIL);
	}
	if (save_std_fds(mini) == FAIL)
		return (FAIL);
	if (handle_redirs(mini->cmds[0], false, mini) == FAIL)
	{
		reset_std_fds(mini);
		return (FAIL);
	}
	return (SUCCESS);
}
