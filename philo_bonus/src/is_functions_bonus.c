/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_functions_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:02:29 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/29 12:19:09 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Using a known wait time, this function determines whether a philosopher will
** die while waiting and, if it will, it preemptively makes it wait until its
** death before is_dead is called to ensure that its death will be caught at the
** right time.
**
** The check is done by first calculating time of death (last_meal+time_to_die).
** Then we retrieve current_time in milliseconds.
** Then, if current_time+wait is greater than time_of_death we know the
** philosopher will die during the wait, so we wait for the amount of time
** corresponding to the difference between time_of_death and current_time plus
** 1 millisecond to ensure we catch it.
**
** If we determine the philosopher will die, 1 is returned. Otherwise, 0 is
** returned.
**
** This check eliminates the need for continuous liveness monitoring except when
** waiting for a waiter. :)
*/

char	will_die(t_progdata *progdata, long long unsigned int last_meal, \
long long unsigned int wait)
{
	long long unsigned int	time_of_death;
	long long unsigned int	current_time;

	time_of_death = last_meal + progdata->time_to_die;
	current_time = pl_get_time_msec();
	if (current_time + wait > time_of_death)
	{
		pl_usleep(time_of_death - current_time + 1);
		return (1);
	}
	return (0);
}

/*
** This function will check whether a philosopher has starved.
**
** First we get the difference between the current timestamp and the last meal.
**
** Then we check whether that difference is greater than time_to_die. If it is,
** we return 1 to confirm the philosopher has starved.
**
** We return 0 to indicate a philosopher has not starved.
*/

char	starved(t_progdata *progdata, long long unsigned int last_meal, int id)
{
	long long unsigned int	diff;

	if (!progdata->philosopher[id].full)
	{
		diff = pl_get_time_msec() - last_meal;
		if (diff > (long long unsigned int)progdata->time_to_die)
			return (1);
	}
	return (0);
}

/*
** This function will check whether a philosopher has eaten
** number_of_times_a_philosopher_must_eat times if that variable has been passed
** as an argument. If it has and the philosopher has eaten that many times, 1 is
** returned. If no such variable was passed, or the philosopher hasn't eaten
** that many times, 0 is returned.
*/

char	is_full(t_progdata *progdata, int id)
{
	if (progdata->argc == 6 && progdata->philosopher[id].times_ate == \
	progdata->number_of_times_each_philosopher_must_eat)
	{
		progdata->philosopher[id].full = 1;
		return (1);
	}
	return (0);
}

/*
** This function determines whether a philosopher has died. A philosopher will
** die by starving. If a philosopher starved, we first get a timestamp of time
** of death and then immediately inform of the death. Then we immediately get
** another timestamp and subtract time_of_death from it to ensure not more than
** 10 milliseconds have passed. If they have, we inform the user with a message.
** Then the philosopher's died variable is set to 1 (it will otherwise be 0).
**
** In any case, we return the value of the philosopher's died variable.
*/

char	is_dead(t_progdata *progdata, long long unsigned int last_meal, int id)
{
	long long unsigned int	time_of_death;

	if (starved(progdata, last_meal, id))
	{
		progdata->philosopher[id].died = 1;
		time_of_death = last_meal + progdata->time_to_die;
		inform(RED"died"RESET, id, progdata);
		if (pl_get_time_msec() - time_of_death > 10)
		{
			printf(RED \
			"Took more than 10 ms to inform of philosopher death\n"RESET);
		}
	}
	return (progdata->philosopher[id].died);
}
