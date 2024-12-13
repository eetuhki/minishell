#include "../incl/minishell.h"

// frees a 2d char array (e.g. the copy of env)
void	free_arr(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

// frees a * of any type if not NULL and sets it to NULL
void	free_ptr(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}
