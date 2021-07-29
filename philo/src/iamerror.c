/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iamerror.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikiencolor <mikiencolor@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:08:32 by miki              #+#    #+#             */
/*   Updated: 2021/07/29 16:35:30 by mikiencolor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Pretty self-explanatory, no? ;)
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
	else if (error_code == PTHREAD_MUTEX_INIT_ERR)
		printf(RED"Failure in pthread_mutex_init call in %s\n"RESET, error_func);
	else if (error_code == NO_PHILOS)
		printf(RED"Are zero philosophers immortal?\n"RESET);
	return (0);
}
