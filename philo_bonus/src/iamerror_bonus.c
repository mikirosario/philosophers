/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iamerror_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:08:32 by miki              #+#    #+#             */
/*   Updated: 2021/07/26 17:44:06 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** This function prints an error message to STDOUT corresponding to the error
** code it is passed, along with the string passed as error_func, which should
** identify the function where the error was thrown.
*/

int	iamerror(int error_code, char *error_func)
{
	if (error_code == MALLOC_ERR)
		printf(RED"Malloc error in %s\n"RESET, error_func);
	else if (error_code == ARG_NUM_ERR)
		printf(RED"Wrong number of arguments in %s\n"RESET, error_func);
	else if (error_code == ARG_SYN_ERR)
		printf(RED"Argument is not a valid integer in %s\n"RESET, error_func);
	else if (error_code == PTHREAD_CREAT_ERR)
		printf(RED"Failure in pthread_create call in %s\n"RESET, error_func);
	else if (error_code == FORK_FAILURE)
		printf(RED"Failure in fork call in %s\n"RESET, error_func);
	else if (error_code == SEM_OPEN_FAIL)
		printf(RED"Failure in sem_open call in %s\n"RESET, error_func);
	return (0);
}
