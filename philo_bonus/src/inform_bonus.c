/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inform_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:43:36 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/09 00:02:09 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** This function informs on a philosopher's status. It uses the printlock mutex
** to ensure that only one philosopher can write to stdout at a time. We
** subtract the time_start from the timestamp before printing to ensure that the
** simulation start time is the zero time. We add 1 to the philosopher id to
** transform from array position to count.
**
** This function will now only inform on a philosopher's status if it is alive
** or if it died of starvation, not if the main function murdered it via the
** hemlock function. We do this by checking the murdered flag. (May remove this)
*/

void	inform(char *msg, int id, t_progdata *progdata)
{
	// if (!(&progdata->philosopher[id])->murdered)
	// {
		sem_wait(progdata->printsem);
		printf("%llu %d"" %s\n", pl_get_time_msec() - progdata->time_start, \
		id + 1, msg);
		sem_post(progdata->printsem);
	// }
}
