#include "../incl/minishell.h"

/* void	exec_fail(t_mini *mini, char *cmd)
{
	ft_putstr_fd("mini: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	mini->exit_code = errno;
	exit(mini->exit_code);
} */

void exit_exec_error(char *cmd)
{
	printf("here [%s]", cmd);
    if (errno == ENOENT) 
        exit(127);
	else if (errno == EACCES)
	{
		if (ft_strchr(cmd, '/'))
        	exit(126);
		else 
			exit(127);
	} 
	else if (errno == EISDIR)
		exit(126);
	else 
        exit(1);
}

void	exec_command(t_mini *mini, char **cmds)
{
	if (!cmds || !cmds[0])
		exit(1);
	if (execve(cmds[0], cmds, mini->env) == -1)
	{
		//exec_fail(mini, cmds[0]);
		printf("mini: execve failed with ERRNO [%d] [%s]\n", errno, strerror(errno));
		exit_exec_error(cmds[0]);
	}
}

void	handle_redirs(t_cmd *cmd)
{
	/* if (cmd->in_file == -2)
	{
		exit(1);
	} */
	if (cmd->in_file != -1)
	{
		if (dup2(cmd->in_file, STDIN) < 0)
			ft_putstr_fd("mini : dup2 input redirection error\n", 2);
		close_fd(cmd->in_file);
	}
	if (cmd->out_file != -1)
	{
		if (dup2(cmd->out_file, STDOUT) < 0)
			ft_putstr_fd("mini : dup2 output redirection error\n", 2);
		close_fd(cmd->out_file);
	}
}

void	exec_no_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;

	// printf("cmds[0][0]:%s\n", mini->cmds_tbl[0][0]);
	if (builtin_only(mini->cmds[0]->tokens[0].content))
	{
		handle_builtin(mini, 0);
		return ;
	}
	pid = fork();
	if (check_pid(pid) == 0)
	{
		sig_init_child();
		handle_redirs(mini->cmds[0]);
		exec_command(mini, mini->cmds_tbl[0]);
	}
	wait_single(mini, pid, &status);
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
		dup2(mini->fd[1], STDOUT);
	close_fd(mini->fd[0]);
	close_fd(mini->fd[1]);
	handle_redirs(mini->cmds[i]); // Handle file redirections
	if (is_there_type(mini, BUILTIN, i))
		handle_builtin(mini, i);
	else
		exec_command(mini, mini->cmds_tbl[i]);
	exit(0);
}

void	exec_with_pipes(t_mini *mini)
{
    int		i;
	pid_t	pid;

	i = 0;
    mini->pids = ft_calloc((size_t)(mini->pipes + 1), sizeof(pid_t));
    if (!mini->pids)
        return;
    while (i <= mini->pipes && mini->cmds[i])
    {
        if (i < mini->pipes && pipe(mini->fd) == -1)  // Create a new pipe for all except last command
			return(perror("mini: pipe failed"));
        pid = fork();
		if (pid == 0)  // Child process
			child_process(mini, i);
		mini->pids[i] = pid;
		close_fd(mini->fd[1]); // Parent closes write end immediately
		if (mini->in_fd != STDIN) // Close the old read end
			close_fd(mini->in_fd);
        mini->in_fd = mini->fd[0]; // Store the read end for the next iteration
        i++;
    }
    close_fd(mini->in_fd); // Close last read end after loop
    wait_multi(mini);
}

void	execute(t_mini *mini)
{
	if (mini && mini->cmds_tbl && *mini->cmds_tbl[0])
	{
		if (cmd_table_size(mini) == 1)
			exec_no_pipes(mini);
		else if (cmd_table_size(mini) > 1)
			exec_with_pipes(mini);
	}
	free_cmds_tbl(mini->cmds_tbl);
}
