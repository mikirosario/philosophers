/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:31:51 by miki              #+#    #+#             */
/*   Updated: 2021/06/25 23:24:05 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

pthread_mutex_t		*fork_init(int number_of_forks, t_progdata *progdata)
{
	int				*nforks;
	pthread_mutex_t	*forks;

	nforks = &progdata->number_of_forks;
	forks = malloc(number_of_forks * sizeof(pthread_mutex_t));
	if (forks == NULL)
		iamerror(MALLOC_ERR, "fork_init", progdata);
	while (*nforks < number_of_forks)
		if (pthread_mutex_init(&forks[(*nforks)++], NULL))
			iamerror(PTHREAD_MUTEX_INIT_ERR, "fork_init", progdata);
	return (forks);
}

pthread_t	*thread_init(int number_of_philosophers, t_progdata *progdata)
{
	size_t		i;
	pthread_t	*threads;

	threads = malloc(number_of_philosophers * sizeof(pthread_t *));
	
	if (threads == NULL)
		iamerror(MALLOC_ERR, "thread_init", progdata);
	i = 0;
	while (i < (size_t)number_of_philosophers)
		if (pthread_create(&threads[i++], NULL, life_cycle, progdata))
			iamerror(PTHREAD_CREAT_ERR, "thread_init", progdata);
	if (pthread_mutex_init(&progdata->idlock, NULL))
		iamerror(PTHREAD_MUTEX_INIT_ERR, "thread_init", progdata);
	return (threads);
}


t_philosopher	*philo_init(int number_of_philosophers, t_progdata *progdata)
{
	size_t			i;
	t_philosopher	*philosophers;

	philosophers = malloc(number_of_philosophers * sizeof(t_philosopher));

	if (philosophers == NULL)
		iamerror(MALLOC_ERR, "philo_init", progdata);
	i = 0;
	while (i < (size_t)number_of_philosophers)
		pl_bzero(&philosophers[i++], sizeof(t_philosopher));
	return (philosophers);
}