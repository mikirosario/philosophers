/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 20:45:12 by mrosario          #+#    #+#             */
/*   Updated: 2021/06/17 23:28:38 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_lib.h"

/*
** Retrieves the addresses of all arguments except the first (the program name).
**
** When no argument is present, NULL is returned. If all arguments have been
** passed, it cycles back to the beginning.
*/

char	*get_next_arg(int argc, char **argv)
{
	static int	i = 0;

	if (i == argc)
		i = 0;
	return (argv[++i]);
}
