#include "../incl/minishell.h"

// frees a 2d char array (e.g. the copy of env)
void	free_arr(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
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
// frees a char * if not NULL and sets it to NULL
void	free_str(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
// frees a * of any type if not NULL and sets it to NULL
// returns FAIL
int	free_ptr_fail(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (FAIL);
}

// frees a pointer to char ** and sets it to NULL
void	free_cmds_tbl(char ***cmds_tbl)
{
	size_t	i;

	if (!cmds_tbl)
		return;

	i = 0;
	while (cmds_tbl[i])
	{
		free_arr(cmds_tbl[i]);
		i++;
	}
	if (cmds_tbl)
	{
		free(cmds_tbl);
		cmds_tbl = NULL;
	}
}
