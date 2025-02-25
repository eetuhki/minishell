#include "../incl/minishell.h"

void	exec_fail(t_mini *mini, char *cmd)
{
	ft_putstr_fd("mini: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	mini->exit_code = 1;
	//exit(mini->exit_code);
}

void	exec_command(t_mini *mini, char **cmds)
{
	if (!cmds || !cmds[0])
		exit(1);
	validate_cmd_access(mini, cmds[0]);
	if (execve(cmds[0], cmds, mini->env) == -1)
	{
		//printf("mini: execve failed with ERRNO [%d] [%s]\n", errno, strerror(errno));
		exec_fail(mini, cmds[0]);
	}
}

int	handle_redirs(t_cmd *cmd, bool in_pipe, t_mini *mini)
{
	if (cmd->out_file == -2 || cmd->in_file == -2)
	{
		mini->exit_code = 1;
		if (in_pipe)
		{
			ft_putstr_fd("DO WE GET HERE WEYYY?\n", 2);
			free_and_exit(mini);
		}
		else
			return (FAIL);
	}
	if (cmd->in_file != -1)
	{
		if (dup2(cmd->in_file, STDIN) < 0)
		{
			ft_putstr_fd("mini : dup2 input redirection error\n", 2);
			return (FAIL);
		}
		close_fd(cmd->in_file);
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

void reset_std_fds(int stdout_copy, int stdin_copy)
{
    if (stdout_copy != -1)
    {
        if (dup2(stdout_copy, STDOUT_FILENO) == -1)
            ft_putstr_fd("mini : dup2 failed\n", 2);
        close_fd(stdout_copy);
    }
    if (stdin_copy != -1)
    {
        if (dup2(stdin_copy, STDIN_FILENO) == -1)
            ft_putstr_fd("mini : dup2 failed\n", 2);
        close_fd(stdin_copy);
    }
}

int	setup_redirs(t_mini *mini, t_cmd *cmd, int *stdout_copy, int *stdin_copy)
{
	if (cmd->out_file != -1 && cmd->out_file != -2)
    {
        *stdout_copy = dup(STDOUT);
        if (*stdout_copy == -1)
        {
            ft_putstr_fd("mini : dup failed\n", 2);
            return (FAIL);
        }
    }
	if (cmd->in_file != -1 && cmd->in_file != -2)
    {
        *stdin_copy = dup(STDIN);
        if (*stdin_copy == -1)
        {
            ft_putstr_fd("mini : dup failed\n", 2);
            close_fd(*stdout_copy);
            return (FAIL);
        }
    }
	if	(handle_redirs(mini->cmds[0],false, mini) == FAIL)
	{
		reset_std_fds(*stdout_copy, *stdin_copy);
		return (FAIL);
	}
	return (SUCCESS);
}

int		exec_no_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;
	int		builtin_exit;
	int		stdout_copy;
	int		stdin_copy;

	builtin_exit = 0;
	stdout_copy = -1;
	stdin_copy = -1;
	if (builtin_only(mini->cmds_tbl[0][0]))
	{
		if (setup_redirs(mini, mini->cmds[0], &stdout_copy, &stdin_copy) == FAIL)
            return (FAIL);
		builtin_exit = handle_builtin(mini, 0);
		//printf ("Ret value from BUILTIN is [%d] \n", builtin_exit);
		mini->exit_code = builtin_exit;
		reset_std_fds(stdout_copy, stdin_copy);
		return (builtin_exit);
	}
	else
	{
		pid = fork();
		if (check_pid(pid) == 0)
		{
			sig_init_child();
			handle_redirs(mini->cmds[0], true, mini);
			exec_command(mini, mini->cmds_tbl[0]);
		}
		else
			wait_single(mini, pid, &status);
	}
	return (0);
}

void	child_process(t_mini *mini, int i)
{
	sig_init_child();
	if (mini->in_fd != STDIN) // If not first command, use previous pipe as stdin
	{
		dup2(mini->in_fd, STDIN);
		close_fd(mini->in_fd);
	}
	if (i < mini->pipes)  // If it's NOT the last command, redirect stdout to pipe
	{
		dup2(mini->fd[1], STDOUT);
		close_fd(mini->fd[1]);
	}
	close_fd(mini->fd[0]);
	close_fd(mini->fd[1]);
	handle_redirs(mini->cmds[i], true, mini); // Handle file redirections
	if (is_there_type(mini, BUILTIN, i))
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

void	exec_with_pipes(t_mini *mini)
{
    int		i;
	pid_t	pid;

	i = 0;
    mini->pids = ft_calloc((size_t)(mini->pipes + 1), sizeof(pid_t));
    if (!mini->pids)
        return;
	//mini->in_fd = STDIN;
    while (i <= mini->pipes && mini->cmds[i])
    {
        if (i < mini->pipes && pipe(mini->fd) == -1)  // Create a new pipe for all except last command
			return(perror("mini: pipe failed"));
        pid = fork();
		if (pid == 0)  // Child process
			child_process(mini, i);
		else
		{
			mini->pids[i] = pid;
			close_fd(mini->fd[1]); // Parent closes write end immediately
			if (mini->in_fd != STDIN) // Close the old read end
			{
				close_fd(mini->in_fd);
			}
    		mini->in_fd = mini->fd[0]; // Store the read end for the next iteration
		}
        i++;
    }
	if (mini->in_fd != STDIN)
	{
    	close_fd(mini->in_fd); // Close last read end after loop
	}
    wait_multi(mini);
	mini->in_fd = STDIN;
}

int		execute(t_mini *mini)
{
	if (!mini || !mini->cmds_tbl || !*mini->cmds_tbl[0])
		return (0);
	if (cmd_table_size(mini) == 1)
	{
		//ft_putstr_fd("EXEC NO pipes \n", 2);
		// TODO free everything here without exiting
		return (exec_no_pipes(mini));
	}
	else if (cmd_table_size(mini) > 1)
	{
		/* ft_putstr_fd("EXEC with pipes\n", 2);
		ft_putstr_fd("MINI->in_fd before exec_with_pipes()\n", 2);
		ft_putnbr_fd(mini->in_fd, 2); */
		exec_with_pipes(mini);
	}
	return (0);
}
