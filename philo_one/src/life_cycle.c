/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/06/25 23:32:18 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	check_in(void)
{
	
}

void	*life_cycle(void *progdata)
{
	static int		uid = 0;
	int				id;
	struct timeval	last_meal;

	gettimeofday(&last_meal, NULL);
	pthread_mutex_lock(&((t_progdata *)progdata)->idlock);
	id = uid++;
	pthread_mutex_unlock(&((t_progdata *)progdata)->idlock);
	while(((t_progdata *)progdata)->philosopher[id].died == 0)
	{
		//get forks

		pthread_mutex_lock(&((t_progdata *)progdata)->forks[id])
		//mierda... esto era lo del dilema... tengo que volver a leer
		//soluciones a dining philosophers... xD

		//eat OR STARVE TO DEATH

		//put down forks

		//sleep

	}

	printf("I am: %d\n", id);
	return (NULL);
}