/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:31:51 by miki              #+#    #+#             */
/*   Updated: 2021/07/02 15:26:25 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function initializes the waiters.
**
** Waiters are mutexes and there are as many waiters as philosophers. Each
** waiter watches over the philosopher equal to their array position. For
** example, waiter[pos] watches over philosopher[pos], etc.
**
** Waiters control access to the forks to the left and right of each
** philosopher, that is, fork[pos] and fork[pos + 1]. Each fork will have as
** many as two waiters assigned. Both waiters must give their permission to
** the dining philosopher for them to pick up their forks and begin eating.
**
** If a neighbouring philosopher is dining, the thinking philosopher will
** need to wait for that neighbour's waiter to become available again before
** taking a fork. This ensures all philosophers eat, as that recently fed
** philosopher then has to wait for its neighbour's waiter, etc.
*/

int	waiter_init(int number_of_philosophers, t_progdata *progdata)
{
	int	i;

	i = 0;
	progdata->waiter = malloc(number_of_philosophers * sizeof(pthread_mutex_t));
	if (progdata->waiter == NULL)
		return (iamerror(MALLOC_ERR, "waiter_init"));
	while (i < number_of_philosophers)
		if (pthread_mutex_init(&progdata->waiter[i++], NULL))
			return (iamerror(PTHREAD_MUTEX_INIT_ERR, "waiter_init"));
	return (1);
}

/*
** This function initializes fork-related variables. Forks as in the eating
** utensil used by the philosophers, not the process spawning function. xD
**
** Forks are mutexes and there are as many as there are philosophers, and they
** are considered to be located to the left and right of each philosopher, with
** the philosophers seated in a circular arrangement.
**
** We will represent this as an array of N forks corresponding to an array of
** N philosophers, where fork[id] corresponds to the fork to the left of
** philosopher[id] and fork[id + 1] corresponds to the fork to the right of
** philosopher[id], and fork[N] wraps around to fork[0].
**
** So this function takes as its arguments the number_of_forks and a pointer to
** the progdata struct. First we malloc enough memory for an array of N mutexes.
** If this goes well we initialize each mutex in the array and simultaneously
** use the variable in the progdata struct that records the number_of_forks as a
** counter so it will also be set to the proper value after initialization. If
** any of the mutex_init calls fails, then the progdata->number_of_forks
** variable will record the position of the last initialized mutex in the array,
** which can then be used to destroy only the initialized mutexes.
**
** If any initialization fails, 0 is returned and an appropriate error message
** is displayed. Otherwise, 1 is returned.
**
*/

int	fork_init(int number_of_forks, t_progdata *progdata)
{
	int				*nforks;

	nforks = &progdata->number_of_forks;
	progdata->forks = malloc(number_of_forks * sizeof(pthread_mutex_t));
	if (progdata->forks == NULL)
		return (iamerror(MALLOC_ERR, "fork_init"));
	while (*nforks < number_of_forks)
		if (pthread_mutex_init(&progdata->forks[(*nforks)++], NULL))
			return (iamerror(PTHREAD_MUTEX_INIT_ERR, "fork_init"));
	return (1);
}

/*
** This function reserves memory for an array of number_of_philosophers (N)
** thread pointers, to set up the threads that will function as the
** 'philosophers'.
**
** If successful, the threads are created one by one and their identifiers are
** pointed to by the array. Threads are destroyed on program exit. A timestamp
** is also collected at simulation start, just before the first thread is
** spawned, to serve as the 0 time.
**
** If any initialization fails, an appropriate error message is displayed and 0
** is returned. Otherwise, 1 is returned.
*/

int	thread_init(int number_of_philosophers, t_progdata *progdata)
{
	size_t		i;

	progdata->thread = malloc(number_of_philosophers * sizeof(pthread_t *));
	if (progdata->thread == NULL)
		return (iamerror(MALLOC_ERR, "thread_init"));
	i = 0;
	progdata->time_start = pl_get_time_msec();
	while (i < (size_t)number_of_philosophers)
		if (pthread_create(&progdata->thread[i++], NULL, life_cycle, progdata))
			return (iamerror(PTHREAD_CREAT_ERR, "thread_init"));
	return (1);
}

/*
** This function reserves memory for an array of t_philosopher structs
** corresponding to each philosopher, which will be used to store the particular
** status of each philosopher. They are zeroed upon creation.
**
** This function will also initialize three additional mutexes used by the
** project, idlock, waiter and printlock. The time_to_eat and time_to_sleep
** variables passed by the user are transformed to microseconds for use with the
** usleep function.
**
** If any of the initializations fail, an error message is displayed and 0 is
** returned. Otherwise, 1 is returned.
*/

int	philo_init(int number_of_philosophers, t_progdata *progdata)
{
	size_t	i;

	progdata->philosopher = malloc(number_of_philosophers * sizeof(t_philosopher));
	if (progdata->philosopher == NULL)
		return (iamerror(MALLOC_ERR, "philo_init"));
	i = 0;
	while (i < (size_t)number_of_philosophers)
		pl_bzero(&progdata->philosopher[i++], sizeof(t_philosopher));
	if (pthread_mutex_init(&progdata->idlock, NULL) || /* pthread_mutex_init(&progdata->waiter, NULL) || */ pthread_mutex_init(&progdata->printlock, NULL))
		return (iamerror(PTHREAD_MUTEX_INIT_ERR, "philo_init"));
	progdata->usec_time_to_eat = progdata->time_to_eat * 1000;
	progdata->usec_time_to_sleep = progdata->time_to_sleep * 1000;
	progdata->usec_time_to_die = progdata->time_to_die * 1000;
	return (1);
}