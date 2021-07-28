/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/07/28 10:24:10 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** This function checks and frees all memory reserved during run-time.
*/

void	freeme(t_progdata *progdata)
{
	if (progdata->philosopher)
		free(progdata->philosopher);
	if (progdata->children)
		free(progdata->children);
	if (progdata->deadchildren)
		free(progdata->deadchildren);
	progdata->philosopher = NULL;
	progdata->children = NULL;
	progdata->deadchildren = NULL;
}

/*
** This function closes all semaphores if they have been opened.
**
** Note that beautiful, norminette-friendly style. ;)
*/

void	closeme(t_progdata *progdata)
{
	if (progdata->gotprintsem && sem_close(progdata->printsem))
		printf("Failure in sem_close call on printsem in closeme PROC_ID: %d\n",
			progdata->bonus_uid);
	if (progdata->gotwaitersem && sem_close(progdata->waitersem))
		printf("Failure in sem_close call on waitersem in closeme PROC_ID: %d\n",
			progdata->bonus_uid);
	if (progdata->gotforksem && sem_close(progdata->forksem))
		printf("Failure in sem_close call on forksem in closeme PROC_ID: %d\n",
			progdata->bonus_uid);
	if (progdata->gotreapersem && sem_close(progdata->reapersem))
		printf("Failure in sem_close call on reapersem in closeme PROC_ID: %d\n",
			progdata->bonus_uid);
	return ;
}

/*
** This function tries to close any potentially open semaphores and exits with
** the status passed as status.
*/

void	exit_status(t_progdata *progdata, int status)
{
	freeme(progdata);
	exit(status);
}

/*
** First we zero the progdata struct. Then we unlink the semaphore names we
** intend to use, in case they are still hanging around from one of my many
** botched experiments. xD Then we run setup. If it fails, it will be sure
** to complain to the user about it. If it succeeds, then the processes are all
** running!
**
** Setup will do everything for us. It will allocate all the dynamic memory we
** need, open all the semaphores we'll be using, and spawn all the philosophers
** for us as child processes. If setup fails at any point it will gracefully
** exit and inform the user why.
**
** If setup succeeded that means all child processes are running. Now, in the
** simulation philosophers can die for two reasons: because they are full, or
** because they are starved. Full philosophers do NOT stop the simulation.
** Starved philosophers DO stop the simulation.
**
** Thus, first we wait for full philosophers in the wait_for_full_philosophers
** function. That function will wait to reap all child processes so long as
** philosophers continue to die because they are full. If a child process dies
** of starvation at any point, it will immediately terminate. In either case, it
** will return the number of dead children. The PIDs of all dead children are
** saved to memory.
**
** Next, if the number of dead children after waiting for all the full
** philosophers is less than the number of philosophers, this means that a
** philosopher died of starvation AND there are still surviving child processes.
** So, in that case, we call kill_philosophers to kill the survivors.
**
** Once the simulation terminates we unlink the named semaphores, free all the
** dynamically allocated memory, close any open semaphores, and exit
** triumphantly. Or with EXIT_FAILURE if setup failed.
*/

int	main(int argc, char **argv)
{
	t_progdata	progdata;
	size_t		number_of_dead_children;
	int			res;

	pl_bzero(&progdata, sizeof(t_progdata));
	sem_unlink("/forksem");
	sem_unlink("/printsem");
	sem_unlink("/waitersem");
	sem_unlink("/reapersem");
	res = setup(&progdata, argc, argv);
	if (res)
	{
		number_of_dead_children = wait_for_full_philosophers(&progdata);
		if (number_of_dead_children < (size_t)progdata.number_of_philosophers)
			kill_philosophers(&progdata);
	}
	sem_unlink("/forksem");
	sem_unlink("/printsem");
	sem_unlink("/waitersem");
	sem_unlink("/reapersem");
	closeme(&progdata);
	if (!res)
		exit_status(&progdata, EXIT_FAILURE);
	exit_status(&progdata, EXIT_SUCCESS);
	return (0);
}
