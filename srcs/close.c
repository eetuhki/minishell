#include "../incl/minishell.h"

void	close_fd(int *fd)
{
	if (*fd != -1 && *fd > 2)
	{
		close(*fd);
		*fd = -1;
	}
}
