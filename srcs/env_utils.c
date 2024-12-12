/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:03:43 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 13:07:48 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

// returns the values for a given env variable, returns NULL if fail
char	*env_get_var(t_mini *mini, char *var)
{
	int		i;
	char	*path;

	if (!mini->env || !var)
		return (NULL);
	path = ft_strjoin(var, "=");
	if (!path)
		return (NULL);
	i = 0;
	while (mini->env[i])
	{
		if (!(ft_strncmp(path, mini->env[i], ft_strlen(path))))
		{
			free_ptr(path);
			return (ft_strchr(mini->env[i], '=') + 1);
		}
		i++;
	}
	free_ptr(path);
	return (NULL);
}

/* // modifies an env variable, return: SUCCESS or FAIL
int	env_set_var(t_mini *mini, char *env_var, char *new_val)
{


} */