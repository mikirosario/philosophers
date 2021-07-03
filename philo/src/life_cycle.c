/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/04 01:38:50 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pl_usleep(long long unsigned int wait)
{
	struct timeval			timestamp;
	long long unsigned int	time_end;

	gettimeofday(&timestamp, NULL);
	time_end = pl_timeval_to_usec(&timestamp) + wait;
	while (!gettimeofday(&timestamp, NULL) && \
	(pl_timeval_to_usec(&timestamp) < time_end))
		usleep(50);
}

void	inform(char *msg, int id, t_progdata *progdata)
{
	pthread_mutex_lock(&progdata->printlock);
	printf("%llu %d"" %s\n", pl_get_time_msec() - progdata->time_start, \
	id + 1, msg);
	pthread_mutex_unlock(&progdata->printlock);
}

char	is_full(t_progdata *progdata, int id)
{
	if (progdata->argc == 6 && (&progdata->philosopher[id])->times_ate == \
	progdata->number_of_times_each_philosopher_must_eat)
			return (1);
	return (0);
}

char	is_dead(t_progdata *progdata, long long unsigned int *last_meal, int id)
{
	long long unsigned int	diff;
	long long unsigned int	time_of_death;

	diff = pl_get_time_msec() - progdata->time_start - *last_meal;
	if (diff > (long long unsigned int)progdata->time_to_die ||
	(&progdata->philosopher[id])->died)
	{
		//die
		time_of_death = pl_get_time_msec();
		inform(RED"died"RESET, id, progdata);
		if (pl_get_time_msec() - time_of_death > 10)
			printf(RED"Took more than 10 ms to inform of philosopher death\n"RESET);
		(&progdata->philosopher[id])->died = 1;
	}
	if (progdata->philosopher[id].eating)
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
	pl_usleep(progdata->usec_time_to_die + 1000);
	is_dead(progdata, last_meal, id);
	pthread_mutex_unlock(&progdata->forks[fork1]);
	// pthread_mutex_unlock(&progdata->waiter[fork1]);
	pthread_mutex_unlock(&progdata->waiter);
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
	if (id < (progdata->number_of_philosophers - 1))
	{
		(progdata->philosopher[id]).fork1 = id;
		(progdata->philosopher[id]).fork2 = id + 1;
	}
	else
	{
		(progdata->philosopher[id]).fork1 = 0;
		(progdata->philosopher[id]).fork2 = id;
	}
}

/*
** Each philosopher is seated in a circle with a fork to its left and right.
**
** Thinking happens after sleeping. Before thinking we check if a philosopher
** slept too long and is dead. If it is, we return 0. If not, we start thinking.
**
** "Thinking" is basically what a philosopher does before picking up its forks.
** Forks are mutexes. If another philosopher has the mutex, then it can't be
** picked up.
**
** Each philosopher has an ID number corresponding to its position in the
** philosophers array, and a fork to its left and right. It will identify one
** fork as fork1 and another fork as fork2, and will always pick up fork1 first.
**
** Each philosopher identifies its fork1 as its right fork (fork[id]) and its
** fork2 as its left fork as (fork[id + 1]), except the last philosopher, who
** identifies its fork1 as its left fork (fork[0]) and fork2 as its right fork
** (fork[id]).
**
** This prevents deadlock because, even if every philosopher were to take their
** fork1 simultaneously, at least one philosopher would be unable to do it,
** since philosopher[0] and philosopher[n] have the same fork1, and so that
** philosopher would also be unable to take its fork2, and so someone would be
** able to eat.
**
** So the table looks like this, where SOCRATES is philosopher[0], DE BEAUVOIR
** is philosopher[1], ARISTOTLE is philosopher[2] and ZAMBRANO is
** philosopher[3].
**
**							fork1<-|0|->fork2
**					|->fork1 f0	SOCRATES  f1 fork1<----|
**					|ZAMBRANO	3	@	1	DE BEAUVOIR|
**					|->fork2 f3	ARISTOTLE f2 fork2<----|
**							fork2<-|2|->fork1
**
*/


char	think(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (is_dead(progdata, last_meal, id))
		return(0);
	inform(CYN"is thinking"RESET, id, progdata);
	// pthread_mutex_lock(&((t_progdata *)progdata)->waiter);
	if (one_philosopher(id, last_meal, progdata))
		return (0);
	//take forks
	pthread_mutex_lock(&progdata->forks[fork1]);
	inform(YEL"has taken a fork"RESET, id, progdata);
	pthread_mutex_lock(&progdata->forks[fork2]);
	inform(YEL"has taken a fork"RESET, id, progdata);
	// pthread_mutex_unlock(&((t_progdata *)progdata)->waiter);
	return (1);
}

// char	think(int id, long long unsigned int *last_meal, t_progdata *progdata)
// {
// 	int	fork1;
// 	int	fork2;

// 	fork1 = (progdata->philosopher[id]).fork1;
// 	fork2 = (progdata->philosopher[id]).fork2;
// 	if (is_dead(progdata, last_meal, id))
// 		return(0);
// 	inform(CYN"is thinking"RESET, id, progdata);
// 	pthread_mutex_lock(&((t_progdata *)progdata)->waiter[fork1]);
// 	if (one_philosopher(id, last_meal, progdata))
// 		return (0);
// 	pthread_mutex_lock(&((t_progdata *)progdata)->waiter[fork2]);
// 	//take forks
// 	pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork1]);
// 	inform("has taken a fork", id, progdata);
// 	pthread_mutex_lock(&((t_progdata *)progdata)->forks[fork2]);
// 	inform("has taken a fork", id, progdata);
// 	pthread_mutex_unlock(&((t_progdata *)progdata)->waiter[fork2]);
// 	pthread_mutex_unlock(&((t_progdata *)progdata)->waiter[fork1]);
// 	return (1);
// }



char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	progdata->philosopher[id].eating = 1;
	if (is_full(progdata, id) || is_dead(progdata, last_meal, id))
	{
		unlock_forks(fork1, fork2, progdata);
		return (0);
	}
	inform(GRN"is eating"RESET, id, progdata);
	if (progdata->argc == 6)
		progdata->philosopher[id].times_ate++;
	pl_usleep(progdata->usec_time_to_eat);
	progdata->philosopher[id].eating = 0;
	unlock_forks(fork1, fork2, progdata);
	return (1);
}
/*
** The basic life cycle of a philosopher. First we want to initialize the
** philosopher. Since we pass our struct pointer as void, to avoid having to
** cast it to its type every time we use it, we copy its address to a pointer
** that is already cast to the right type. Bit of a waste of a cycle but it
** does make the code cleaner.
**
** After that we'll consider this philosopher's life to have started, so we
** record the birthdate as our last_meal time. The birthdate is the current time
** minus the simuation start time. This treats the start time as the 'zero time'
** and records any subsequent increments in relation to the start time. The
** start time is initialized just before the first thread is created.
**
** The first thing a newborn philosopher will need to do is identify itself by
** taking a unique ID number. A philosopher's unique ID number is equal to its
** position in the philosopher array, which starts at 0. The subject specifies
** that we print the first philosopher ID as 1, so when we print a philosopher
** ID we add 1 to the unique ID.
**
** We will consider that the fork to the right of each philosopher has the same
** position in the fork array as the philosopher's ID, and the fork to the left
** of each philosopher will have 1 + the philosopher's ID, except for the last
** philosopher, whose left fork will always be 0.
**
** The table will look like this:
**
**							 		0
**									|
**							f0  SOCRATES  f1
**					ZAMBRANO---3	@	 1---DE BEAUVOIR
**							f3	ARISTOTLE f2
**									|
**							 		2
**
** Once a philosopher has a unique ID number, we use the ID number to identify
** the forks that pertain to that philosopher. We want to ensure that each
** philosopher will always identify the lower-numbered fork as fork1 and the
** higher-numbered fork as fork2, to avoid deadlock (more on this in the eat
** description).
**
** Generally, fork1 will be fork[philosopher_id] and fork2 will be
** fork[philosopher_id + 1].
**
** However, if the philosopher's ID corresponds to the LAST philosopher, this
** logic will be inverted as the fork to this philosopher's left will always be
** fork0. So in this special case, fork1 will be fork[0], while fork2 will be
** fork[philosopher_id] (the RIGHT fork).
**
** If there is only one philosopher there will only be one fork, so both fork1
** and fork2 will equal 0. This philosopher will also be the last philosopher,
** so special case rules will apply.
**
** Once the forks are identified we enter the life_cycle loop. This is an
** infinite loop in which the philosopher thinks, eats and sleeps in that order.
** Thinking fails if a philosopher dies before thinking. Eating fails if a
** philosopher either dies or is 'full' (times_ate is equal to
** number_of_times_a_philosopher_must_eat) before eating. After eating, we also
** check to see if a philosopher is full or dead (took too long to eat). In any
** of these cases, we break the loop and end the thread.
**
** Otherwise, we enter sleeping mode and loop back to thinking.
*/

void	*life_cycle(void *progdata)
{
	int			id;
	t_progdata	*pdata;			
	long long unsigned int	last_meal;

	pdata = ((t_progdata *)progdata);
	last_meal = pl_get_time_msec() - pdata->time_start;
	id = identify_self(progdata);
	initialize(id, progdata);
	while(1)
	{
		if (!think(id, &last_meal, progdata) || !eat(id, &last_meal, progdata) \
		|| is_full(progdata, id) || is_dead(progdata, &last_meal, id))
			break ;
		inform(MAG"is sleeping"RESET, id,  progdata);
		pl_usleep(pdata->usec_time_to_sleep);
	}
	return (NULL);
}
