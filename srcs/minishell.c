/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsolomon <fsolomon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:06:34 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/10 17:21:36 by fsolomon         ###   ########.fr       */
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
	((void)ac, (void)av, (void)env);
	g_mini = ft_calloc(1, sizeof(t_mini));
	if (!g_mini)
		return (1);
	get_input();
	return (0);
}
