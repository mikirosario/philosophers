/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:31:51 by miki              #+#    #+#             */
/*   Updated: 2021/07/28 23:36:56 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <string.h>

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
	progdata->waitersem = sem_open("/waitersem", \
	O_CREAT | O_EXCL, 0644, number_of_forks / 2);
	if (progdata->waitersem == SEM_FAILED)
		return (iamerror(SEM_OPEN_FAIL, "waiter_init"));
	else
		progdata->gotwaitersem = 1;
	return (1);
}

/*
** This function opens the semaphore that represents the pile of forks in the
** middle of the table. So basically it's just a semaphore initialized to
** number_of_forks. We name it /forksem. The address is saved at
** progdata->forksem.
**
** If the sem_open function fails, an appropriate error message is displayed
** and 0 is returned. Otherwise, 1 is returned.
*/

int	fork_init(int number_of_forks, t_progdata *progdata)
{
	progdata->forksem = sem_open("/forksem", \
	O_CREAT | O_EXCL, 0644, number_of_forks);
	if (progdata->forksem == SEM_FAILED)
		return (iamerror(SEM_OPEN_FAIL, "fork_init"));
	else
		progdata->gotforksem = 1;
	return (1);
}

/*
** This function spawns all the child processes.
**
** First we create an array to store the pid of every child process we will
** create.
**
** Next we initialize time by getting a timestamp, which we save at
** progdata->time_start. We'll subtract this from future timestamps to get the
** relative times for all processes, so this time counts as the "simulation
** start".
**
** For each philosopher we fork a new process in the while. Child processes
** break away from the while and start their new lives by calling the life_cycle
** function. Failed forks cause the parent process to break from the while and
** display an error message before returning 0. We also increment the bonus_uid
** integer between fork calls, which will allow each process to identify itself.
**
** Do not confuse the fork function with the forks on the table... two
** completely different concepts. xD
**
** If all philosophers are successfully forked into child processes we leave the
** while with number_of_philosophers == -1. The parent will then return 1.
**
** Child processes will continue in life_cycle. Parent process will return 0 or
** 1 as failed or successful.
*/

int	proc_init(int number_of_philosophers, t_progdata *progdata)
{
	progdata->children = malloc(number_of_philosophers * sizeof(pid_t));
	if (progdata->children == NULL)
		return (iamerror(MALLOC_ERR, "proc_init"));
	memset(progdata->children, -1, number_of_philosophers * sizeof(pid_t));
	progdata->time_start = pl_get_time_msec();
	while (number_of_philosophers-- > 0)
	{
		progdata->children[progdata->bonus_uid] = fork();
		if (progdata->children[progdata->bonus_uid] < 1)
			break ;
		progdata->bonus_uid++;
	}
	if (number_of_philosophers == -1)
		return (1);
	else if (progdata->children[progdata->bonus_uid] == 0)
		life_cycle(progdata);
	return (iamerror(FORK_FAILURE, "proc_init"));
}

/*
** PHILOSOPHER STRUCT ARRAY NOT NEEDED FOR BONUS; REMOVE!
** This function reserves memory for an array of t_philosopher structs
** corresponding to each philosopher, which will be used to store the particular
** status of each philosopher. They are zeroed upon creation.
** PHILOSOPHER STRUCT ARRAY NOT NEEDED FOR BONUS; REMOVE!
**
** This function will create the deadchildren array. This array is used to keep
** track of children that have exited because they are full. If a child exits
** because it is full and later another child dies and we kill all the remaining
** child processes, we will skip over the children that already terminated by
** checking for their pid in the deadchildren array.
**
** Processes will never have the pid 0, so we initialize the array with 0.
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

	progdata->philosopher = \
	malloc(number_of_philosophers * sizeof(t_philosopher));
	if (progdata->philosopher == NULL)
		return (iamerror(MALLOC_ERR, "philo_init"));
	progdata->deadchildren = \
	malloc(progdata->number_of_philosophers * sizeof(int));
	if (progdata->deadchildren == NULL)
		return (iamerror(MALLOC_ERR, "philo_init"));
	pl_bzero(progdata->deadchildren, \
	progdata->number_of_philosophers * sizeof(int));
	i = 0;
	while (i < (size_t)number_of_philosophers)
		pl_bzero(&progdata->philosopher[i++], sizeof(t_philosopher));
	progdata->printsem = sem_open("/printsem", O_CREAT | O_EXCL, 0644, 1);
	if (progdata->printsem == SEM_FAILED)
		return (iamerror(SEM_OPEN_FAIL, "philo_init"));
	else
		progdata->gotprintsem = 1;
	return (1);
}
