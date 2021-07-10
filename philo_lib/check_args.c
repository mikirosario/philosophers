/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 20:46:09 by mrosario          #+#    #+#             */
/*   Updated: 2021/07/10 14:18:57 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** If a number passed as a string has 10 digits we check each digit to make sure
** it doesn't go over INT_MAX. This program doesn't handle negative integers so
** we're only interested in positives.
*/

char	number_too_large(char *numstart, char *numend)
{
	size_t	digits;
	size_t	i;
	char	*int_max;

	digits = numend - numstart;
	int_max = "2147483647";
	if (digits == 10)
	{
		i = 0;
		while (i < digits)
		{
			if (numstart[i] > int_max[i])
				return (1);
			i++;
		}
	}
	else if (digits > 10)
		return (1);
	return (0);
}

/*
** Ensures that each argument is a positive integer between 0 and 2147483647
** (INT_MAX). Hardcoded INT_MAX to avoid having to import my itoa.
*/

char	check_args(char **argv)
{
	char	*arg;
	size_t	i;

	while (*++argv)
	{
		arg = *argv;
		i = 0;
		while (pl_isdigit(arg[i]))
			i++;
		if (arg[i] || number_too_large(arg, &arg[i]))
			return (0);
	}
	return (1);
}
