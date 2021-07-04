/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/07/04 03:42:55 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function checks and frees all memory reserved during run-time and also
** destroys all mutexes. We print error messages if any mutex destroys fail.
**
** Note that beautiful, norminette-friendly style. ;)
*/

void	freeme(t_progdata *progdata)
{
	if (pthread_mutex_destroy(&progdata->idlock))
		printf("Failure in pthread_mutex_destroy call on idlock in freeme\n");
	if (pthread_mutex_destroy(&progdata->waiter))
		printf("Failure in pthread_mutex_destroy call on waiter in freeme\n");
	while (progdata->number_of_forks--)
		if (pthread_mutex_destroy(progdata->forks + progdata->number_of_forks))
			printf \
			("Failure in pthread_mutex_destroy call on forks[%d] in freeme\n", \
			progdata->number_of_forks);
	if (progdata->thread)
		free(progdata->thread);
	if (progdata->forks)
		free(progdata->forks);
	if (progdata->philosopher)
		free(progdata->philosopher);
	progdata->philosopher = NULL;
	return ;
}
/*
** This function will return 1 if ANY philosophers are not full, meaning have
** not eaten at least number_of_times_a_philosopher_must_eat.
**
** If ALL philosophers are full, we return 0 to indicate there are no hungry
** philosophers left.
**
** The is_full function always returns false (aka. hungry) if no argument was
** passed for number_of_times_a_philosopher_must_eat. Thus, this function will
** also always return 0 in that case.
*/

char	hungry_philosophers(t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
		if (!is_full(progdata, i++))
			return (1);
	return (0);
}

/*
** First we zero the progdata struct. Then we run setup. If it fails, it will
** be sure to complain to the user about it. If it succeeds, then the threads
** are all running!
**
** We wait for either the death of any philosopher or for all philosophers to be
** full. If any philosopher dies, all are sacrificed to the gods via the hemlock
** function.
**
** If all philosophers are dead or full, then they all close their threads. So
** now we can use pthread_join to reclaim the resources.
**
** Then we do freeme to free dynamically allocated memory.
*/

int	main(int argc, char **argv)
{
	t_progdata	progdata;

	pl_bzero(&progdata, sizeof(t_progdata));
	// //unit test
	// get_args_utest(argc, argv, &progdata);
	// //unit test

	printf("\n");
	if (setup(&progdata, argc, argv))
	{
		size_t i = 0;
		while (!(&progdata.philosopher[i])->died && hungry_philosophers(&progdata))
		{
			if (++i == (size_t)progdata.number_of_philosophers)
				i = 0;
		}
		//masacre
		if ((&progdata.philosopher[i])->died)
		{
			i = 0;
			while (i < (size_t)progdata.number_of_philosophers)
				(&progdata.philosopher[i++])->died = 1;
		}
		i = 0;
		while (i < (size_t)progdata.number_of_philosophers)
			pthread_join(progdata.thread[i++], NULL);
	}
	freeme(&progdata);
	return (0);
}
