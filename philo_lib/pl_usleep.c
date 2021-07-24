/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 01:56:58 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/24 05:12:28 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** Raquel suggested I could get better accuracy by making my own usleep, so I
** did! :) Will wait for wait milliseconds, give or take 50 microseconds.
*/

void	pl_usleep(int wait)
{
	uint64_t	time_end;

	time_end = pl_get_time_msec() + wait;
	while (pl_get_time_msec() != time_end)
		usleep(50);
}
