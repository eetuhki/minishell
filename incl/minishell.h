/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsolomon <fsolomon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:10:58 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 16:27:09 by fsolomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <string.h>
# include <sys/wait.h>

// custom headers & libraries
# include "../libft/incl/libft.h"
# include "macros.h"

typedef enum e_symbol_type
{
	PIPE,
	INFILE,
	OUTFILE,
	AND,
	OR,
	D_QUOTE,
	S_QUOTE,
}	t_symbol_type;

typedef struct s_mini
{
	char	*input;
	char	**env;
}	t_mini;

// arg_check.c
void	arg_check(int ac, char **av);

// env_copy.c
int		env_copy(t_mini *mini, char **env);
size_t	env_count_variables(char **env);

// free.c
void	free_arr(char **arr);

// builtin functions
void	ft_env(t_mini *mini, int fd);
void	ft_pwd(int fd);
#endif