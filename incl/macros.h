/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelaine <eelaine@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 10:33:12 by eelaine           #+#    #+#             */
/*   Updated: 2024/12/11 15:00:26 by eelaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

// error status
# define SUCCESS 0
# define FAIL 1
# define CMD_EXEC_FAIL 126
# define CMD_NOT_FOUND 127

// delimiter definitions
# define DELIMITERS " \t|<>'\""
# define WHITE_SPACE " \t"

// file descriptors
# define STDIN 0
# define STDOUT 1
# define STDERR 2

#endif