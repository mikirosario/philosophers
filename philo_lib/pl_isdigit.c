
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 17:03:04 by mrosario          #+#    #+#             */
/*   Updated: 2021/03/25 01:59:39 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

int	pl_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
	
}