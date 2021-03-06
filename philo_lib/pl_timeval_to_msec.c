/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_timeval_to_msec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 13:45:23 by miki              #+#    #+#             */
/*   Updated: 2021/07/24 01:39:31 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** This function will take a timestamp passed via the timeval struct and convert
** it to milliseconds.
*/

long long unsigned int	pl_timeval_to_msec(struct timeval *time)
{
	return ((time->tv_sec * 1000) + (time->tv_usec / 1000));
}
