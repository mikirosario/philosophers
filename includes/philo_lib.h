/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_lib.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:53:43 by miki              #+#    #+#             */
/*   Updated: 2021/06/17 22:50:15 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_LIB_H
# define PHILO_LIB_H
# include "ansi_color_codes.h"
# include <stdlib.h>

typedef struct s_philosopher
{
	char	has_fork : 1;
	char	is_eating : 1;
	char	is_sleeping : 1;
	char	is_thinking : 1;
	char	died : 1;
}				t_philosopher;

typedef struct s_waiter
{
}				t_waiter;

typedef struct s_progdata
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	t_philosopher	**philosopher;
}				t_progdata;

char	check_args(char **argv);
char	*get_next_arg(int argc, char **argv);
int		pl_atoi(char const *str);
int		pl_isdigit(int c);
void	pl_putstr_fd(char *s, int fd);
size_t	pl_strlen(char const *s);

#endif
