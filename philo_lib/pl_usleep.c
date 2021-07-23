/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 01:56:58 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/24 01:02:58 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** Raquel suggested I could get better accuracy by making my own usleep, so I
** did! :) Will wait for wait microseconds, give or take 50.
*/

void	pl_usleep(long long unsigned int wait)
{
	struct timeval			timestamp;
	long long unsigned int	time_end;

	gettimeofday(&timestamp, NULL);
	time_end = pl_timeval_to_usec(&timestamp) + wait;
	while (!gettimeofday(&timestamp, NULL) && \
	(pl_timeval_to_usec(&timestamp) < time_end))
	{
	}
		//usleep(100);
}
