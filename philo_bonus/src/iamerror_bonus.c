/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iamerror_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:08:32 by miki              #+#    #+#             */
/*   Updated: 2021/07/08 23:52:19 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	iamerror(int error_code, char *error_func)
{
	if (error_code == MALLOC_ERR)
		printf(RED"Malloc Error in %s\n"RESET, error_func);
	else if (error_code == ARG_NUM_ERR)
		printf(RED"Wrong number of arguments in %s\n"RESET, error_func);
	else if (error_code == ARG_SYN_ERR)
		printf(RED"Argument contains non-digits in %s\n"RESET, error_func);
	else if (error_code == PTHREAD_CREAT_ERR)
		printf(RED"Failure in pthread_create call in %s\n"RESET, error_func);
	else if (error_code == PTHREAD_MUTEX_INIT_ERR)
		printf(RED"Failure in pthread_mutex_init call in %s\n"RESET, error_func);
	else if (error_code == FORK_FAILURE)
		printf(RED"Failure in fork call in %s\n"RESET, error_func);
	else if (error_code == SEM_OPEN_FAIL)
		printf(RED"Failure in sem_open call in %s\n"RESET, error_func);
	return (0);
}
