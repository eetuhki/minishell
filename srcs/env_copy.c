/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsolomon <fsolomon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:43:14 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 12:21:28 by fsolomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

// counts the env variable lines
size_t	env_count_variables(char **env)
{
	size_t	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

// copies the env into a 2d array for later use and frees the array if copy fails
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
		mini->env[i] = ft_strdup(env[i]);
		if (!mini->env[i])
		{
			free_arr(mini->env);
			return (FAIL);
		}
	}
	return (SUCCESS);
}
