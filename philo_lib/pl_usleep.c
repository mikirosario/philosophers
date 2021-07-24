/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 01:56:58 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/24 14:13:58 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** Raquel suggested I could get better accuracy by making my own usleep, so I
** did! :) Will wait for wait milliseconds, give or take 50 microseconds.
*/

void	pl_usleep(int wait)
{
	long long unsigned int	time_end;

	time_end = pl_get_time_msec() + wait;
	while (pl_get_time_msec() != time_end)
		usleep(50);
}
