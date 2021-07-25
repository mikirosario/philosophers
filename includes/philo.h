/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 21:12:48 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/25 17:19:34 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "philo_lib.h"
# include <stdio.h>
# define MALLOC_ERR 1
# define ARG_NUM_ERR 2
# define ARG_SYN_ERR 3
# define PTHREAD_CREAT_ERR 4
# define PTHREAD_MUTEX_INIT_ERR 5
# define NO_PHILOS 6

/*
** Error Functions
*/

int		iamerror(int error_code, char *error_func);

/*
** Initialization
*/

int		philo_init(int number_of_philosophers, t_progdata *progdata);
int		thread_init(int number_of_philosophers, t_progdata *progdata);
int		fork_init(int number_of_forks, t_progdata *progdata);
int		setup(t_progdata *progdata, int argc, char **argv);

/*
** Life Cycle
*/

int		identify_self(t_progdata *progdata);
void	identify_forks(int id, t_progdata *progdata);
void	inform(char *msg, int id, t_progdata *progdata);
char	is_dead(t_progdata *progdata, long long unsigned int last_meal, \
		int id);
char	is_full(t_progdata *progdata, int id);
void	unlock_forks(int fork1, int fork2, int id, t_progdata *progdata);
void	*life_cycle(void *progdata);

#endif