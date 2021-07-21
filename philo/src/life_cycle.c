/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/22 01:00:59 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
**
** If there is more than one philosopher we immediately return 0. Otherwise, we
** wait time_to_die milliseconds, we run is_dead, which will confirm and
** register the philosopher's death, informing the main function of it so it can
** kill the other philosophers. Then we return 1.
*/

static char	one_philosopher(int id, long long unsigned int *last_meal, \
t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (fork1 != fork2)
		return (0);
	pl_usleep(progdata->usec_time_to_die + 1000);
	is_dead(progdata, last_meal, id);
	return (1);
}

/*
** This function simply unlocks the philosopher's forks if the philosopher has
** locked them. If an unlock fails, we notify the user. We also inform Comrade
** Karl Marx that the forks are available again. Comrade Marx will give the next
** group at the table permission to eat when he sees all forks have been
** returned.
**
** The special printf style is brought to you by norminette. ;)
*/

void	unlock_forks(int fork1, int fork2, int id, t_progdata *progdata)
{
	char	*msg1;
	char	*msg2;

	msg1 = "Failed pthread_mutex_unlock(fork[";
	msg2 = "]) in unlock_forks\n";
	if (progdata->philosopher[id].hasfork1)
	{
		pthread_mutex_lock(&progdata->kremlock);
		progdata->free_forks++;
		pthread_mutex_unlock(&progdata->kremlock);
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
		pthread_mutex_lock(&progdata->kremlock);
		progdata->free_forks++;
		pthread_mutex_unlock(&progdata->kremlock);
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
** Each even philosopher identifies its fork1 as its right fork (fork[id]) and
** its fork2 as its left fork as (fork[id + 1]), and each odd philosopher
** identifies its fork1 as its left fork (fork id + 1) and its fork 2 as its
** right fork (fork[id]). The left fork of the last philosopher is always
** fork[0].
**
** This prevents deadlock because, even if every philosopher were to try to take
** their fork1 simultaneously, at least one philosopher in any given pair would
** be unable to do it, since philosopher[0] and philosopher[1] have the same
** fork1, and so that philosopher would then also be unable to take its fork2,
** and so some philosophers would be able to eat.
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
** If the philosopher successfully thinks, we return 1.
**
** This solution on its own works reasonably well for even numbers of
** philosophers, but it chokes for odd numbers (like 5 800 200 200) because of
** a precession issue. In an odd number of philosophers there is always one more
** even philosopher than odd philosophers. Since this philosopher has both an
** odd AND an even neighbour, this extra philosopher can be starved by its
** neighbours hogging all its food in every instance.
**
** To resolve this problem I've turned to COMMUNISM. An extra thread will run
** the marx function. When the number of philosophers is odd, the marx function
** forces all philosophers to eat in alternating groups of a size determined by
** the maximum number of concurrent eaters. It does this by issuing a ration
** card that each philosopher can use to confirm whether it is or is not part of
** the group that is allowed to eat. It also forces all philosophers to begin
** eating at the same time. When the number of philosophers is even, anarchy
** reigns supreme. For reasons I don't quite understand, the evens seem to
** perform better with anarchy, while the odds prefer communism. O_O So I've
** adopted a one-project two-systems approach. :p
**
** When a philosopher takes its fork, it decrements the free_forks variable that
** the marx function checks continuously. When all forks are returned to the
** table, the marx function signals the next group to begin eating while the
** previous group is sleeping.
*/

static char	think(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	if (is_dead(progdata, last_meal, id))
		return (0);
	inform(CYN"is thinking"RESET, id, progdata);
	while (progdata->ration_card == -1)
		usleep(50);
	if (one_philosopher(id, last_meal, progdata))
		return (0);
	if (is_dead(progdata, last_meal, id))
		return (0);
	while (!ration_card(id, progdata) && !progdata->stop)
	{
	}
	if (is_dead(progdata, last_meal, id))
		return (0);
	pthread_mutex_lock(&progdata->forks[fork1]);
	pthread_mutex_lock(&progdata->kremlock);
	progdata->free_forks--;
	pthread_mutex_unlock(&progdata->kremlock);
	progdata->philosopher[id].hasfork1 = 1;
	inform(YEL"has taken a fork"RESET, id, progdata);
	if (is_dead(progdata, last_meal, id))
		return (0);
	pthread_mutex_lock(&progdata->forks[fork2]);
	pthread_mutex_lock(&progdata->kremlock);
	progdata->free_forks--;
	pthread_mutex_unlock(&progdata->kremlock);
	progdata->philosopher[id].hasfork2 = 1;
	inform(YEL"has taken a fork"RESET, id, progdata);
	return (1);
}

/*
** This function performs the eating tasks of a philosopher. First we set the
** philosopher status to eating. Then we check if it is full. A philosopher is
** full if it has eaten number_of_times_a_philosopher_must_eat times. If no
** argument was passed for that, is_full returns 0. Then we check if the
** philosopher is dead. A philosopher is dead if the time since its last meal
** is greater than the time_to_die. If the philosopher is not dead, and it is
** eating, the time_to_die is also reset here.
**
** If a philosopher is full or is dead, its forks are unlocked and we return 0.
**
** Otherwise, we inform that it is eating. If a sixth argument was provided, we
** increment the times_ate variable for the philosopher.
**
** Then the philosopher waits for time_to_eat milliseconds.
**
** Then the philosopher unlocks its forks.
**
** If the philosopher successfully eats, we return 1.
*/

static char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	int	fork1;
	int	fork2;

	fork1 = (progdata->philosopher[id]).fork1;
	fork2 = (progdata->philosopher[id]).fork2;
	progdata->philosopher[id].eating = 1;
	if (is_dead(progdata, last_meal, id) || is_full(progdata, id))
		return (0);
	inform(GRN"is eating"RESET, id, progdata);
	if (progdata->argc == 6)
		progdata->philosopher[id].times_ate++;
	pl_usleep(progdata->usec_time_to_eat);
	progdata->philosopher[id].eating = 0;
	unlock_forks(fork1, fork2, id, progdata);
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

void	*odd_life_cycle(void *progdata)
{
	int						id;
	t_progdata				*pdata;			
	long long unsigned int	last_meal;

	pdata = ((t_progdata *)progdata);
	id = identify_self(progdata);
	if (id % 2 == 0)
		pdata->philosopher[id].even = 1;
	identify_forks(id, progdata);
	pthread_mutex_lock(&pdata->kremlock);
	pdata->comrades_present++;
	pthread_mutex_unlock(&pdata->kremlock);
	last_meal = pl_get_time_msec();
	while (1)
	{
		if (!think(id, &last_meal, progdata) || !eat(id, &last_meal, progdata) \
		|| is_dead(progdata, &last_meal, id) || is_full(progdata, id))
			break ;
		inform(MAG"is sleeping"RESET, id, progdata);
		pl_usleep(pdata->usec_time_to_sleep);
	}
	return (NULL);
}
