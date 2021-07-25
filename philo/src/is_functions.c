/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 02:02:29 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/25 21:23:16 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function will check whether a philosopher has starved. Full philosophers
** cannot starve.
**
** First we get the difference between the current timestamp and the last meal.
**
** Then we check whether that difference is greater than time_to_die. If it is,
** we return 1 to confirm the philosopher has starved.
**
** We return 0 to indicate a philosopher has not starved.
*/

char	starved(t_progdata *progdata, int id, long long unsigned int last_meal)
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
** as an argument. If it has and the philosopher has eaten exactly that many
** times, 1 is returned. If no such variable was passed, or the philosopher has
** eaten more or less than that many times, 0 is returned. If the fullness monitor
** in main detects all philosophers are full, the simulation is stopped.
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
** This function determines whether a philosopher has died. There are two ways
** a philosopher might die: by starving, or by being killed by the hemlock
** function because another philosopher has died ("murdered").
**
** If a philosopher starved, or was murdered by the hemlock function, we first
** get a timestamp of the time of death and then immediately inform of the
** death. Any philosopher's time of death will always be the same as the time of
** its last meal + time_to_die. We pass last_meal by value here, so even if it's
** been updated while this check is ongoing, we will still properly record the
** death.
**
** Then we immediately get another timestamp and subtract time_of_death from it
** to ensure not more than 10 milliseconds have passed. If they have, we inform
** the user with a message. Note that this program is optimized to run on 42
** Madrid Macs. If you have slower CPUs or less threads, you're more likely to
** run past the 10ms limit with high numbers of philosphers. This is because the
** time between checks for every philosopher is directly related to the number
** of philosophers that need to be checked. This is confirmed to be under 10 ms
** for 200 philosophers on 42 Madrid Macs, but definitely not on my humble
** laptop or ye olde potato. ;)
**
** Then, if dead, the philosopher's died variable is set to 1.
**
** Whether or not the philosopher is dead, we always return the value of the
** philosopher's died variable to the calling function to inform of the result.
**
** NOTE: The first dead philosopher will now cause the inform function to flip
** the stop flag to mute the other threads, per popular (Kevin ;p) demand.
**
** NOTE: 10 millisecond check likewise now only applies to the starved
** philosopher, NOT to the subsequently the murdered ones, by flipping a static
** char in the is_dead function. Note the printlock mutex locks the whole check,
** not just the print. There can only be ONE first_death, so if two threads die
** at the same time we do need that photo-finish to determine who the REAL first
** dead philosopher is. ;) Once the flag IS flipped though, we don't want to
** bother with that mutex again, so it's also behind an un-mutexed check.
*/

char	is_dead(t_progdata *progdata, long long unsigned int last_meal, int id)
{
	long long unsigned int	time_of_death;
	static char				first_death = 1;

	if (starved(progdata, id, last_meal) || \
	(&progdata->philosopher[id])->murdered)
	{
		(&progdata->philosopher[id])->died = 1;
		time_of_death = last_meal + progdata->time_to_die;
		inform(RED"died"RESET, id, progdata);
		if (first_death)
		{
			pthread_mutex_lock(&progdata->printlock);
			if (first_death && pl_get_time_msec() - time_of_death > 10)
			{
				printf(RED \
				"Took more than 10 ms to inform of philosopher death\n"RESET);
			}
			first_death = 0;
			pthread_mutex_unlock(&progdata->printlock);
		}
	}
	return ((&progdata->philosopher[id])->died);
}
