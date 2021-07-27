/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reap_philosophers_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 22:08:56 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/27 19:17:25 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>

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
	pl_usleep(pdata->time_to_die + 1);
	while (!is_dead(pdata, pdata->philosopher[pdata->bonus_uid].last_meal, \
	pdata->bonus_uid))
		pl_usleep(pdata->time_to_die + 1);
	exit_status(progdata, STARVED);
	return (NULL);
}

/*
** Finds matching number in array of size size. If none exists, returns 0.
** In this program, the numbers we're looking for are child PIDs. Children will
** never have PID 0, so we'll treat PID 0 as the end of the array.
*/
char	pidcmp(int num, int *arr, int size)
{
	int	i;

	i = 0;
	while (i < size && arr[i])
		if (arr[i++] == num)
			return (1);
	return (0);
}

/*
** This function waits to reap philosophers that have died because they were
** 'full' (ate number_of_times_a_philosopher_must_eat). We know why they died
** because they exit with status FULL when they die because they are full, and
** with status STARVED when they die of starvation. As long as philosophers
** continue to die because they are full, the simulation will continue. If any
** philosopher dies of starvation, however, we will leave this function and kill
** the rest to force the simulation to terminate.
**
** We do not want to kill philosophers that are already dead.
**
** The pid of every dead child is stored in the deadchildren array. This way,
** even if a philosopher dies because it is full followed by another philosopher
** dying of starvation, we will never try to kill philosophers that are already
** dead.
**
** If a child dies of starvation we immediately exit the function and return the
** number of dead children. If all children die because they are full we will
** return number_of_philosophers.
*/

int	wait_for_full_philosophers(t_progdata *progdata)
{
	int		stat_loc;
	size_t	i;

	i = 0;
	stat_loc = FULL;
	while (stat_loc == FULL && i < (size_t)progdata->number_of_philosophers)
	{
		progdata->deadchildren[i++] = waitpid(-1, &stat_loc, WUNTRACED);
		if (WIFEXITED(stat_loc))
			stat_loc = WEXITSTATUS(stat_loc);
	}
	if (stat_loc == PTHREAD_CREAT_ERR)
		iamerror(PTHREAD_CREAT_ERR, "wait_for_full_philosophers");
	return (i);
}

/*
** This function kills all processes that are still running.
**
** The pids of all child processes that have been spawned are stored in the
** progdata->children array. The pids of all child processes that are already
** dead are stored in the progdata->deadchildren array. A child cannot have pid
** 0.
**
** For each philosopher/child process in the progdata->children array we will
** compare its PID to all of the PIDs in the progdata->deadchildren array using
** the pidcmp function. If pidcmp finds a match it will return 1, otherwise it
** returns 0. If a match is found we skip over this child because it is already
** dead. If no match is found then this child is still alive, so we call kill on
** it.
*/

void	kill_philosophers(t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
	{
		if (!pidcmp(progdata->children[i], progdata->deadchildren, \
		progdata->number_of_philosophers))
			kill(progdata->children[i], SIGTERM);
		i++;
	}
}
