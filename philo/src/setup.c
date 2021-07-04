/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 19:01:24 by miki              #+#    #+#             */
/*   Updated: 2021/07/04 03:22:40 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	progdata->argc = argc;
}

/*
** This function attempts to do all necessary prior setup and launch the
** threads. If it succeeds, 1 is returned. If it fails for any reason, 0 is
** returned. The iamerror function always returns 0. The init functions return
** 1 if successful and otherwise 0, so we AND them all together. If any of them
** return 0, we return 0. If they all return 1, we return 1.
**
** REMOVE UNIT_TEST FROM FINAL VERSION.
*/

int	setup(t_progdata *progdata, int argc, char **argv)
{
	pl_bzero(progdata, sizeof(t_progdata));
	if (argc < 5 || argc > 6)
		return (iamerror(ARG_NUM_ERR, "main"));
	else if (!check_args(argv))
		return (iamerror(ARG_SYN_ERR, "main"));
	else
		get_args(argc, argv, progdata);
	// //unit test
	// get_args_utest(argc, argv, &progdata);
	// //unit test

	printf("\n");
	return (philo_init(progdata->number_of_philosophers, progdata) & \
	waiter_init(progdata->number_of_philosophers, progdata) & \
	fork_init(progdata->number_of_philosophers, progdata) & \
	thread_init(progdata->number_of_philosophers, progdata));
}
