#include "../incl/minishell.h"

static void	exec_command(t_mini *mini, char **cmds)
{
	validate_cmd_access(mini, cmds[0]);
	if (execve(cmds[0], cmds, mini->env) == -1)
	{
		exec_fail(mini, cmds[0]);
	}
}

static int	exec_no_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;

	if (builtin_only(mini->cmds_tbl[0][0]))
	{
		if (setup_redirs(mini, mini->cmds[0]) == FAIL)
			return (FAIL);
		mini->exit_code = handle_builtin(mini, 0);
		reset_std_fds(mini);
		return (mini->exit_code);
	}
	pid = fork();
	if (check_pid(pid, mini) < 0)
		return (FAIL);
	if (pid == 0)
	{
		sig_init_child();
		handle_redirs(mini->cmds[0], true, mini);
		exec_command(mini, mini->cmds_tbl[0]);
	}
	else
		wait_single(mini, pid, &status);
	return (SUCCESS);
}

static void	child_process(t_mini *mini, int i)
{
	sig_init_child();
	pipe_redirect(mini, i);
	close_fd(&mini->fd[0]);
	close_fd(&mini->fd[1]);
	close_inherited_fds(mini, i);
	handle_redirs(mini->cmds[i], true, mini);
	if (builtin_only(*mini->cmds_tbl[i]))
	{
		mini->exit_code = handle_builtin(mini, i);
		free_and_exit(mini);
	}
	else
		exec_command(mini, mini->cmds_tbl[i]);
	free_and_exit(mini);
}

static int	exec_with_pipes(t_mini *mini)
{
	int		i;
	pid_t	pid;

	i = 0;
	if (allocate_pids(mini) == FAIL)
		return (FAIL);
	while (i <= mini->pipes && mini->cmds[i])
	{
		if (i < mini->pipes && create_pipe(mini->fd) == FAIL)
			return (FAIL);
		pid = fork();
		if (check_pid(pid, mini) < 0)
			return (FAIL);
		if (pid == 0)
			child_process(mini, i);
		else
			handle_fds(mini, pid, i);
		i++;
	}
	if (mini->in_fd != STDIN)
		close_fd(&mini->in_fd);
	wait_multi(mini);
	mini->in_fd = STDIN;
	return (SUCCESS);
}

int	execute(t_mini *mini)
{
	if (!mini || !mini->cmds_tbl || !*mini->cmds_tbl || !*mini->cmds_tbl[0])
		return (0);
	if (cmd_table_size(mini) == 1)
	{
		mini->in_pipe = false;
		if (exec_no_pipes(mini) == FAIL)
		{
			mini->exit_code = 1;
			return (FAIL);
		}
	}
	else if (cmd_table_size(mini) > 1)
	{
		mini->in_pipe = true;
		if (exec_with_pipes(mini) == FAIL)
		{
			mini->exit_code = 1;
			return (FAIL);
		}
	}
	return (SUCCESS);
}
