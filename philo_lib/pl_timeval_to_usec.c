/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_timeval_to_usec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 21:33:38 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/10 14:20:49 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** This function will take a timestamp passed via the timeval struct and convert
** it to microseconds.
*/

long long unsigned int	pl_timeval_to_usec(struct timeval *time)
{
	return ((time->tv_sec * 1000000) + (time->tv_usec));
}
