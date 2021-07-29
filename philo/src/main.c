/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/07/29 16:23:25 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function checks and frees all memory reserved during run-time and also
** destroys all mutexes. We print error messages if any mutex destroys fail.
**
** Note that beautiful, norminette-friendly style. ;)
*/

void	freeme(t_progdata *progdata)
{
	if (progdata->gotidlock)
		if (pthread_mutex_destroy(&progdata->idlock))
			printf("Failed pthread_mutex_destroy call on idlock in freeme\n");
	if (progdata->gotprintlock)
		if (pthread_mutex_destroy(&progdata->printlock))
			printf("Failed pthread_mutex_destroy call on printlock in freeme\n");
	while (progdata->number_of_forks--)
		if (pthread_mutex_destroy(progdata->forks + progdata->number_of_forks))
			printf \
			("Failed pthread_mutex_destroy call on forks[%d] in freeme\n", \
			progdata->number_of_forks);
	if (progdata->thread)
		free(progdata->thread);
	if (progdata->forks)
		free(progdata->forks);
	if (progdata->philosopher)
		free(progdata->philosopher);
	progdata->philosopher = NULL;
	progdata->forks = NULL;
	progdata->thread = NULL;
	return ;
}

/*
** This function will return 1 if ANY philosophers are not full, meaning have
** not eaten at least number_of_times_a_philosopher_must_eat.
**
** If ALL philosophers are full, we return 0 to indicate there are no hungry
** philosophers left.
*/

char	hungry_philosophers(t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
		if (!progdata->philosopher[i++].full)
			return (1);
	return (0);
}

/*
** This function resolves philosophical debates. ;)
*/

void	hemlock(t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
		progdata->philosopher[i++].murdered = 1;
}

/*
** This function will join all threads owned by the process. It's called after
** philosophers are killed or we confirm they are all full.
*/

void	tjoin(t_progdata *progdata)
{
	size_t	i;
	int		err;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
	{
		err = pthread_join(progdata->thread[i++], NULL);
		if (err)
			printf("pthread_join error: %d\n", err);
	}
}

/*
** First we zero the progdata struct. Then we run setup. If it fails, it will
** be sure to complain to the user about it. If it succeeds, then the threads
** are all running!
**
** We wait for either the death of any philosopher or for all philosophers to be
** full. If any philosopher dies, all are sacrificed to the gods via the hemlock
** function.
**
** If all philosophers are dead or full, then they all close their threads. So
** now we can use pthread_join to reclaim the resources.
**
** Then we do freeme to free dynamically allocated memory.
**
** The is_dead function is now exclusively called from main in a continual loop.
** All philosophers are monitored continually by comparing the last time they
** ate with that time plus their time to die, and they are declared dead if the
** result is higher. To prevent philosophers from overwriting their last_meal
** while a check is in progress, we pass it by value, not by reference.
** Nevertheless, while this achieves the goal of continuous life-status checking
** without communication between philosophers, in slow CPUs or with many
** philosophers you get to a point where checking the status of every philosopher
** takes longer than 10ms. The 42 school Macs are able to handle it, though. :)
**
** Philosophers are monitored even after they finish all tasks in their thread.
** I don't use detatch_thread, so the thread data persists even when the
** philosophers finish all their tasks. However, in the is_dead check we specify
** that a full philosopher can never starve. This prevents is_dead from
** mistakenly reporting the death of philosophers that have stopped eating
** because they are full.
**
** There is also the problem that the checks can start before all the
** philosophers have a chance to initialize their last_meal variable, hence the
** milisecond sleep before the check routine begins to give the philosopher
** threads some extra time to do that.
**
** Maybe I could get better performance with a dedicated grim reaper thread, so
** the dead and hungry checks are performed concurrently instead of
** sequentially.
*/

int	main(int argc, char **argv)
{
	t_progdata	progdata;
	size_t		i;

	pl_bzero(&progdata, sizeof(t_progdata));
	if (setup(&progdata, argc, argv))
	{
		i = 0;
		usleep(1000);
		while (!is_dead(&progdata, progdata.philosopher[i].last_meal, i) && \
		hungry_philosophers(&progdata))
			if (++i == (size_t)progdata.number_of_philosophers)
				i = 0;
		if (progdata.philosopher[i].died)
			hemlock(&progdata);
		tjoin(&progdata);
	}
	freeme(&progdata);
	return (0);
}
