/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:43:19 by miki              #+#    #+#             */
/*   Updated: 2021/07/27 16:13:59 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** This function checks whether there is only one philosopher. If there is only
** one philosopher, there is only one fork. Since philosophers need two forks to
** eat, a sole philosopher must die.
**
** In that case, we have the philosopher sleep for time_to_die milliseconds,
** and then we run is_dead to register its death. I add 1ms to time_to_sleep,
** otherwise sometimes the is_dead instructions are executed so quickly that the
** philosopher survives and the program hangs waiting for its termination.
**
** If there is more than one philosopher this function does nothing.
*/

char	one_philosopher(t_progdata *progdata)
{
	if (progdata->number_of_philosophers == 1)
	{
		pl_usleep(progdata->time_to_die);
		return (1);
	}
	return (0);
}

/*
** This thread is spawned by each process and acts as a liveness monitor. If the
** process meets the conditions for starvation at any point, is_dead will return
** true. The is_dead function calls inform to notify of the first death, and
** inform will NOT return the printsem binary semaphore after a death
** notification, so the death notification will be the last thing printed.
**
** We check liveness every 5 milliseconds, so we will report a philosopher's
** death from 0 - ~5 milliseconds after it happens. In practice, on my laptop,
** if a philosopher dies at 310 we are informed from 311 to 316 at latest. The
** school Macs might be better at this.
**
** If the philosopher is dead, we exit with the STARVED status. If there is only
** one philosopher, of course, it must starve. Exiting destroys the process
** along with the thread, so nothing more to do here. The main function will
** proceed to kill and reap the rest of the children as soon as it receives
** confirmation that a process has exited with the STARVED status. :)
**
** Semaphore possession doesn't matter as all remaining processes will now be
** terminated.
*/

void	*grim_reaper(void *progdata)
{
	t_progdata	*pdata;

	pdata = (t_progdata *)progdata;
	//one_philosopher(pdata);
	pl_usleep(pdata->time_to_die + 1);
	while (!is_dead(pdata, pdata->philosopher[pdata->bonus_uid].last_meal, \
	pdata->bonus_uid))
	pl_usleep(pdata->time_to_die + 1);
		//pl_usleep(1);
	exit_status(progdata, STARVED);
	return (NULL);
}

/*
** This function simply has the philosopher return its forks to the table by
** posting the associated semaphore twice. If a post fails, we notify the user.
** Note that when both forks are returned to the table we also free a waiter so
** that another philosopher can get that waiter's attention and pick up the
** forks.
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

char	think(int id, t_progdata *progdata)
{
	inform(CYN"is thinking"RESET, id, progdata);
	sem_wait(progdata->waitersem);
	sem_wait(progdata->forksem);
	inform(YEL"has taken a fork"RESET, id, progdata);
	sem_wait(progdata->forksem);
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

char	eat(int id, long long unsigned int *last_meal, t_progdata *progdata)
{
	progdata->philosopher[id].eating = 1;
	if (is_full(progdata, id))
	{
		unlock_forks(progdata);
		return (0);
	}
	inform(GRN"is eating"RESET, id, progdata);	
	*last_meal += pl_get_time_msec() - *last_meal;
	if (progdata->argc == 6)
		progdata->philosopher[id].times_ate++;
	pl_usleep(progdata->time_to_eat);
	progdata->philosopher[id].eating = 0;
	unlock_forks(progdata);
	if (is_full(progdata, id))
		return(0);
	return (1);
}

/*eat
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
** Thinking fails if a philosopher dies before thinking. Eating fails if a
** philosopher either dies or is 'full' (times_ate is equal to
** number_of_times_a_philosopher_must_eat) before eating. After eating, we also
** check to see if a philosopher is full or dead (took too long to eat). In any
** of these cases, we break the loop and leave the loop.
**
** Otherwise, we enter sleeping mode and loop back to thinking.
**
** When we leave the loop, if the philosopher died, we return EXIT_FAILURE. If
** it is alive and stopped eating because it is 'full', we return EXIT_SUCCESS.
** This is so the parent process can distinguish between types of termination.
*/

void	life_cycle(void *progdata)
{
	int						id;
	t_progdata				*pdata;			
	//long long unsigned int	last_meal;

	pdata = ((t_progdata *)progdata);
	//pdata->forksem = sem_open("/forksem", 0);
	//pdata->printsem = sem_open("/printsem", 0);
	//pdata->waitersem = sem_open("/waitersem", 0);
	id = pdata->bonus_uid;
	if (pthread_create(&pdata->philosopher[id].grim_reaper, NULL, grim_reaper, progdata))
		exit_status(progdata, PTHREAD_CREAT_ERR);
	pdata->philosopher[id].last_meal = pl_get_time_msec();
	while (1)
	{
		if (!think(id, progdata) || !eat(id, &pdata->philosopher[id].last_meal, progdata))
			break ;
		inform(MAG"is sleeping"RESET, id, progdata);
		pl_usleep(pdata->time_to_sleep);
	}
	// while (1)
	// {
	// 	pl_usleep(100000);
	// }
	// if (pdata->philosopher[id].died)
	// 	exit_status(progdata, STARVED);
	exit_status(progdata, FULL);
}
