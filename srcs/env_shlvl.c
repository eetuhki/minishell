#include "../incl/minishell.h"

static void	shlvl_too_high(int lvl)
{
	ft_putstr_fd("mini: warning: shell level (", 2);
	ft_putnbr_fd(lvl, 2);
	ft_putendl_fd(") too high, resetting to 1", 2);
}

int	env_shlvl(t_mini *mini)
{
	char	*shlvl;
	int		new_lvl;

	shlvl = env_get_var(mini, "SHLVL");
	if (!shlvl)
		return (env_set_var(mini, "SHLVL", "1"));
	new_lvl = ft_atoi(shlvl) + 1;
	if (new_lvl > 999)
	{
		shlvl_too_high(new_lvl);
		new_lvl = 1;
	}
	shlvl = ft_itoa(new_lvl);
	if (!shlvl)
		return (FAIL);
	if (env_set_var(mini, "SHLVL", shlvl) == FAIL)
	{
		free_ptr(shlvl);
		return (FAIL);
	}
	free_ptr(shlvl);
	return (SUCCESS);
}
