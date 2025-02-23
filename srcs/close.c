#include "../incl/minishell.h"

void	close_fd(int fd)
{
	if (fd != -1 && fd > 2)
	{
		close(fd);
		fd = -1;
	}
}

void	close_fds(int *fd)
{
	if (fd[1] != -1  && fd[1] > 2)
	{
		close(fd[1]);
		fd[1] = -1;
	}
	if (fd[0] != -1  && fd[0] > 2)
	{
		close(fd[0]);
		fd[0] = -1;
	}
}
