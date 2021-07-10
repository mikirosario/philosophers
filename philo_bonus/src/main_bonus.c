/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/07/10 14:06:34 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/*
** This function checks and frees all memory reserved during run-time and also
** closes all semaphores. We print error messages if any mutex destroys fail.
**
** Note that beautiful, norminette-friendly style. ;)
*/

void	freeme(t_progdata *progdata)
{
	// // MI GOZO EN UN POZO
	// if (sem_close(progdata->killsem))
	// 	printf("Failure in sem_close call on killsem in freeme PROC_ID: %d\n",
	// 	progdata->bonus_uid);
	// // MI GOZO EN UN POZO
	if (sem_close(progdata->printsem))
		printf("Failure in sem_close call on printsem in freeme PROC_ID: %d\n",
		progdata->bonus_uid);
	if (sem_close(progdata->waitersem))
		printf("Failure in sem_close call on waitersem in freeme PROC_ID: %d\n",
		progdata->bonus_uid);
	if (sem_close(progdata->forksem))
		printf("Failure in sem_close call on forksem in freeme PROC_ID: %d\n",
		progdata->bonus_uid);
	return ;
}

void	exit_success(t_progdata *progdata)
{
	freeme(progdata);
	exit(EXIT_SUCCESS);
}

void	exit_failure(t_progdata *progdata)
{
	freeme(progdata);
	exit(EXIT_FAILURE);
}

/*
** First we zero the progdata struct. Then we unlink the semaphore names we
** intend to use, in case they are still hanging around from one of my many
** botched experiments. xD Then we run setup. If it fails, it will be sure
** to complain to the user about it. If it succeeds, then the threads are all
** running!
**
** We wait for either the death of any philosopher or for all philosophers to be
** full. The bonus implementation uses processes, so waitpid is used to wait for
** all child processes to end.
**
** If all philosophers are dead or full, then they all exit their processes. So
** now we use waitpid to reap them.
**
** Then we do freeme to free dynamically allocated memory and close the
** semaphores.
*/

int	main(int argc, char **argv)
{
	t_progdata	progdata;
	//pid_t		deadproc;
	int			stat_loc;

	pl_bzero(&progdata, sizeof(t_progdata));
	stat_loc = 0;
	sem_unlink("/forksem");
	sem_unlink("/printsem");
	sem_unlink("/waitersem");
	printf("\n");
	if (setup(&progdata, argc, argv))
	{
		size_t i = 0;
		pid_t deadproc;
		i = 0;
		deadproc = waitpid(-1, &stat_loc, WUNTRACED);
		//add deadproc array, more robust check
		//philosopher was full
		if (!stat_loc)
		{
			while  (i++ < (size_t)progdata.number_of_philosophers - 1)
				deadproc = waitpid(-1, &stat_loc, WUNTRACED);
		}
		//philosopher died
		else
		{
			while(i < (size_t)progdata.number_of_philosophers - 1)
			{
				if (progdata.children[i] != deadproc)
					kill(progdata.children[i], SIGTERM);
				i++;
			}
		}
	}
	sem_unlink("/forksem");
	sem_unlink("/printsem");
	sem_unlink("/waitersem");
	if (progdata.philosopher)
		free(progdata.philosopher);
	if  (progdata.children)
		free(progdata.children);
	progdata.philosopher = NULL;
	progdata.children = NULL;
	exit_success(&progdata);
	return (0);
}

// int	main(void)
// {
		
	
// }