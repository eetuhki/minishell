/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:43:14 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/11 15:18:05 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

size_t	env_count_variables(char **env)
{
	size_t	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

int	env_copy(t_mini *mini, char **env)
{
	size_t	i;
	size_t	env_count;

	env_count = env_count_variables(env) + 1;
	mini->env = ft_calloc(env_count, (sizeof * mini->env));
	if (!mini->env)
		return (FAIL);
	i = -1;
	while (env[++i])
	{
		mini->env = ft_strdup(env[i]);
		if (!mini->env[i])
			return (FAIL);
	}
	return (SUCCESS);
}
