/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:31:51 by miki              #+#    #+#             */
/*   Updated: 2021/07/10 01:05:09 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <fcntl.h>
#include <sys/stat.h>

/*
** This function opens the waiter semaphore. The waiter semaphore controls
** philosopher access to forks. Each philosopher needs 2 forks to eat. Thus,
** there are always number_of_forks / 2 waiters. That is to say, for every pair
** of forks there is 1 waiter. In order to be allowed to take forks from the
** pile, philosophers need the permission of a waiter. Once they get the
** permission of a waiter they will hold that waiter's "attention" until they
** put their forks down. If less than 2 forks are left in the pile, then all
** waiters will be occupied and any hungry philosophers will have to wait for
** another philosopher to put their forks down to get a waiter's attention.
**
** If the sem_open function fails, an appropriate error message is displayed
** and 0 is returned. Otherwise, 1 is returned. 
*/

int	waiter_init(int	number_of_forks, t_progdata *progdata)
{
	progdata->waitersem = sem_open("/waitersem", O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, number_of_forks / 2);
	if (progdata->waitersem == SEM_FAILED)
		return (iamerror(SEM_OPEN_FAIL, "waiter_init"));
	return (1);
}

/*
** This function opens the semaphore that represents the pile of forks in the
** middle of the table. So basically it's just a semaphore initialized to
** nuimber_of_forks. We name it /forksem. The address is saved at
** progdata->forksem.
**
** If the sem_open function fails, an appropriate error message is displayed
** and 0 is returned. Otherwise, 1 is returned.
*/

int	fork_init(int number_of_forks, t_progdata *progdata)
{
	progdata->forksem = sem_open("/forksem", O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, number_of_forks);
	if (progdata->forksem == SEM_FAILED)
		return (iamerror(SEM_OPEN_FAIL, "fork_init"));
	return (1);
}

/*
** This function spawns all the child processes. First we initialize time by
** getting a timestamp, which we save at time_start. We'll subtract this from
** future time_stamps to get the relative times for all processes, so this time
** counts as the "simulation start".
**
** For each philosopher we fork a new process in the while. Child processes
** break away from the while and start their new lives by calling the life_cycle
** function. Failed forks cause the parent process to break from the while and
** display an error message before returning 0. We also increment the bonus_uid
** int between fork calls, which will allow each process to identify itself.
**
** Do not confuse the fork function with the forks on the table... two
** completely different concepts. xD
**
** If all philosophers are successfully forked into child processes we leave the
** while with the pid of the last child. I might save the pids in an array or
** something, but I haven't really seen the need. Since we're allowed exit for
** this project, I just have the processes kill themselves as needed.
**
** Child processes will continue in life_cycle. Parent process will return.
*/

int	proc_init(int number_of_philosophers, t_progdata *progdata)
{
	//pid_t	pid;

	progdata->children = malloc(number_of_philosophers * sizeof(pid_t));
	if (progdata->children == NULL)
		return (iamerror(MALLOC_ERR, "proc_init"));
	// // MI GOZO EN UN POZO
	// progdata->killsem = sem_open("/killsem", O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, 0);
	// if (progdata->killsem == SEM_FAILED)
	// 	return (iamerror(SEM_OPEN_FAIL, "proc_init"));
 	// // MI GOZO EN UN POZO
	 progdata->time_start = pl_get_time_msec();
	while (number_of_philosophers-- > 0)
	{
		progdata->children[progdata->bonus_uid] = fork();
		if (progdata->children[progdata->bonus_uid] < 1) //soy hijo o ha habido error
			break ;
		progdata->bonus_uid++;
	}
	if (number_of_philosophers == -1)
		return (1);
	else if (progdata->children[progdata->bonus_uid] == 0) //soy hijo -> empieza nueva vida
		life_cycle(progdata);
//soy padre y ha habido error -> aborta
	return (iamerror(FORK_FAILURE, "proc_init"));
}

/*
** PHILOSOPHER STRUCT ARRAY NOT NEEDED FOR BONUS; REMOVE!
** This function reserves memory for an array of t_philosopher structs
** corresponding to each philosopher, which will be used to store the particular
** status of each philosopher. They are zeroed upon creation.
** PHILOSOPHER STRUCT ARRAY NOT NEEDED FOR BONUS; REMOVE!
**
** This function will create the print semaphore. The print semaphore is a
** binary named semaphore that works essentially like a mutex and will be used
** to regulate exclusive access to stdout from all processes.
**
** The time_to_eat, time_to_sleep and time_to_die variables passed by the user
** are transformed to microseconds for use with the usleep function.
**
** If print semaphore creation fails, an error message is displayed and 0 is
** returned. Otherwise 1 is returned.
*/

int	philo_init(int number_of_philosophers, t_progdata *progdata)
{
	size_t	i;

	progdata->philosopher = malloc(number_of_philosophers * sizeof(t_philosopher));
	if (progdata->philosopher == NULL)
		return (iamerror(MALLOC_ERR, "philo_init"));
	i = 0;
	while (i < (size_t)number_of_philosophers)
		pl_bzero(&progdata->philosopher[i++], sizeof(t_philosopher));
	progdata->printsem = sem_open("/printsem", O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, 1);
	if (progdata->printsem == SEM_FAILED)
		return (iamerror(SEM_OPEN_FAIL, "philo_init"));
	progdata->usec_time_to_eat = progdata->time_to_eat * 1000;
	progdata->usec_time_to_sleep = progdata->time_to_sleep * 1000;
	progdata->usec_time_to_die = progdata->time_to_die * 1000;
	return (1);
}
	