/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_lib.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:53:43 by miki              #+#    #+#             */
/*   Updated: 2021/06/26 14:54:56 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_LIB_H
# define PHILO_LIB_H
# include "ansi_color_codes.h"
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philosopher
{
	char	has_fork : 1;
	char	is_eating : 1;
	char	is_sleeping : 1;
	char	is_thinking : 1;
	char	died : 1;
}				t_philosopher;

typedef struct s_progdata
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				number_of_forks;
	long long int	usec_time_to_eat;
	long long int	usec_time_to_sleep;
	pthread_t		*thread;
	pthread_mutex_t	*forks;
	t_philosopher	*philosopher;
	pthread_mutex_t idlock;
	pthread_mutex_t	waiter;
	
	//Tests
	// pthread_t		t1;
	// pthread_t		t2;
	// size_t			mails;
	// size_t			res;
	//Tests
}				t_progdata;

char					check_args(char **argv);
char					*get_next_arg(int argc, char **argv);
int						pl_atoi(char const *str);
int						pl_isdigit(int c);
void					pl_putstr_fd(char *s, int fd);
size_t					pl_strlen(char const *s);
void					*pl_bzero(void *s, size_t n);
long long unsigned int	pl_msec_diff(struct timeval *t0, struct timeval *t1);
long long unsigned int	pl_timeval_to_msec(struct timeval *time);
long long unsigned int	pl_get_time_msec(void);

#endif
