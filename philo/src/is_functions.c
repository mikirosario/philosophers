/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:02:29 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/24 23:36:40 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function will check whether a philosopher has starved, and also will
** replenish it if it has the eating status.
**
** First we get the difference between the current timestamp and the last meal.
**
** Then we check whether that difference is greater than time_to_die. If it is,
** we return 1 to confirm the philosopher has starved.
**
** If the philosopher has the eating status, we add the difference to the
** last_meal.
**
** We return 0 to indicate a philosopher has not starved.
*/

char	starved(t_progdata *progdata, long long unsigned int last_meal, int id)
{
	long long unsigned int	diff;
(void)id;
	diff = pl_get_time_msec() - last_meal;
	if (diff > (long long unsigned int)progdata->time_to_die)
		return (1);
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
	if (progdata->argc == 6 && (&progdata->philosopher[id])->times_ate == \
	progdata->number_of_times_each_philosopher_must_eat)
	{
		if (progdata->philosopher[id].eating)
			pl_usleep(progdata->time_to_eat);
		progdata->philosopher[id].full = 1;
		unlock_forks(progdata->philosopher[id].fork1, \
		progdata->philosopher[id].fork2, id, progdata);
		return (1);
	}
	return (0);
}

/*
** This function determines whether a philosopher has died. There are two ways
** a philosopher might die: by starving, or by being killed by the hemlock
** function because another philosopher has died. If a philosopher starved, or
** is simply already made dead by the hemlock function, we first get a timestamp
** of time of death and then immediately inform of the death. Then we
** immediately get another timestamp and subtract time_of_death from it to
** ensure not more than 10 milliseconds have passed. If they have, we inform the
** user with a message. Then the philosopher's died variable is set to 1, in
** case it wasn't already. In any case, we return the value of the philosopher's
** died variable.
**
** NOTE: The first starved philosopher will now flip the stop flag to mute the
** other threads, per popular demand. The 10 millisecond rule likewise only
** applies to the starved philosopher, not the murdered ones.
*/

char	is_dead(t_progdata *progdata, long long unsigned int last_meal, int id)
{
	long long unsigned int	time_of_death;
	static char				first_death = 1;

	if (starved(progdata, last_meal, id) || \
	(&progdata->philosopher[id])->murdered)
	{
		(&progdata->philosopher[id])->died = 1;
		time_of_death = last_meal + progdata->time_to_die;
		inform(RED"died"RESET, id, progdata);
		pthread_mutex_lock(&progdata->printlock);
		if (first_death && pl_get_time_msec() - time_of_death > 10)
		{
			printf(RED \
			"Took more than 10 ms to inform of philosopher death\n"RESET);
		}
		pthread_mutex_unlock(&progdata->printlock);
		first_death = 0;
	}
	return ((&progdata->philosopher[id])->died);
}
