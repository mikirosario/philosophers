/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 20:46:09 by mrosario          #+#    #+#             */
/*   Updated: 2021/06/17 23:29:51 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

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
		if (arg[i])
			return (0);
	}
	return (1);
}
