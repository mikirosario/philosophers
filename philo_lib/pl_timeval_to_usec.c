/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_timeval_to_usec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 21:33:38 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/03 21:37:51 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

long long unsigned int	pl_timeval_to_usec(struct timeval *time)
{
	return ((time->tv_sec * 1000000) + (time->tv_usec));
}
