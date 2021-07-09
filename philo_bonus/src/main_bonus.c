/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/07/09 10:16:04 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/types.h>
#include <sys/wait.h>

/*
** This function checks and frees all memory reserved during run-time and also
** destroys all mutexes. We print error messages if any mutex destroys fail.
**
** Note that beautiful, norminette-friendly style. ;)
*/

void	freeme(t_progdata *progdata)
{
	if (sem_close(progdata->printsem))
		printf("Failure in sem_close call on printsem in freeme CHILD_ID: %d\n", progdata->bonus_uid);
	if (sem_close(progdata->waitersem))
		printf("Failure in sem_close call on waitersem in freeme CHILD_ID: %d\n", progdata->bonus_uid);
	if (sem_close(progdata->forksem))
		printf("Failure in sem_close call on forksem in freeme CHILD_ID: %d\n", progdata->bonus_uid);
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

void	hemlock(t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
		(&progdata->philosopher[i++])->murdered = 1;
}

void	exit_success(t_progdata *progdata)
{
	freeme(progdata);
	exit(EXIT_SUCCESS);
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
	int			stat_loc;

	pl_bzero(&progdata, sizeof(t_progdata));
	// //unit test
	// get_args_utest(argc, argv, &progdata);
	// //unit test
	sem_unlink("/forksem");
	sem_unlink("/printsem");
	sem_unlink("/waitersem");
	printf("\n");
	if (setup(&progdata, argc, argv))
	{
		size_t i = 0;
		i = 0;
		while (i++ < (size_t)progdata.number_of_philosophers)
			waitpid(-1, &stat_loc, WUNTRACED);
	}
	sem_unlink("/forksem");
	sem_unlink("/printsem");
	sem_unlink("/waitersem");
	exit_success(&progdata);
	return (0);
}

// int	main(void)
// {
		
	
// }