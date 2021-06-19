/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/06/19 18:27:40 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** This function checks and frees all memory reserved during run-time.
*/

void	freeme(t_progdata *progdata)
{
	(void)progdata;
	return ;
}

/*
** This function exits with the error message passed as error_msg after freeing
** all memory.
*/

void	exit_failure(char *error_msg, t_progdata *progdata)
{
	printf(RED"%s\n"RESET, error_msg);
	freeme(progdata);
	exit(EXIT_FAILURE);
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


void	*test_routine(void *test)
{
	(void)test;
	printf("Test from threads\n");
	sleep(3);
	printf("Ending thread\n");
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_progdata	progdata;

	(void)argv;
	if (argc < 5 || argc > 6)
		exit_failure(RED"Wrong number of arguments"RESET, &progdata);
	if (!check_args(argv))
		exit_failure(RED"Bad arguments"RESET, &progdata);
	get_args(argc, argv, &progdata);
	//unit test
	get_args_utest(argc, argv, &progdata);
	//unit test

	printf("\n");
	pthread_create(&progdata.t1, NULL, &test_routine, NULL);
	pthread_create(&progdata.t2, NULL, &test_routine, NULL);
	pthread_join(progdata.t1, NULL);
	pthread_join(progdata.t2, NULL);
	return (1);
}
