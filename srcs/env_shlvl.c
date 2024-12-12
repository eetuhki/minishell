/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_shlvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:56:05 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 13:06:33 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	env_shlvl(t_mini *mini)
{
	char	*shlvl;
	int		new_lvl;

	shlvl = env_get_var(mini->env, "SHLVL");
	printf("env_shlvl: shlvl: %s", shlvl);
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

