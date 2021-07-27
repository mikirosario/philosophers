/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inform_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:43:36 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/27 14:50:46 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** This function informs on a philosopher's status. It uses the printlock binary
** semaphore to ensure that only one philosopher can write to stdout at a time.
** We subtract the time_start from the timestamp before printing to ensure that
** the simulation start time is the zero time. We add 1 to the philosopher id
** as internally our unique ids start at 0.
**
** This function will only inform on a philosopher's status if it is alive or if
** it died of starvation. Processes waiting to be killed by the parent process
** no longer inform of their status, so a dying process does not post its
** print semaphore.
**
** You'll notice that weird infinite while under the sem_wait.
**
** On the school Macs the program hangs on exit. After some testing, my
** hypothesis is that when the process exits there is a point at which the
** printsem is deallocated/closed, yet the program is still running, which
** which causes the sem_wait here to fail and the process to continue and print
** out messages we don't want. This reliably happens on the school Macs, but
** does not seem to happen ever on my home Linux laptop.
**
** Either sem_wait or exit has different behaviour on Linux, or my laptop is
** just too slow for the program to react faster than the exit function can
** fully kill it.
**
** Whatever the case, by diverting the program into an infinite while loop if
** the sem_wait fails, we can avoid this bug. Not the most elegant solution, of
** course (if sem_wait ever legitimately fails the program will hang), but I'd
** be more elegant about it IF WE WERE ALLOWED TO USE MORE FUNCTIONS for the
** assignment. Actually if we could use more functions I'd set up a single
** shared memory space instead screwing around with all these reaper threads. :p
**
** Since I can't set up shared memory spaces due to assignment limitations, the
** semaphores and the process exit statuses are basically my only lines of
** inter-process communication. So I gotta make it work. xD 
*/

void	inform(char *msg, int id, t_progdata *progdata)
{
	if (sem_wait(progdata->printsem) == -1)
		while (1)
			usleep(10000);
	printf("%llu %d"" %s\n", pl_get_time_msec() - progdata->time_start, \
	id + 1, msg);
	if (pl_strcmp(msg, RED"died"RESET))
		sem_post(progdata->printsem);
}
