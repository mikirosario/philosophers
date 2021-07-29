/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   self_identification.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 01:50:57 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/29 16:31:27 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** This function identifies which forks will be picked up first and second by
** each philosopher. Even philosophers will pick up their right fork first, then
** their left, while odd philosophers will pick up their left fork first, then
** their right. This prevents deadlock and ensures alternation between
** neighbouring philosophers.
**
** Both forks and philosophers are in arrays numbered from 0 to
** total forks/philosophers - 1.
**
** We define right forks as those corresponding to the ID of the philosopher in
** their array (fork[id]), and left forks as those corresponding to the ID + 1
** (fork[id + 1]). The last philosopher's left fork is the first philosopher's
** right fork (fork 0), as they are seated in a circle.
**
** Since we want IDMAX + 1 to equal 0 (modular arithmetic), when we add 1 to a
** fork number we take the modulo of the fork number by the number of forks.
** This way, for example, in a circle with 4 forks numbered 0 - 3:
**
** 3 + 1 = 4 and 4 % 4 = 0, so 3 + 1 = 0.
** 3 + 2 = 5 and 5 % 4 = 1, so 3 + 2 = 1.
** 0 + 1 = 1 and 1 % 4 = 1, so 0 + 1 = 1.
**
** This is like adding times on a clock, basically. :) So now our position
** arithmetic treats the forks as if in a circle.
*/

void	identify_forks(int id, t_progdata *progdata)
{
	if (progdata->philosopher[id].even)
	{
		progdata->philosopher[id].fork1 = id;
		progdata->philosopher[id].fork2 = (id + 1) % progdata->number_of_forks;
	}
	else
	{
		progdata->philosopher[id].fork1 = (id + 1) % progdata->number_of_forks;
		progdata->philosopher[id].fork2 = id;
	}
}

/*
** This function is used by a philosopher thread to identify itself. The static
** int uid holds a unique identifier for each thread. When a thread requests an
** id, it uses the idlock mutex to ensure that only one thread can retrieve an
** id at a time. The id is copied to a local variable id and the static variable
** uid is incremented. When this is completed the idlock is released for another
** thread to use and the retrieved id is returned.
*/

int	identify_self(t_progdata *progdata)
{
	static int	uid = 0;
	int			id;

	pthread_mutex_lock(&progdata->idlock);
	id = uid++;
	pthread_mutex_unlock(&progdata->idlock);
	return (id);
}
