#include "../incl/minishell.h"

void	sig_handler_wait(int sig)
{
	if (sig == SIGINT)
		printf("\n");
}

int	wait_single(t_mini *mini, pid_t pid, int *status)
{
	signal(SIGINT, sig_handler_wait);
	waitpid(pid, status, 0);
	mini->exit_code = WEXITSTATUS(*status);
	//printf("EXIT CODE at SINGLE mini->exitcode = [%d]\n ", mini->exit_code);
	sig_init();
	return (SUCCESS);
}

int	wait_multi(t_mini *mini)
{
    int	i;
    int	status;
    int	last_exit = 0;

    i = 0;
	status = 0;
	signal(SIGINT, sig_handler_wait);
    while (i <= mini->pipes)
    {
        waitpid(mini->pids[i], &status, 0);
        last_exit = WEXITSTATUS(status); // Store only the last command's exit status
        i++;
    }
    mini->exit_code = last_exit;
	//printf("EXIT CODE at MULTI last_exit=[%d] and mini->exitcode = [%d]\n ", last_exit, mini->exit_code);
	sig_init();
    return (SUCCESS);
}

// int	wait_multi(t_mini *mini)
// {
// 	//int	i;
// 	int	status;
// 	int	child_pid;

// 	//i = 0;
// 	child_pid = wait(&status);
// 	while (child_pid > 0 /* i < mini->pipes*/)
// 	{
// 		if (WIFEXITED(status))
// 		{
// 			mini->exit_code = WEXITSTATUS(status);
// 		}
// 		child_pid = wait(&status);
// 		//waitpid(mini->pids[i], &status, 0);
// 		//mini->exit_code = WEXITSTATUS(status);
// 		//i++;
// 	}
// 	return (SUCCESS);
// }
/* static int	wait_for_child_processes(t_pinfo *pi, int pid2)
{
	int	child_pid;
	int	status;
	int	last_cmd_exit_code;
	int	exit_code;

	last_cmd_exit_code = 0;
	child_pid = wait(&status);
	while (child_pid > 0)
	{
		if (WIFEXITED(status))
		{
			exit_code = WEXITSTATUS(status);
			if (child_pid == pid2)
				last_cmd_exit_code = exit_code;
			if (exit_code == 126 || exit_code == 127)
				handle_exit_error(pi, child_pid, pid2, exit_code);
		}
		child_pid = wait(&status);
	}
	return (last_cmd_exit_code);
} */
