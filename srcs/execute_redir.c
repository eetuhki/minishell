#include "../incl/minishell.h"

int	handle_redirs(t_cmd *cmd, bool in_pipe, t_mini *mini)
{
	if (cmd->out_file == -2 || cmd->in_file == -2)
	{
		mini->exit_code = 1;
		if (in_pipe)
		{
			free_and_exit(mini);
		}
		else
			return (FAIL);
	}
	if (cmd->in_file != -1)
	{
		//printf("Redirecting STDIN to in_file FD: [%d]\n", cmd->in_file);
		if (dup2(cmd->in_file, STDIN) < 0)
		{
			ft_putstr_fd("mini : dup2 input redirection error\n", 2);
			return (FAIL);
		}
		close_fd(cmd->in_file);
		//printf("STDIN redirected to in_file FD: [%d] \n", cmd->in_file);
	}
	if (cmd->out_file != -1)
	{
		if (dup2(cmd->out_file, STDOUT) < 0)
		{
			ft_putstr_fd("mini : dup2 output redirection error\n", 2);
			return (FAIL);
		}
		close_fd(cmd->out_file);
	}
	return (SUCCESS);
}

void reset_std_fds(t_mini *mini)
{
    if (mini->std_fds[1] != -1)
    {
        if (dup2(mini->std_fds[1], STDOUT) == -1)
            ft_putstr_fd("mini : dup2 failed\n", 2);
        close_fd(mini->std_fds[1]);
    }
    if (mini->std_fds[0] != -1)
    {
        if (dup2(mini->std_fds[0], STDIN) == -1)
            ft_putstr_fd("mini : dup2 failed\n", 2);
        close_fd(mini->std_fds[0]);
    }
}

int	setup_redirs(t_mini *mini, t_cmd *cmd)
{
	if (cmd->out_file == -2 || cmd->in_file == -2)
	{
		mini->exit_code = 1;
		return (FAIL);
	}
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
		close_fd(mini->std_fds[1]);
		return (FAIL);
	}
	if	(handle_redirs(mini->cmds[0],false, mini) == FAIL)
	{
		reset_std_fds(mini);
		return (FAIL);
	}
	return (SUCCESS);
}

