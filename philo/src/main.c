/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/07/02 18:25:30 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function checks and frees all memory reserved during run-time and also
** destroys all mutexes.
*/

void	freeme(t_progdata *progdata)
{
	int	mutexc;

	mutexc = progdata->number_of_philosophers;
	//solo if mutex created
	if (pthread_mutex_destroy(&progdata->idlock))
		printf("Failure in pthread_mutex_destroy call on idlock in freeme\n");
	// if (pthread_mutex_destroy(&progdata->waiter))
	// 	printf("Failure in pthread_mutex_destroy call on waiter in freeme\n");
	while (mutexc--)
		if (pthread_mutex_destroy(progdata->waiter + mutexc))
			printf("Failure in pthread_mutex_destroy call on waiter[%d] in freeme\n", mutexc);
	while (progdata->number_of_forks--)
		if (pthread_mutex_destroy(progdata->forks + progdata->number_of_forks))
			printf("Failure in pthread_mutex_destroy call on forks[%d] in freeme\n", progdata->number_of_forks);
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
** This function exits the program after freeing all dynamically allocated
** memory with the status passed as exit_status.
*/

void	exit_program(t_progdata *progdata, int exit_status)
{
	freeme(progdata);
	exit(exit_status);
}


// void	*test_routine(void *test)
// {
// 	size_t i = 0;
// 	for ( ; i < 1000000; i++)
// 	{
// 	}
// 	pthread_mutex_lock(&((t_progdata *)test)->mutex);
// 	((t_progdata *)test)->mails += i;
// 	pthread_mutex_unlock(&((t_progdata *)test)->mutex);
// 	printf("Test from threads\n");
// 	printf("Ending thread\n");
// 	((t_progdata *)test)->res = i;
// 	return (&((t_progdata *)test)->res);
// }

char	hungry_philosophers(t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while (i < (size_t)progdata->number_of_philosophers)
		if (!is_full(progdata, i++))
			return (1);
	return (0);
}

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
	// pthread_create(&progdata.t1, NULL, &test_routine, &progdata);
	// pthread_create(&progdata.t2, NULL, &test_routine, &progdata);
	// pthread_join(progdata.t1, &res);
	// printf("Number of mails incremented by thread 1: %zu\n", *(size_t *)res);
	// pthread_join(progdata.t2, &res);
	// printf("Number of mails incremented by thread 2: %zu\n", *(size_t *)res);
	// pthread_mutex_destroy(&progdata.mutex);
	// printf("Number of mails: %zu\n", progdata.mails);
	// exit_program(&progdata, EXIT_SUCCESS);
	freeme(&progdata);
	return (0);
}
