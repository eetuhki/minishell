/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsolomon <fsolomon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:06:34 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 12:34:19 by fsolomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	get_input(t_mini *mini)
{
	mini->input = readline("mini> ");
	if (!mini->input)
		exit(0);
}
void	init_mini(t_mini *mini)
{
	mini->env = NULL;
	mini->input = NULL;
}

int	main(int ac, char **av, char **env)
{
	t_mini	*mini;
	arg_check(ac, av);
	mini = ft_calloc(1, sizeof(t_mini));
	if (!mini)
		return (1);
	init_mini(mini);
	env_copy(mini, env);
	get_input(mini);
	return (0);
}
