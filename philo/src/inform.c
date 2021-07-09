/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inform.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:43:36 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/09 20:05:00 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function informs on a philosopher's status. It uses the printlock mutex
** to ensure that only one philosopher can write to stdout at a time. We
** subtract the time_start from the timestamp before printing to ensure that the
** simulation start time is the zero time. We add 1 to the philosopher id to
** transform from array position to count.
**
** This function will now only inform on a philosopher's status if it is alive
** or if it died of starvation, not if the main function murdered it via the
** hemlock function. We do this by flipping the stop flag when any philosopher
** dies. The stop flag check is mutexed so it cannot be read until we're done
** flipping it, and so no other process can decide to print until then.
*/

void	inform(char *msg, int id, t_progdata *progdata)
{
	pthread_mutex_lock(&progdata->printlock);
	if (!progdata->stop)
	{
		printf("%llu %d"" %s\n", pl_get_time_msec() - progdata->time_start, \
		id + 1, msg);
	}
	if (!pl_strcmp(msg, RED"died"RESET))
		progdata->stop = 1;
	pthread_mutex_unlock(&progdata->printlock);
}
