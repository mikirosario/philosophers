/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/06/26 15:11:15 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** This function checks and frees all memory reserved during run-time and also
** destroys all mutexes.
*/

void	freeme(t_progdata *progdata)
{
	if (pthread_mutex_destroy(&progdata->idlock))
		printf("Failure in pthread_mutex_destroy call on idlock in freeme\n");
	if (pthread_mutex_destroy(&progdata->waiter))
		printf("Failure in pthread_mutex_destroy call on waiter in freeme\n");
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

/*
** This function retrieves all the arguments (except the first) from the
** argument array and converts them into integers in the progdata struct.
*/

void	get_args(int argc, char **argv, t_progdata *progdata)
{
	char	*arg;
	size_t	i;

	i = 0;
	arg = get_next_arg(argc, argv);
	while (arg)
	{
		((int *)(progdata))[i++] = pl_atoi(arg);
		arg = get_next_arg(argc, argv);
	}	
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

int	main(int argc, char **argv)
{
	t_progdata	progdata;
	// void		*res;
	pl_bzero(&progdata, sizeof(t_progdata));
	// progdata.mails = 0;
	if (argc < 5 || argc > 6)
		iamerror(ARG_NUM_ERR, "main", &progdata);
	if (!check_args(argv))
		iamerror(ARG_SYN_ERR, "main", &progdata);
	get_args(argc, argv, &progdata);
	//unit test
	get_args_utest(argc, argv, &progdata);
	//unit test

	printf("\n");

	progdata.philosopher = philo_init(progdata.number_of_philosophers, &progdata);
	progdata.thread = thread_init(progdata.number_of_philosophers, &progdata);
	progdata.forks = fork_init(progdata.number_of_philosophers, &progdata);
	size_t i = 0;
	while (i < (size_t)progdata.number_of_philosophers)
		pthread_join(progdata.thread[i++], NULL);

	// pthread_create(&progdata.t1, NULL, &test_routine, &progdata);
	// pthread_create(&progdata.t2, NULL, &test_routine, &progdata);
	// pthread_join(progdata.t1, &res);
	// printf("Number of mails incremented by thread 1: %zu\n", *(size_t *)res);
	// pthread_join(progdata.t2, &res);
	// printf("Number of mails incremented by thread 2: %zu\n", *(size_t *)res);
	// pthread_mutex_destroy(&progdata.mutex);
	// printf("Number of mails: %zu\n", progdata.mails);
	exit_program(&progdata, EXIT_SUCCESS);
}
