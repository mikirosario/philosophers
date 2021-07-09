/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   self_identification_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 01:50:57 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/09 09:32:56 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

void	identify_forks(int id, t_progdata *progdata)
{
	if (id < (progdata->number_of_philosophers - 1))
	{
		(progdata->philosopher[id]).fork1 = id;
		(progdata->philosopher[id]).fork2 = id + 1;
	}
	else
	{
		(progdata->philosopher[id]).fork1 = 0;
		(progdata->philosopher[id]).fork2 = id;
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
	return (progdata->bonus_uid);
}
