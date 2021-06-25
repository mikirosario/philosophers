/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 23:04:44 by mrosario          #+#    #+#             */
/*   Updated: 2021/06/25 17:48:14 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** This is a unit test for the argument retrieval functionality.
**
** The variables in progdata are compared with the arguments to ensure they
** were properly caught.
*/

void	get_args_utest(int argc, char **argv, t_progdata *progdata)
{
	size_t	i;

	i = 0;
	while ((int)i < argc - 1)
		if (((int *)(progdata))[i++] != pl_atoi(*++argv))
		{
			printf(RED"KO: get_args unit test"RESET);
			exit_program(progdata, EXIT_FAILURE);
		}
	printf(GRN"OK: get_args unit test"RESET);
}