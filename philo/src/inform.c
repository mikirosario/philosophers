/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inform.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:43:36 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/10 14:35:44 by miki             ###   ########.fr       */
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
** dies. The stop flag check is mutexed so the stop flag cannot be read by any
** thread until other threads are done flipping it or leaving it alone, and so
** no other thread can decide to print while another is performing the check.
**
** Personally I think it's SILLY. All it does is stop other threads from
** printing their status until they are killed by main after one thread dies.
** It does NOT change the underlying functioning OR make the simulation stop
** any sooner than it otherwise would OR prevent other threads from continuing
** with their life cycles until they hit the next is_dead check. All it does is
** mute all the threads.
**
** But certain 42 students apparently have a very emotional attachment to having
** "died" be the last message they see and apparently will FAIL projects that do
** not satisfy this criterion. So... here it is. :P
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
