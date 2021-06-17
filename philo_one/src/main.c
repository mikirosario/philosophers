/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:41:57 by miki              #+#    #+#             */
/*   Updated: 2021/06/17 23:31:58 by mrosario         ###   ########.fr       */
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
	return (1);
}
