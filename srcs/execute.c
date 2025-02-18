#include "../incl/minishell.h"

void	exec_command(t_mini *mini, char **cmds)
{
	printf("\nTHE cmd here in exec stage %s\n", cmds[0]);
	if (!cmds || !cmds[0])
		exit(1);
	ft_putstr_fd("commant to exec:", 2);
	ft_putstr_fd(cmds[0], 2);
	ft_putstr_fd("\n", 2);
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

	printf("cmds[0][0]:%s\n", mini->cmds_tbl[0][0]);
	if (builtin_only(mini->cmds[0]))
	{
		handle_builtin(mini, 0);
		return ;
	}
	pid = fork();
	if (check_pid(pid) == 0)
	{
		handle_redirs(mini->cmds[0]);
		exec_command(mini, mini->cmds_tbl[0]);
	}
	wait_single(mini, pid, &status);
}

void	child_process(t_mini *mini, int	*fd, int i)
{
	if (i < mini->pipes)
	{
		dup2(fd[1], STDOUT);
		close(fd[1]);
	}
	if (i > 0)
	{
        dup2(fd[0], STDIN);   // Redirect stdin to previous pipe read end
    }
	close(fd[0]);
	handle_redirs(mini->cmds[i]);
	if (is_there_type(mini, BUILTIN, i))
		handle_builtin(mini, i);
	else
		exec_command(mini, mini->cmds_tbl[i]);
}

void	exec_funct(t_mini *mini, int *fd, int i)
{
	pid_t	pid;

	//mini->pids = malloc(sizeof(pid_t) * mini->pipes);
	//if (!mini->pids)
	//	return ;
	pid = fork();
	if (check_pid(pid) == 0)
	{

		//dup2(prev_fd, STDIN);
			//close_fd(prev_fd);
		child_process(mini, fd, i);
	}
	mini->pids[i] = pid;
	close(fd[1]);
	dup2(fd[0], STDIN);
	close(fd[0]);
	wait_multi(mini);
	/* if (prev_fd != STDIN)
		close_fd(prev_fd); */
	/* if (i < mini->pipes - 1)
	{
		close_fd(fd[1]);
		prev_fd = fd[0];
	} */
	// while (wait(&status) > 0)
	//	;
}

/* void	exec_with_pipes(t_mini *mini)
{
	int		fd[2];
	//int		prev_fd;
	int		i;

	i = 0;
	//prev_fd = STDIN;
	mini->pids = ft_calloc((size_t)mini->pipes, sizeof(pid_t));
	if (!mini->pids)
		return ;
	printf("PIPES COUNT = [%d] \n", mini->pipes);
	while (i <= mini->pipes && mini->cmds[i])
	{
		if (pipe(fd) == -1)
			ft_putendl_fd("mini: pipe failed", 2);
		exec_funct(mini, fd, i);
		//prev_fd = fd[0];
		i++;
	}
	// if (prev_fd != STDIN)
	//	close_fd(prev_fd);
} */
void	exec_with_pipes(t_mini *mini)
{
    int	fd[2];
    int	i = 0;
    int	in_fd = STDIN;  // Track input for the next command

    mini->pids = ft_calloc((size_t)(mini->pipes + 1), sizeof(pid_t));
    if (!mini->pids)
        return;

    while (i <= mini->pipes && mini->cmds[i])
    {
        if (i < mini->pipes)  // Create a new pipe for all except last command
        {
            if (pipe(fd) == -1)
            {
                perror("mini: pipe failed");
                return;
            }
        }

        pid_t pid = fork();
        if (pid == 0)  // Child process
        {
            if (in_fd != STDIN) // If not first command, use previous pipe as stdin
            {
                dup2(in_fd, STDIN);
                close_fd(in_fd);
            }

            if (i < mini->pipes)  // If it's NOT the last command, redirect stdout to pipe
            {
                dup2(fd[1], STDOUT);
            }

            close_fd(fd[0]); // Always close read end of pipe in child
            close_fd(fd[1]); // Always close write end in child

            handle_redirs(mini->cmds[i]); // Handle file redirections

            if (is_there_type(mini, BUILTIN, i))
                handle_builtin(mini, i);
            else
                exec_command(mini, mini->cmds_tbl[i]);

            exit(1); // Safety exit if execution fails
        }

        // Parent process
        mini->pids[i] = pid;

        close_fd(fd[1]); // Parent closes write end immediately

        if (in_fd != STDIN) // Close the old read end
        {
            close_fd(in_fd);
        }

        in_fd = fd[0]; // Store the read end for the next iteration

        i++;
    }

    close_fd(in_fd); // Close last read end after loop
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
