/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/06/26 16:05:15 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	inform(char *msg, int id)
{
	printf("%llu %d"" %s\n", pl_get_time_msec(), id + 1, msg);
}

void	*life_cycle(void *progdata)
{
	static int				uid = 0;
	int						id;
	long long unsigned int	last_meal;
	long long unsigned int	diff;
	int						fork1;
	int						fork2;
	//struct timeval	current;

	last_meal = pl_get_time_msec();
	pthread_mutex_lock(&((t_progdata *)progdata)->idlock);
	id = uid++;
	pthread_mutex_unlock(&((t_progdata *)progdata)->idlock);
	fork1 = id;
	if (id < ((t_progdata *)progdata)->number_of_philosophers - 1)
		fork2 = id + 1;
	else
		fork2 = 0;
	while(1)
	{
		//think
		inform("is thinking", id);
		pthread_mutex_lock(&((t_progdata *)progdata)->waiter);
		//take forks
		pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork1]);
		inform("has taken a fork", id);
		pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork2]);
		inform("has taken a fork", id);
		pthread_mutex_unlock(&((t_progdata *)progdata)->waiter);
		//eat
		inform("is eating", id);
		usleep(((t_progdata *)progdata)->usec_time_to_eat);
		diff = (pl_get_time_msec() - last_meal);
		if (diff > (long long unsigned int)((t_progdata *)progdata)->time_to_die)
		{
			//die
			sleep(1);
			inform("died", id);
			if (last_meal + diff - pl_get_time_msec() > 10)
				iamerror(DEATH_TOOK_TOO_LONG, "life_cycle", progdata);
			exit_program(progdata, EXIT_SUCCESS);
		}
		last_meal += diff;
		pthread_mutex_unlock(&((t_progdata *)progdata)->forks[fork2]);
		pthread_mutex_unlock(&((t_progdata *)progdata)->forks[fork1]);
		//sleep
		inform("is sleeping", id);
		usleep(((t_progdata *)progdata)->usec_time_to_sleep);
		
		
		
		//mierda... esto era lo del dilema... tengo que volver a leer
		//soluciones a dining philosophers... xD

		//eat OR STARVE TO DEATH

		//put down forks

		//sleep

	}
	return (NULL);
}