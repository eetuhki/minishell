#include "../incl/minishell.h"

int	env_shlvl(t_mini *mini)
{
	char	*shlvl;
	int		new_lvl;

	shlvl = env_get_var(mini, "SHLVL");
	printf("\nenv_shlvl: shlvl: %s\n", shlvl);
	if (!shlvl)
		return (env_set_var(mini, "SHLVL", "1"));
	new_lvl = ft_atoi(shlvl) + 1;
	if (new_lvl > 999)
		new_lvl = 1;
	shlvl = ft_itoa(new_lvl);
	if (!shlvl)
		return (FAIL);
	if (!env_set_var(mini, "SHLVL", shlvl))
	{
		free_ptr(shlvl);
		return (FAIL);
	}
	free_ptr(shlvl);
	return (SUCCESS);
}

