/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:06:34 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/09 14:39:49 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	get_input(void)
{
	char	*read_line;
	
	read_line = readline("mini> ");
	if (!read_line)
		exit(0);
}

int	main(int ac, char **av, char **env)
{
	((void)ac, (void)av, (void)env);
	get_input();
	return (0);
}
