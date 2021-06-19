/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 21:12:48 by mrosario          #+#    #+#             */
/*   Updated: 2021/06/19 18:25:18 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include "philo_lib.h"
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

/*
** Exit Functions
*/

void	exit_failure(char *error_msg, t_progdata *progdata);

/*
** Unit Tests
*/

void	get_args_utest(int argc, char **argv, t_progdata *progdata);

#endif