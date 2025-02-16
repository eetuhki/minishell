#include "../incl/minishell.h"

void	exec_command(t_mini *mini, char **cmds)
{
	if (!cmds || !cmds[0])
		exit(1);
	if (execve(cmds[0], cmds, mini->env) == -1)
	{
		printf("mini : execve fail\n");
		exit(127);
	}
}

void	handle_redirs(t_cmd *cmd)
{
	if (cmd->in_file != -1)
	{
		if (dup2(cmd->in_file, STDIN) < 0)
			printf("mini : dup2 input redirection error\n");
		close_fd(cmd->in_file);
	}
	if (cmd->out_file != -1)
	{
		if (dup2(cmd->out_file, STDOUT) < 0)
			printf("mini : dup2 output redirection error\n");
		close_fd(cmd->out_file);
	}
}

void	exec_no_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;

	if (builtin_only(mini->cmds[0]))
	{
		handle_builtin(mini, 0);
		return ;
	}
	pid = fork();
	if (check_pid(pid) == 0)
	{
		handle_redirs(mini->cmds[0]);
		// if (is_there_type(mini, BUILTIN))
			// handle_builtin(mini, 0);
		exec_command(mini, mini->cmds_tbl[0]);
	}
	wait_single(mini, pid, &status);
}

void	child_process(t_mini *mini, int	*fd, int i)
{
	if (i < mini->pipes - 1)
	{
		dup2(fd[1], STDOUT);
		close_fds(fd);
	}
	handle_redirs(mini->cmds[i]);
	if (is_there_type(mini, BUILTIN))
		handle_builtin(mini, i);
	exec_command(mini, mini->cmds_tbl[i]);
}

void	exec_funct(t_mini *mini, int *fd, int i, int prev_fd)
{
	pid_t	pid;

	mini->pids = malloc(sizeof(pid_t) * mini->pipes);
	if (!mini->pids)
		return ;
	pid = fork();
	if (check_pid(pid) == 0)
	{
		if (prev_fd != STDIN)
		{
			dup2(prev_fd, STDIN);
			close_fd(prev_fd);
		}
		child_process(mini, fd, i);
	}
	mini->pids[i] = pid;
	if (prev_fd != STDIN)
		close_fd(prev_fd);
	if (i < mini->pipes - 1)
	{
		close_fd(fd[1]);
		prev_fd = fd[0];
	}
	// while (wait(&status) > 0)
	//	;
}

void	exec_with_pipes(t_mini *mini)
{
	int		fd[2];
	int		prev_fd;
	int		i;

	i = 0;
	prev_fd = STDIN;
	while (i < mini->pipes && mini->cmds[i])
	{
		if (pipe(fd) == -1)
			ft_putendl_fd("mini: pipe failed", 2);
		exec_funct(mini, fd, i, prev_fd);
		prev_fd = fd[0];
		i++;
	}
	if (prev_fd != STDIN)
		close_fd(prev_fd);
	wait_multi(mini);
}

void	execute(t_mini *mini)
{
	if (mini->cmds_tbl)
	{
		if (cmd_table_size(mini) == 1)
			exec_no_pipes(mini);
		else if (cmd_table_size(mini) > 1)
			exec_with_pipes(mini);
	}
	free_cmds_tbl(mini->cmds_tbl);
}



	/* int	i;

	if (is_there_type(mini, REDIR_IN) || is_there_type(mini, REDIR_OUT)
		|| is_there_type(mini, HEREDOC) || is_there_type(mini, APPEND))
	{
		i = 0;
		while (cmd->tokens[i].content && cmd->tokens[i + 1].content)
		{
			if ((cmd->tokens[i].type == REDIR_IN && 
				cmd->tokens[i + 1].type == INFILE) || (cmd->tokens[i].type == REDIR_OUT && 
					cmd->tokens[i + 1].type == OUTFILE) || (cmd->tokens[i].type == HEREDOC && 
					cmd->tokens[i + 1].type == LIMITER) || (cmd->tokens[i].type == APPEND && 
					cmd->tokens[i + 1].type == APP_OUT))
			{

			}
			i++;
		}
	}
} */

/* void	exec_single_child(t_mini *mini)
{
	execve
} */