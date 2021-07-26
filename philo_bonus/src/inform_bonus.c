/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inform_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:43:36 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/26 09:57:48 by mikiencolor      ###   ########.fr       */
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
*/

void	inform(char *msg, int id, t_progdata *progdata)
{
	sem_wait(progdata->printsem);
	printf("%llu %d"" %s\n", pl_get_time_msec() - progdata->time_start, \
	id + 1, msg);
	if (pl_strcmp(msg, RED"died"RESET))
		sem_post(progdata->printsem);
}
