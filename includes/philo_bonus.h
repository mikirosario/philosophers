/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 21:12:48 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/10 13:36:33 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

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
# define FORK_FAILURE 6
# define SEM_OPEN_FAIL 7

/*
** Exit Functions
*/

void	exit_success(t_progdata *progdata);
void	exit_failure(t_progdata *progdata);

/*
** Error Functions
*/

int		iamerror(int error_code, char *error_func);

/*
** Unit Tests
*/

char	get_args_utest(int argc, char **argv, t_progdata *progdata);

/*
** Initialization
*/

int		philo_init(int number_of_philosophers, t_progdata *progdata);
int		waiter_init(int number_of_forks, t_progdata *progdata);
int		proc_init(int number_of_philosophers, t_progdata *progdata);
int		fork_init(int number_of_forks, t_progdata *progdata);
int		setup(t_progdata *progdata, int argc, char **argv);

/*
** Life Cycle
*/

int		identify_self(t_progdata *progdata);
void	identify_forks(int id, t_progdata *progdata);
void	inform(char *msg, int id, t_progdata *progdata);
char	is_dead(t_progdata *progdata, long long unsigned int *last_meal, \
		int id);
char	is_full(t_progdata *progdata, int id);
void	life_cycle(void *progdata);

#endif