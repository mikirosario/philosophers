/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/29 12:21:07 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** This function simply has the philosopher return its forks to the table by
** posting the associated semaphore twice. If a post fails, we notify the user.
** Note that when both forks are returned to the table we also free a waiter so
** that another philosopher can get that waiter's attention and pick up the
** forks.
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
			printf(RED"Failed %s sem_post(progdata->forksem) in unlock_forks\n" \
			RESET, msg[i]);
			sem_post(progdata->printsem);
		}
		i++;
	}
	sem_post(progdata->waitersem);
}

/*
** This function performs the thinking tasks of a philosopher.
**
** Each philosopher is seated in a circle with a pile of forks in the middle.
**
** Thinking happens after sleeping.
**
** "Thinking" is basically what a philosopher does before picking up its forks.
** Forks are integers in the forksem semaphore and there are as many forks as
** there are philosophers. Each philosopher requires two forks to eat. If all
** forks are held by philosophers, then philosophers must wait before picking up
** more forks.
**
** Philosophers are served by waiters. Waiters are integers in the waitsem
** semaphore. Waiters serve philosophers forks two at a time. Thus, there is one
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
** The problem with the waiter semaphore is we don't know exactly how long a
** philosopher might be waiting, and so we don't know if it will die while
** waiting for a waiter to become available. To resolve this within the
** assignment limitations (which don't allow functions that set up shared memory
** spaces ¬¬), I create a dedicated grim_reaper thread that lives while a
** process is waiting to get a waiter's attention. The grim_reaper thread runs a
** continuous liveness check on the philosopher. When the philosopher gets the
** attention of the waiter, it signals the thread to exit and calls pthread_join
** on it. So it's "occasional continuous liveness monitoring". :p
**
** This reduces the number of simultaneous threads as they will only launch while
** processes are waiting for a waiter to become available. This helps reduce the
** CPU load with large numbers of philosophers.
**
** Thread creation failure at any point will cause the program to exit as if the
** philosopher had starved. An error message indicating the error will also be
** printed in that case.
**
** If the philosopher successfully thinks, we return.
*/

void	think(int id, t_progdata *progdata)
{
	inform(CYN"is thinking"RESET, id, progdata);
	progdata->stop = 0;
	if (pthread_create(&progdata->philosopher[id].grim_reaper, NULL, \
	grim_reaper, progdata))
		exit_status(progdata, PTHREAD_CREAT_ERR);
	sem_wait(progdata->waitersem);
	progdata->stop = 1;
	pthread_join(progdata->philosopher[id].grim_reaper, NULL);
	sem_wait(progdata->forksem);
	inform(YEL"has taken a fork"RESET, id, progdata);
	sem_wait(progdata->forksem);
	inform(YEL"has taken a fork"RESET, id, progdata);
}

/*
** This function performs the eating tasks of a philosopher. First we check if
** it is full. A philosopher is full if it has eaten
** number_of_times_a_philosopher_must_eat times. If no argument was passed for
** that, is_full always returns 0.
**
** If a philosopher is full before the meal its forks are unlocked and we return
** 0.
**
** Otherwise, we inform that it is eating. If the philosopher is eating then the
** last_meal time is reset here. If a sixth argument was provided, we also
** increment the times_ate variable for the philosopher.
**
** Then the philosopher waits for time_to_eat milliseconds. Before the wait we
** call will_die to determine whether the philosopher will die while eating. If
** so, we make it wait for as much time as it has left to live and call is_dead
** to inform of the death before exiting with the STARVED status. This causes
** all philosophers to be terminated by the parent.
**
** Otherwise, the philosopher waits for time_to_eat milliseconds. Then the
** philosopher unlocks its forks and its waiter.
**
** If the philosopher is full AFTER the meal we also return 0.
**
** If the philosopher successfully eats and is neither dead nor full, we return
** 1.
*/

char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	if (is_full(progdata, id))
	{
		unlock_forks(progdata);
		return (0);
	}
	inform(GRN"is eating"RESET, id, progdata);
	*last_meal += pl_get_time_msec() - *last_meal;
	if (progdata->argc == 6)
		progdata->philosopher[id].times_ate++;
	if (will_die(progdata, progdata->philosopher[id].last_meal, \
	progdata->time_to_eat))
		if (is_dead(progdata, progdata->philosopher[id].last_meal, id))
			exit_status(progdata, STARVED);
	pl_usleep(progdata->time_to_eat);
	unlock_forks(progdata);
	if (is_full(progdata, id))
		return (0);
	return (1);
}

/*
** The basic life cycle of a philosopher. First we want to initialize the
** philosopher. Since we pass our struct pointer as void, to avoid having to
** cast it to its type every time we use it, we copy its address to a pointer
** that is already cast to the right type. Bit of a waste of a cycle but it
** does make the code cleaner. Then we open all the named semaphores we'll be
** using.
**
** After that we'll consider this philosopher's life to have started, so we
** record the birthdate as our last_meal time. The birthdate is the current time
** minus the simuation start time. This treats the start time as the 'zero time'
** and records any subsequent increments in relation to the start time. The
** start time is initialized just before the first process is created.
**
** The first thing a newborn philosopher will need to do is identify itself by
** taking a unique ID number. A philosopher's unique ID number is equal to its
** position in the philosopher array, which starts at 0. The subject specifies
** that we print the first philosopher ID as 1, so when we print a philosopher
** ID we add 1 to the unique ID.
**
** A philosopher array is not needed in this implementation, it was inherited
** from the original. Might change this if there is time.
**
** Forks are piled up in the middle of the table and represented by the fork
** semaphore. There are forks / 2 waiters, that is, one waiter for every pair of
** forks.
**
** The table will look like this with four philosophers:
**
**							waiter | waiter
**
**							 		0
**									|
**								SOCRATES
**					ZAMBRANO---3  ffff	1---DE BEAUVOIR
**								ARISTOTLE
**									|
**							 		2
**
** Unlike with the mutex implementation, there is no need here to identify
** individual forks. The forks are all represented by the fork semaphore and
** when there are less than two left on the table, all the waiters are occupied,
** forcing the consumers to wait. Waiters are made available when pairs of forks
** are returned to the table by the consumers.
**
** If there is only one philosopher there will only be one fork and the
** philosopher then must die. This is controlled by the one_philosopher special
** case.
**
** Once the initial set-up is complete we enter the life cycle loop. This is an
** infinite loop in which the philosopher thinks, eats and sleeps in that order.
** Thinking fails if a philosopher dies while thinking. Eating fails if a
** philosopher is 'full' (times_ate is equal to
** number_of_times_a_philosopher_must_eat) before eating, dies while eating, or
** is full after eating.
**
** Otherwise, we enter sleeping mode and loop back to thinking. We also check to
** see if a philosopher dies while sleeping.
**
** Since we know the sleeping and eating times beforehand, we can predict
** whether a philosopher will die while sleeping or eating. We use will_die for
** this. If we predict a philosopher's death this way, will_die will make it
** wait until its time of death and is_dead will confirm and inform of its
** death. We then exit with the STARVED status. We don't know the thinking time
** beforehand so spawn a thread with a liveness monitor while thinking.
**
** When we leave the loop, if the philosopher died, we return STARVED. If
** it is alive and stopped eating because it is 'full', we return FULL. If
** thread creation failed in the think function, we return PTHREAD_CREAT_ERR,
** which for all intents and purposes behaves the same as STARVED, except an
** extra error message is then printed.
**
** This is so the parent process can distinguish between types of termination
** and act accordingly.
*/

void	life_cycle(void *progdata)
{
	int						id;
	t_progdata				*pdata;			

	pdata = ((t_progdata *)progdata);
	id = pdata->bonus_uid;
	pdata->philosopher[id].last_meal = pl_get_time_msec();
	while (1)
	{
		think(id, progdata);
		if (!eat(id, &pdata->philosopher[id].last_meal, progdata))
			break ;
		inform(MAG"is sleeping"RESET, id, progdata);
		if (will_die(progdata, pdata->philosopher[id].last_meal, \
		pdata->time_to_sleep))
			if (is_dead(progdata, pdata->philosopher[id].last_meal, id))
				exit_status(progdata, STARVED);
		pl_usleep(pdata->time_to_sleep);
	}
	exit_status(progdata, FULL);
}
