/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/10 01:49:06 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

/*
** This function checks whether there is only one philosopher. If there is only
** one philosopher, there is only one fork. Since philosophers need two forks to
** eat, a sole philosopher must die.
**
** In that case, we have the philosopher sleep for time_to_die milliseconds,
** and then we run is_dead to register its death.. I add 1ms to time_to_sleep,
** otherwise sometimes the is_dead instructions are executed so quickly that the
** philosopher survives and the program hangs waiting for its termination.
**
** If there is more than one philosopher this function does nothing.
*/

char	one_philosopher(int id, long long unsigned int *last_meal, \
t_progdata *progdata)
{
	if (progdata->number_of_philosophers == 1)
	{
		pl_usleep(progdata->usec_time_to_die + 1000);
		is_dead(progdata, last_meal, id);
		return (1);
	}
	return (0);
}

/*
** This function simply has the philosopher return its forks to the table by
** posting the associated semaphore twice. If a post fails, we notify the user.
**
** The redundant printfs are brought to you by norminette.
*/

void	unlock_forks(t_progdata *progdata)
{
	int		i;
	char	*msg[2];

	i = 0;
	msg[0] = "first";
	msg[1] = "second";
	while (i < 2)
	{
		if (sem_post(progdata->forksem) == -1)
		{
			sem_wait(progdata->printsem);
			printf(RED);
			printf("Failed %s sem_post(progdata->forksem) in unlock_forks\n", \
			msg[i]);
			printf(RESET);
			sem_post(progdata->printsem);
		}
		i++;
	}
	sem_post(progdata->waitersem);
}

/*
** Each philosopher is seated in a circle with a pile of forks in the middle.
**
** Thinking happens after sleeping. Before thinking we check if a philosopher
** slept too long and is dead. If it is, we return 0. If not, we start thinking.
**
** "Thinking" is basically what a philosopher does before picking up its forks.
** Forks are integers in the forksem semaphore and there are as many forks as
** there are philosophers. Each philosopher requires two forks to eat. If all
** forks are held by philosophers, then philosophers must wait before picking up
** more forks.
**
** Philosophers are served by waiters. Waiters are integers in the waitsem
** semaphore. Waiters serve philosophers forks two at a time. There is one
** waiter for every pair of forks, which is to say there are number_of_forks / 2
** waiters.
**
** When a philosopher wants forks it must get the 'attention' of a waiter by
** calling wait on the waiter semaphore. When it has a waiter's attention it can
** pick up its two forks. It will continue to have the attention of the waiter
** until it releases both forks. If less than two forks are left in the pile,
** all waiters will be occupied and a philosopher must wait for a waiter to be
** free before taking its forks.
**
** So the table looks like this, where SOCRATES is philosopher[0], DE BEAUVOIR
** is philosopher[1], ARISTOTLE is philosopher[2] and ZAMBRANO is
** philosopher[3]. With four forks on the table there are forks / 2 waiters.
**
**							 waiter | waiter
**									0
**								SOCRATES
**					|ZAMBRANO	3 ffff	1	DE BEAUVOIR|
**								ARISTOTLE
**									2
**
** If the philosopher successfully thinks, we return 1.
*/

char	think(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	if (is_dead(progdata, last_meal, id))
		return (0);
	inform(CYN"is thinking"RESET, id, progdata);
	// pthread_mutex_lock(&((t_progdata *)progdata)->waiter);
	if (one_philosopher(id, last_meal, progdata))
		return (0);
	//take forks
	sem_wait(progdata->waitersem);
	sem_wait(progdata->forksem);
	inform(YEL"has taken a fork"RESET, id, progdata);
	sem_wait(progdata->forksem);
	inform(YEL"has taken a fork"RESET, id, progdata);
	// pthread_mutex_unlock(&((t_progdata *)progdata)->waiter);
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

char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	progdata->philosopher[id].eating = 1;
	if (is_full(progdata, id) || is_dead(progdata, last_meal, id))
	{
		unlock_forks(progdata);
		return (0);
	}
	inform(GRN"is eating"RESET, id, progdata);
	if (progdata->argc == 6)
		progdata->philosopher[id].times_ate++;
	pl_usleep(progdata->usec_time_to_eat);
	progdata->philosopher[id].eating = 0;
	unlock_forks(progdata);
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

void	*reaper(void *progdata)
{
	//(void)progdata;
	sem_wait(((t_progdata *)progdata)->killsem);
	//printf("CHIVATIN %d \n", (((t_progdata *)progdata)->bonus_uid));
	printf("CHIVATIN %d \n", getpid());
	//pthread_detach(((t_progdata *)progdata)->reaper);
	//exit_failure(progdata);
	kill(getpid(), SIGTERM);
	return (NULL);
}


void	*life_cycle(void *progdata)
{
	int			id;
	t_progdata	*pdata;			
	long long unsigned int	last_meal;

	pdata = ((t_progdata *)progdata);
	// // MI GOZO EN UN POZO
	// pdata->killsem = sem_open("/killsem", 0);
	// // MI GOZO EN UN POZO
	pdata->forksem = sem_open("/forksem", 0);
	pdata->printsem = sem_open("/printsem", 0);
	pdata->waitersem = sem_open("/waitersem", 0);
	// // MI GOZO EN UN POZO
	// if (pthread_create(&pdata->reaper, NULL, reaper, progdata))
	// {
	// 	exit_failure(progdata);
	// }
	// // MI GOZO EN UN POZO
	last_meal = pl_get_time_msec();
	id = pdata->bonus_uid;
	//identify_forks(id, progdata);
	while(1)
	{
		// // MI GOZO EN UN POZO
		// if (!think(id, &last_meal, progdata) || !eat(id, &last_meal, progdata) \
		// || is_full(progdata, id) || is_dead(progdata, &last_meal, id))
		// 	sem_post(pdata->killsem);
		// // MI GOZO EN UN POZO

		if (!think(id, &last_meal, progdata) || !eat(id, &last_meal, progdata) \
		|| is_full(progdata, id) || is_dead(progdata, &last_meal, id))
			break ;
		inform(MAG"is sleeping"RESET, id, progdata);
		pl_usleep(pdata->usec_time_to_sleep);
	}
	if (pdata->philosopher[id].died)
		exit_failure(progdata);
	exit_success(progdata);
	return (NULL);
}
