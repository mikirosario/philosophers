/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   marx.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 04:16:36 by miki              #+#    #+#             */
/*   Updated: 2021/07/11 06:23:53 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*communism(void *progdata)
{
	t_progdata *pdata;
	int			ration;

	pdata = (t_progdata *)progdata;
	pdata->max_concurrent_eaters = pdata->number_of_philosophers / 2;
	ration = pdata->number_of_philosophers - 1;
	while (pdata->comrades_present != pdata->number_of_philosophers)
		usleep(50);
	//a comer!!!!
	pdata->ration_card = ration;
	while (!pdata->stop)
	{
		while (pdata->free_forks != pdata->number_of_forks && !pdata->stop)
			usleep(50);
		if (ration < pdata->number_of_forks - 1)
			ration++;
		else
			ration = 0;
		pdata->ration_card = ration;
		usleep(100);
	}
	return (NULL);
}