/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_mod.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:34:20 by miki              #+#    #+#             */
/*   Updated: 2021/07/13 14:45:47 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** This function will do a modulo operation on both positive and negative
** dividends. It will only take positive divisors.
*/

unsigned int	pl_mod(int dividend, unsigned int divisor)
{
	int	rem;

	rem = dividend % divisor;
	if (rem < 0)
		return (rem + divisor);
	return (rem);
}
