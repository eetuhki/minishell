/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:06:34 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 10:01:54 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	get_input(void)
{
	g_mini->input = readline("mini> ");
	if (!g_mini->input)
		exit(0);
}

int	main(int ac, char **av, char **env)
{
	arg_check(ac, av);
	g_mini = ft_calloc(1, sizeof(t_mini));
	if (!g_mini)
		return (1);
	env_copy(g_mini, env);
	get_input();
	return (0);
}
