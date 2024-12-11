/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:39:32 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/11 13:43:26 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	arg_check(int ac, char **av)
{
	if (ac != 1 || av[1])
	{
		printf("Please run minishell without any additional arguments\n");
		exit(0);
	}
}
