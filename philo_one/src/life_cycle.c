/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/06/27 05:43:38 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	inform(char *msg, int id, t_progdata *progdata)
{
	pthread_mutex_lock(&progdata->printlock);
	printf("%llu %d"" %s\n", pl_get_time_msec() - progdata->time_start, id + 1, msg);
	pthread_mutex_unlock(&progdata->printlock);
}

char	is_full(t_progdata *progdata, int id)
{
	if (progdata->argc == 6 && (&progdata->philosopher[id])->times_ate == \
	progdata->number_of_times_each_philosopher_must_eat)
			return (1);
	return (0);
}

char	full_check(t_progdata *progdata, int id)
{
	if (progdata->argc == 6)
	{
		if (is_full(progdata, id))
			return (1);
		(&progdata->philosopher[id])->times_ate++;
	}
	return (0);
}

char	is_dead(t_progdata *progdata, long long unsigned int *last_meal, int id)
{
	long long unsigned int	diff;
	long long unsigned int	time_of_death;

	diff = (pl_get_time_msec() - ((t_progdata *)progdata)->time_start - *last_meal);
	if (diff > (long long unsigned int)((t_progdata *)progdata)->time_to_die)
	{
		//die
		time_of_death = pl_get_time_msec();
		inform(RED"died"RESET, id, progdata);
		if (pl_get_time_msec() - time_of_death > 10)
			printf(RED"Took more than 10 ms to inform of philosopher death\n"RESET);
		(&progdata->philosopher[id])->died = 1;
	}
	*last_meal += diff;
	return ((&progdata->philosopher[id])->died);
}

void	*life_cycle(void *progdata)
{
	static int				uid = 0;
	int						id;
	long long unsigned int	last_meal;
	int						fork1;
	int						fork2;
	//struct timeval	current;

	last_meal =  pl_get_time_msec() - ((t_progdata *)progdata)->time_start;
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
		if (is_dead(progdata, &last_meal, id))
			break ;
		inform(CYN"is thinking"RESET, id, progdata);
		pthread_mutex_lock(&((t_progdata *)progdata)->waiter);
		//take forks
		pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork1]);
		inform("has taken a fork", id, progdata);
		pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork2]);
		inform("has taken a fork", id, progdata);
		pthread_mutex_unlock(&((t_progdata *)progdata)->waiter);
		//eat
		if (is_full(progdata, id) || is_dead(progdata, &last_meal, id))
		{
			pthread_mutex_unlock(&((t_progdata *)progdata)->forks[fork2]);
			pthread_mutex_unlock(&((t_progdata *)progdata)->forks[fork1]);
			break ;
		}
		inform(YEL"is eating"RESET, id, progdata);
		if (((t_progdata *)progdata)->argc == 6)
			(&((t_progdata *)progdata)->philosopher[id])->times_ate++;
		usleep(((t_progdata *)progdata)->usec_time_to_eat);
		pthread_mutex_unlock(&((t_progdata *)progdata)->forks[fork2]);
		pthread_mutex_unlock(&((t_progdata *)progdata)->forks[fork1]);
		//sleep
		if (is_full(progdata, id) || is_dead(progdata, &last_meal, id))
			break ;
		inform("is sleeping", id,  progdata);
		usleep(((t_progdata *)progdata)->usec_time_to_sleep);
		
		
		
		//mierda... esto era lo del dilema... tengo que volver a leer
		//soluciones a dining philosophers... xD

		//eat OR STARVE TO DEATH

		//put down forks

		//sleep
	}
	return (NULL);
}