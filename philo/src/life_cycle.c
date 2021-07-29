/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/29 16:30:47 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function checks whether there is only one philosopher. If there is only
** one philosopher, there is only one fork. Since philosophers need two forks to
** eat, a sole philosopher must die. If fork1 == fork2 then there is only one
** fork, and therefore only one philosopher. In that case, we have the
** philosopher sleep for time_to_die milliseconds and return 1. This is so that
** the thread doesn't get stuck waiting for a double mutex lock on the same fork
** since it lacks two forks.
**
** If there is more than one philosopher we immediately return 0.
*/

char	one_philosopher(int id, t_progdata *progdata)
{
	if (progdata->philosopher[id].fork1 != progdata->philosopher[id].fork2)
		return (0);
	pl_usleep(progdata->time_to_die);
	return (1);
}

/*
** This function simply unlocks the philosopher's forks if the philosopher has
** locked them. If an unlock fails, we notify the user with the error message.
*/

void	unlock_forks(int fork1, int fork2, int id, t_progdata *progdata)
{
	char	*msg1;
	char	*msg2;

	msg1 = "Failed pthread_mutex_unlock(fork[";
	msg2 = "]) in unlock_forks\n";
	if (progdata->philosopher[id].hasfork1)
	{
		if (pthread_mutex_unlock(&progdata->forks[fork1]))
		{
			pthread_mutex_lock(&progdata->printlock);
			printf(RED"%s%d%s"RESET, msg1, fork1, msg2);
			pthread_mutex_unlock(&progdata->printlock);
		}
		progdata->philosopher[id].hasfork1 = 0;
	}
	if (progdata->philosopher[id].hasfork2)
	{
		if (pthread_mutex_unlock(&progdata->forks[fork2]))
		{
			pthread_mutex_lock(&progdata->printlock);
			printf(RED"%s%d%s"RESET, msg1, fork2, msg2);
			pthread_mutex_unlock(&progdata->printlock);
		}
		progdata->philosopher[id].hasfork2 = 0;
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
** Each even-numbered philosopher identifies its fork1 as its right fork
** (fork[id]) and its fork2 as its left fork as (fork[id + 1]), while each
** odd-numbered philosopher identifies its fork1 as its left fork (fork[id]) and
** its fork2 as its right fork (fork[id + 1]). We use modular arithmetic, since
** the forks are in a circle, so the last fork + 1 = the first fork.
**
** This prevents deadlock because pairs of neighbours have the same fork1, so
** even if every philosopher were to take their fork1 simultaneously, only one
** neighbour will succeed, while the other would have to wait, and thus be
** unable to take their fork2, which will be the fork1 of their other neighbour
** and can then be used by them.
**
** So the table looks like this, where SOCRATES is philosopher[0], DE BEAUVOIR
** is philosopher[1], ARISTOTLE is philosopher[2] and ZAMBRANO is
** philosopher[3].
**
**							fork1<-|0|->fork2
**					|->fork1 f0	SOCRATES  f1 fork2<----|
**					|ZAMBRANO	3	@	1	DE BEAUVOIR|
**					|->fork2 f3	ARISTOTLE f2 fork1<----|
**							fork2<-|2|->fork1
**
** Say Socrates takes fork1, Zambrano must wait; De Beauvoir takes fork1,
** Aristotle must wait. De Beauvoir and Socrates both try to take fork2, which
** is the same fork between them. Forks are mutexes, so only one can succeed.
** Whoever wins, eats. As soon as they release their forks, the one left waiting
** eats.
**
** If the philosopher successfully thinks, we return 1.
*/

char	think(int id, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (progdata->philosopher[id].died || progdata->philosopher[id].murdered)
		return (0);
	inform(CYN"is thinking"RESET, id, progdata);
	if (one_philosopher(id, progdata))
		return (0);
	pthread_mutex_lock(&progdata->forks[fork1]);
	progdata->philosopher[id].hasfork1 = 1;
	inform(YEL"has taken a fork"RESET, id, progdata);
	pthread_mutex_lock(&progdata->forks[fork2]);
	progdata->philosopher[id].hasfork2 = 1;
	inform(YEL"has taken a fork"RESET, id, progdata);
	return (1);
}

/*
** This function performs the eating tasks of a philosopher.
**
** First we set the philosopher status to eating. First we check if the
** philosopher is dead or murdered. A philosopher is flagged as dead if the
** liveness monitor in the main function finds at any time that the time since
** its last meal (current_time - last_meal) is greater than the time_to_die. A
** philosopher is flagged as murdered when the liveness monitor in the main
** function calls the hemlock function after it detects any philosopher has
** died.
**
** Then we check if the philosopher is full. A philosopher is full if it has
** eaten number_of_times_a_philosopher_must_eat times. If no argument was passed
** for that, is_full returns 0.
**
** If the philosopher neither dead, murdered nor full, we inform that the
** philosopher is eating and immediately update the last_meal time, which will
** now be equal to the time of the last_meal plus the time elapsed since the
** last_meal, which is just a needlessly complicated way of saying it is equal
** to the current time. But bear with me. xD
**
** If a sixth argument was provided, we increment the times_ate variable for the
** philosopher.
**
** Then the philosopher waits for time_to_eat milliseconds.
**
** Then the philosopher unlocks its forks.
**
** Then we check whether the philosopher was made full by this meal. If so, we
** set its full flag (within is_full) and return 0.
**
** If the philosopher successfully eats, we return 1.
*/

char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	progdata->philosopher[id].eating = 1;
	if (progdata->philosopher[id].died || progdata->philosopher[id].murdered \
	|| is_full(progdata, id))
		return (0);
	inform(GRN"is eating"RESET, id, progdata);
	*last_meal += pl_get_time_msec() - *last_meal;
	if (progdata->argc == 6)
		progdata->philosopher[id].times_ate++;
	pl_usleep(progdata->time_to_eat);
	progdata->philosopher[id].eating = 0;
	unlock_forks(fork1, fork2, id, progdata);
	if (is_full(progdata, id))
		return (0);
	return (1);
}

/*
** The basic life cycle of a philosopher. First we want to initialize the
** philosopher. Since we pass our struct pointer as void, to avoid having to
** cast it to its type every time we use it, we copy its address to a pointer
** that is already cast to the right type. Bit of a waste of a cycle but it
** does make the code cleaner. We also indicate evenness using the even flag.
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
** philosopher, whose left fork will always be 0, but we'll use modular
** arithmetic to get back around to it.
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
** even-numbered philosopher will always identify the lower-numbered fork as
** fork1 and the higher-numbered fork as fork2 and each odd-numbered philosopher
** will always identify the higher-numbered fork as fork 1 and the
** lower-numbered fork as fork2, to avoid deadlock and maximize concurrency
** (more on this in the eat description).
**
** If there is only one philosopher there will only be one fork, so both fork1
** and fork2 will equal 0. This philosopher will also be the last philosopher,
** so special case rules will apply (see one_philosopher function).
**
** After that we'll consider this philosopher's life to have started, so we
** record the birthdate as our last_meal time. Any to get the time since
** simulation start you always subtract the start_time from the timestamp (in
** milliseconds).
**
** Once the last_meal time is taken we enter the life_cycle loop. This is an
** infinite loop in which the philosopher thinks, eats and sleeps in that order.
**
** A continuous liveness monitor checks the liveness of every philosopher from
** the main thread. If a philosopher dies at any point, it is flagged with the
** died variable so it will exit at the next died check.
**
** Whenever a philosopher exits its life cycle we check whether it has to
** release its fork mutexes before ending the thread.
*/

void	*life_cycle(void *progdata)
{
	int						id;
	t_progdata				*pdata;

	pdata = ((t_progdata *)progdata);
	id = identify_self(progdata);
	if (id % 2 == 0)
		pdata->philosopher[id].even = 1;
	identify_forks(id, progdata);
	pdata->philosopher[id].last_meal = pl_get_time_msec();
	while (1)
	{
		if (!think(id, progdata) || \
		!eat(id, &pdata->philosopher[id].last_meal, progdata) || \
		pdata->philosopher[id].died || pdata->philosopher[id].murdered)
			break ;
		inform(MAG"is sleeping"RESET, id, progdata);
		pl_usleep(pdata->time_to_sleep);
	}
	unlock_forks(pdata->philosopher[id].fork1, \
	pdata->philosopher[id].fork2, id, progdata);
	return (NULL);
}
