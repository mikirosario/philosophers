/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 21:44:23 by mrosario          #+#    #+#             */
/*   Updated: 2021/06/17 22:55:56 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** This used to be the work of a ternary, until they were banned by 42 school.
** Need I say more? Need I roll my eyes any further back into my head?
*/

static int	norminette_made_me_do_it(int sign)
{
	if (sign == 1)
		return (-1);
	else
		return (0);
}

int	pl_atoi(char const *str)
{
	char const			*ptr;
	int					base;
	long unsigned int	res;
	int					sign;

	ptr = str;
	base = 10;
	sign = 1;
	res = 0;
	while (*ptr == '\t' || *ptr == '\n' || *ptr == '\v'
		|| *ptr == '\f' || *ptr == '\r' || *ptr == ' ')
		ptr++;
	if (*ptr == '-')
	{
		ptr++;
		sign = -1;
	}
	else if (*ptr == '+')
		ptr++;
	while (pl_isdigit(*ptr))
		res = (res * base) + (*ptr++ - 48);
	if (res < 2147483649)
		return ((int)res * sign);
	return (norminette_made_me_do_it(sign));
}
