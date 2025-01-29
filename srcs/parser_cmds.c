#include "../incl/minishell.h"

int	parse_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds[i])
	{
		if (parse_individual_cmd(mini, mini->cmds[i]) == FAIL)
		{
			mini->exit_code = 1;
			// free cmd structs
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}
