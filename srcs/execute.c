#include "../incl/minishell.h"

void	exec_no_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;

	if (builtin_only(mini, &mini->cmds[0]))
	{
		handle_builtin(mini, 0);
		return ;
	}
	pid = fork();
	if (check_pid(pid) == 0)
	{
		handle_redirs(mini, &mini->cmds[0]);
		if (is_there_type(mini, BUILTIN))
			handle_builtin(mini, 0);
		exec_command(mini);
	}
	wait_single(mini, pid, &status);
}

void	child_process(t_mini *mini, t_cmd *cmd, int	*fd, int i)
{
	if (cmd->in_file > 2)
	{
		dup2(cmd->in_file, STDIN);
		close_fd(cmd->in_file);
	}
	if (i < mini->pipes - 1)
	{
		dup2(fd[1], STDOUT);
		close_fds(fd);
	}
	handle_redirs(mini, &mini->cmds[i]);
	if (is_there_type(mini, BUILTIN))
		handle_builtin(mini, i);
	exec_command(mini);
}

void	exec_funct(t_mini *mini, pid_t pid, int *fd, int i)
{
	if (i < mini->pipes - 1 && pipe(fd) == -1)
		ft_putendl_fd("mini: pipe failed", 2);
	pid = fork();
	if (check_pid(pid) == 0)
		child_process(mini, &mini->cmds[i], fd, i);
	if (mini->cmds[i]->in_file != STDIN)
		close_fd(mini->cmds[i]->in_file);
	if (i < mini->pipes - 1)
		close_fd(fd[1]);
	mini->cmds[i]->in_file = fd[0];
}

void	exec_with_pipes(t_mini *mini)
{
	pid_t	pid;
	int		status;
	int		fd[2];
	int		i;

	i = 0;
	while (i < mini->pipes && mini->cmds[i])
	{
		exec_funct(&mini->cmds[i], pid, fd[2], i);
		i++;
	}
	wait_multi(mini, pid, &status);
}

void	execute(t_mini *mini)
{
	if (mini->cmds_tbl)
	{
		if (cmd_table_size(mini) == 1)
			exec_no_pipes(mini);
		else if (cmd_table_size(mini) > 1);
			exec_with_pipes(mini);
	}
	free_cmds_tbl(mini->cmds_tbl);
}

/* void	execute_command(t_mini *mini)
{
	while (mini->cmds[i])
	{
		exec()
	}

}

void	exec_with_pipes(t_mini *mini)
{
	int	fd[2];

	
} */

/* void	handle_redirs(t_mini *mini, t_cmd *cmd)
{
	if (cmd->in_file != -1 || cmd->out_file != -1)
	{
		if (dup2(cmd->in_file, STDIN) < 0)
			printf("mini : dup error\n");
		if (dup2(cmd->out_file, STDOUT) < 0)
			printf("mini : dup error\n");
		execve(mini->cmds_tbl[0][0], );
	} */


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