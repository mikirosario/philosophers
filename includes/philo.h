/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 21:12:48 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/02 14:57:45 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "philo_lib.h"
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# define MALLOC_ERR 1
# define ARG_NUM_ERR 2
# define ARG_SYN_ERR 3
# define PTHREAD_CREAT_ERR 4
# define PTHREAD_MUTEX_INIT_ERR 5

/*
** Exit Functions
*/

void	exit_program(t_progdata *progdata, int exit_status);
int		iamerror(int error_code, char *error_func);

/*
** Unit Tests
*/

void	get_args_utest(int argc, char **argv, t_progdata *progdata);

/*
** Initialization
*/

int	philo_init(int number_of_philosophers, t_progdata *progdata);
int	waiter_init(int number_of_philosophers, t_progdata *progdata);
int	thread_init(int number_of_philosophers, t_progdata *progdata);
int	fork_init(int number_of_forks, t_progdata *progdata);
int	setup(t_progdata *progdata, int argc, char **argv);

/*
** Life Cycle
*/

char	is_full(t_progdata *progdata, int id);
void	*life_cycle(void *progdata);

#endif