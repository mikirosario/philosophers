/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_msec_diff.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 17:02:32 by miki              #+#    #+#             */
/*   Updated: 2021/07/10 22:36:53 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** BRIEF DESCRIPTION
**
** This function returns the difference in milliseconds between the times passed
** in t0 and t1, where t0 is the earlier time and t1 is the later time.
**
** If no t0 is passed, the function returns 0. If no t1 is passed, then the
** function will create one, use it to get the present time, and then return
** the difference between that time and t0.
**
** MORE IN-DEPTH
**
** The timeval struct stores the timestamp in seconds (tv_sec) and microseconds
** (tv_usec). A millisecond is one thousand microseconds, and one thousandth of
** a second. Hence, to convert the seconds to milliseconds we multiply by a
** thousand, and to convert the microseconds to milliseconds we divide by a
** thousand, then simply sum up the results.
**
** In this case, since we want the difference in milliseconds between the two
** timestamps passed as arguments (t0 and t1), where t1 should be the later time
** and t0 the earlier time, first we subtract the seconds of t0 from the seconds
** of t1, then convert the difference into milliseconds, then subtract the
** microseconds of t0 from the microseconds of t1, then convert the difference
** into milliseconds, then sum up the differences.
**
** A long long unsigned int will hold a difference in milliseconds up to
** 584,942,417 Years, 4 Months, 7 Days, 22 Hours, 6 Minutes and 40 Seconds long.
**
** Approximately.
**
** If you believe you may need more time than this, do not use this function.
**
** We're also assuming time only moves forwards, so behaviour in
** faster-than-light applications, or when mixing up the later and earlier
** times, or in non-entropic universes, or with broken clocks, may not be as
** expected.
*/

long long unsigned int	pl_msec_diff(struct timeval *t0, struct timeval *t1)
{
	struct timeval	t_end;

	if (!t0)
		return (0);
	else if (!t1)
	{
		gettimeofday(&t_end, NULL);
		t1 = &t_end;
	}
	return ((t1->tv_sec - t0->tv_sec) * 1000.0 + \
	(t1->tv_usec - t0->tv_usec) / 1000.0);
}
