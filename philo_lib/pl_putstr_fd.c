/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:53:23 by miki              #+#    #+#             */
/*   Updated: 2021/06/17 16:57:04 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"
#include <unistd.h>

/*
** This function is meant to be a very fast putstr to stay under the 10 ms
** limit. I'll trot out my ASM version of strlen if I have to... ;)
*/

void	pl_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, pl_strlen(s));
}