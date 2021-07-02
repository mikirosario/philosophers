/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/02 20:45:22 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (diff > (long long unsigned int)((t_progdata *)progdata)->time_to_die ||
	(&progdata->philosopher[id])->died)
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

/*
** This function checks whether there is only one philosopher. If there is only
** one philosopher, there is only one fork. Since philosophers need two forks to
** eat, a sole philosopher must die. If fork1 == fork2 then there is only one
** fork, and therefore only one philosopher. In that case, we have the
** philosopher sleep for time_to_die milliseconds, register its death, and
** unlock its mutexes before returning 1. Otherwise, we return 0. I add 1ms to
** time_to_sleep, otherwise sometimes the is_dead instructions are executed so
** quickly that the philosopher survives and the program hangs waiting for its
** termination.
*/

char	one_philosopher(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (fork1 != fork2)
		return (0);
	usleep(progdata->usec_time_to_die + 1000);
	is_dead(progdata, last_meal, id);
	pthread_mutex_unlock(&progdata->forks[fork1]);
	pthread_mutex_unlock(&progdata->waiter[fork1]);
	return (1);
}

void	unlock_forks(int fork1, int fork2, t_progdata *progdata)
{
		if (pthread_mutex_unlock(&progdata->forks[fork2]))
			printf("Failure in pthread_mutex_unlock call on waiter[%d] in unlock_forks\n", fork2);
		if (pthread_mutex_unlock(&progdata->forks[fork1]))
			printf("Failure in pthread_mutex_unlock call on waiter[%d] in unlock_forks\n", fork1);
}

int	identify_self(t_progdata *progdata)
{
	static int	uid = 0;
	int			id;

	pthread_mutex_lock(&progdata->idlock);
	id = uid++;
	pthread_mutex_unlock(&progdata->idlock);
	return (id);
}

static void	initialize(int id, t_progdata *progdata)
{
	(progdata->philosopher[id]).fork1 = id;
	if (id < (progdata->number_of_philosophers - 1))
		(progdata->philosopher[id]).fork2 = id + 1;
	else
		(progdata->philosopher[id]).fork2 = 0;
}


char	think(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (is_dead(progdata, last_meal, id))
		return(0);
	inform(CYN"is thinking"RESET, id, progdata);
	pthread_mutex_lock(&((t_progdata *)progdata)->waiter[fork1]);
	if (one_philosopher(id, last_meal, progdata))
		return (0);
	pthread_mutex_lock(&((t_progdata *)progdata)->waiter[fork2]);
	//take forks
	pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork1]);
	inform("has taken a fork", id, progdata);
	pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork2]);
	inform("has taken a fork", id, progdata);
	pthread_mutex_unlock(&((t_progdata *)progdata)->waiter[fork2]);
	pthread_mutex_unlock(&((t_progdata *)progdata)->waiter[fork1]);
	return (1);
}

char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (is_full(progdata, id) || is_dead(progdata, last_meal, id))
		{
			unlock_forks(fork1, fork2, progdata);
			return (0);
		}
		inform(YEL"is eating"RESET, id, progdata);
		if (progdata->argc == 6)
			progdata->philosopher[id].times_ate++;
		usleep(((t_progdata *)progdata)->usec_time_to_eat);
		unlock_forks(fork1, fork2, progdata);
	return (1);
}

void	*life_cycle(void *progdata)
{
	int			id;
	t_progdata	*pdata;			
	long long unsigned int	last_meal;
	int						fork1;
	int						fork2;
	//struct timeval	current;
	pdata = ((t_progdata *)progdata);
	last_meal = pl_get_time_msec() - ((t_progdata *)progdata)->time_start;
	id = identify_self(progdata);
	initialize(id, progdata);
	fork1 = (pdata->philosopher[id]).fork1;
	fork2 = (pdata->philosopher[id]).fork2;
	while(1)
	{
		//think
		// if (is_dead(progdata, &last_meal, id))
		// 	break ;
		// inform(CYN"is thinking"RESET, id, progdata);
		// pthread_mutex_lock(&((t_progdata *)progdata)->waiter[fork1]);
		// if (one_philosopher(fork1, fork2, id, last_meal, progdata))
		// 	break ;
		// pthread_mutex_lock(&((t_progdata *)progdata)->waiter[fork2]);
		// //take forks
		// pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork1]);
		// inform("has taken a fork", id, progdata);
		// pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork2]);
		// inform("has taken a fork", id, progdata);
		// pthread_mutex_unlock(&((t_progdata *)progdata)->waiter[fork2]);
		// pthread_mutex_unlock(&((t_progdata *)progdata)->waiter[fork1]);
		if (!think(id, &last_meal, progdata) || !eat(id, &last_meal, progdata))
			break ;
		//eat
		// if (is_full(progdata, id) || is_dead(progdata, &last_meal, id))
		// {
		// 	unlock_forks(fork1, fork2, progdata);
		// 	break ;
		// }
		// inform(YEL"is eating"RESET, id, progdata);
		// if (((t_progdata *)progdata)->argc == 6)
		// 	(&((t_progdata *)progdata)->philosopher[id])->times_ate++;
		// usleep(((t_progdata *)progdata)->usec_time_to_eat);
		// unlock_forks(fork1, fork2, progdata);
		//sleep
		if (is_full(progdata, id) || is_dead(progdata, &last_meal, id))
			break ;
		inform("is sleeping", id,  progdata);
		usleep(((t_progdata *)progdata)->usec_time_to_sleep);
	}
	return (NULL);
}
