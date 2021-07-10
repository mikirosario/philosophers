/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_get_time_msec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 14:51:51 by miki              #+#    #+#             */
/*   Updated: 2021/07/10 14:19:28 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** Retrieves current time in milliseconds.
*/

long long unsigned int	pl_get_time_msec(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return (pl_timeval_to_msec(&timestamp));
}
