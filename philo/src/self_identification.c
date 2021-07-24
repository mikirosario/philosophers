/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   self_identification.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 01:50:57 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/24 03:43:20 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

void	identify_forks(int id, t_progdata *progdata)
{
	//id par
	if (progdata->philosopher[id].even)
	{
		progdata->philosopher[id].fork1 = id;
		progdata->philosopher[id].fork2 = (id + 1) % progdata->number_of_philosophers;
	}
	//id impar
	else
	{
		progdata->philosopher[id].fork1 = (id + 1) % progdata->number_of_philosophers;
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
