/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:10:58 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/12 13:15:15 by eelaine          ###   ########.fr       */
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

// env_shlvl.c
int		env_shlvl(t_mini *mini);

// env_utils.c
char	*env_get_var(t_mini *mini, char *var);

// free.c
void	free_arr(char **arr);
void	ft_env(t_mini *mini);
#endif