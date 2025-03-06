#include "../incl/minishell.h"

void	exec_command(t_mini *mini, char **cmds)
{
	validate_cmd_access(mini, cmds[0]);
	if (execve(cmds[0], cmds, mini->env) == -1)
	{
		exec_fail(mini, cmds[0]);
	}
}

int	exec_no_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;
	int		builtin_exit;

	builtin_exit = 0;
	if (builtin_only(mini->cmds_tbl[0][0]))
	{
		if (setup_redirs(mini, mini->cmds[0]) == FAIL)
			return (FAIL);
		builtin_exit = handle_builtin(mini, 0);
		mini->exit_code = builtin_exit;
		reset_std_fds(mini);
		return (builtin_exit);
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

void	close_inherited_fds(t_mini *mini, int pipe_i)
{
	int	i;

	i = 0;
	while (mini && mini->cmds[i])
	{
		if (i != pipe_i)
		{
			close_fd(&mini->cmds[i]->in_file);
			close_fd(&mini->cmds[i]->out_file);
		}
		i++;
	}
}

// If not first command, use previous pipe as stdin
// If it's NOT the last command, redirect stdout to pipe
// Handle file redirections
void	child_process(t_mini *mini, int i)
{
	sig_init_child();
	if (mini->in_fd != STDIN)
	{
		if (dup2(mini->in_fd, STDIN) < 0)
		{
			ft_putstr_fd("mini : dup2 failed\n", 2);
			mini->exit_code = 1;
			free_and_exit(mini);
		}
		close_fd(&mini->in_fd);
	}
	if (i < mini->pipes)
	{
		if (dup2(mini->fd[1], STDOUT) < 0)
		{
			ft_putstr_fd("mini : dup2 failed\n", 2);
			mini->exit_code = 1;
			free_and_exit(mini);
		}
		close_fd(&mini->fd[1]);
	}
	close_fd(&mini->fd[0]);
	close_fd(&mini->fd[1]);
	close_inherited_fds(mini, i);
	handle_redirs(mini->cmds[i], true, mini);
	if (builtin_only(*mini->cmds_tbl[i]))
	{
		if (handle_builtin(mini, i) == FAIL)
			mini->exit_code = EXIT_FAILURE;
		else
			mini->exit_code = EXIT_SUCCESS;
		free_and_exit(mini);
	}
	else
		exec_command(mini, mini->cmds_tbl[i]);
	free_and_exit(mini);
}

// Create a new pipe for all except last command
// Close last read end after loop
int	exec_with_pipes(t_mini *mini)
{
	int		i;
	pid_t	pid;

	i = 0;
	if (mini->pids)
		free_ptr(mini->pids);
	mini->pids = ft_calloc((size_t)(mini->pipes + 1), sizeof(pid_t));
	if (!mini->pids)
		return (FAIL);
	while (i <= mini->pipes && mini->cmds[i])
	{
		if (i < mini->pipes && pipe(mini->fd) == -1)
		{
			perror("mini: pipe failed");
			return (FAIL);
		}
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
		return (SUCCESS);
	}
	else if (cmd_table_size(mini) > 1)
	{
		mini->in_pipe = true;
		if (exec_with_pipes(mini) == FAIL)
		{
			mini->exit_code = 1;
			return (FAIL);
		}
		return (SUCCESS);
	}
	return (SUCCESS);
}
