/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 21:12:48 by mrosario          #+#    #+#             */
/*   Updated: 2021/06/26 15:39:52 by miki             ###   ########.fr       */
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
#define MALLOC_ERR 1
#define ARG_NUM_ERR 2
#define ARG_SYN_ERR 3
#define PTHREAD_CREAT_ERR 4
#define PTHREAD_MUTEX_INIT_ERR 5
#define DEATH_TOOK_TOO_LONG 6

/*
** Exit Functions
*/

void	exit_program(t_progdata *progdata, int exit_status);
void	iamerror(int error_code, char *error_func, t_progdata *progdata);

/*
** Unit Tests
*/

void	get_args_utest(int argc, char **argv, t_progdata *progdata);

/*
** Initialization
*/

t_philosopher	*philo_init(int number_of_philosophers, t_progdata *progdata);
pthread_t		*thread_init(int number_of_philosophers, t_progdata *progdata);
pthread_mutex_t	*fork_init(int number_of_forks, t_progdata *progdata);

/*
** Life Cycle
*/

void	*life_cycle(void *progdata);

#endif