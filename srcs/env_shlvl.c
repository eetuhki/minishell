#include "../incl/minishell.h"

int	env_shlvl(t_mini *mini)
{
	char	*shlvl;
	int		new_lvl;

	shlvl = env_get_var(mini, "SHLVL");
	printf("env_shlvl: shlvl: %s\n", shlvl);
	if (!shlvl)
		return (env_set_var(mini, "SHLVL", "1"));
	new_lvl = ft_atoi(shlvl) + 1;
	printf("env_shlvl: new_lvl: %d\n", new_lvl);
	if (new_lvl >= INT_MAX)
		new_lvl = 1;
	shlvl = ft_itoa(new_lvl);
	if (!shlvl)
		return (FAIL);
	if (env_set_var(mini, "SHLVL", shlvl) == FAIL)
	{
		free_ptr(shlvl);
		return (FAIL);
	}
	printf("env_shlvl: updated shlvl: %s\n", shlvl);
	free_ptr(shlvl);
	return (SUCCESS);
}

