/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsolomon <fsolomon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:52:49 by fsolomon          #+#    #+#             */
/*   Updated: 2024/12/12 16:36:41 by fsolomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_mini *mini, int fd)
{
	char	**env;
	
	env = mini->env;
	while (*env)
	{
		ft_putstr_fd(*env++, fd);
		ft_putstr_fd("\n", fd);
	}
}

void	ft_pwd(int fd)
{
	char	*curr_dir;

	curr_dir = NULL;
	curr_dir = getcwd(curr_dir, 0);
	if (curr_dir != NULL)
	{
		ft_putstr_fd(curr_dir, fd);
		ft_putchar_fd('\n', fd);
		free(curr_dir);
	}
}
