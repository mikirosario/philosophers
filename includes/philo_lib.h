/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_lib.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:53:43 by miki              #+#    #+#             */
/*   Updated: 2021/07/21 23:09:18 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_LIB_H
# define PHILO_LIB_H
# include "ansi_color_codes.h"
# include <semaphore.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philosopher
{
	long long unsigned int	last_meal;
	int						fork1;
	int						fork2;
	int						times_ate;
	char					died;
	char					murdered;
	char					eating;
	char					hasfork1;
	char					hasfork2;

	//experimentos
	char					even;
}				t_philosopher;

typedef struct s_progdata
{
	int						number_of_philosophers;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						number_of_times_each_philosopher_must_eat;
	int						number_of_forks;
	int						argc;
	int						error;
	int						bonus_uid;
	long long int			usec_time_to_eat;
	long long int			usec_time_to_sleep;
	long long int			usec_time_to_die;
	long long unsigned int	time_start;
	int						*deadchildren;
	pid_t					*children;
	sem_t					*killsem;
	sem_t					*forksem;
	sem_t					*waitersem;
	sem_t					*printsem;
	pthread_t				reaper;
	pthread_t				*thread;
	pthread_mutex_t			*forks;
	t_philosopher			*philosopher;
	pthread_mutex_t			idlock;
	pthread_mutex_t			printlock;
	char					gotidlock;
	char					gotprintlock;
	char					gotprintsem;
	char					gotwaitersem;
	char					gotforksem;
	char					stop;
	char					all_full;

	//republica populimpar
	pthread_t				marx;
	pthread_mutex_t			kremlock;
	int						ration_card;
	int						free_forks;
	int						comrades_present;
	int						max_concurrent_eaters;
	char					gotkremlock;
	char					odd_num_philosophers;
	//experimentos
	char					gotwaiter;
	pthread_mutex_t			waiter[2];
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
long long unsigned int	pl_timeval_to_usec(struct timeval *time);
long long unsigned int	pl_get_time_msec(void);
int						pl_strcmp(const char *s1, const char *s2);
void					pl_usleep(long long unsigned int wait);
unsigned int			pl_mod(int dividend, unsigned int divisor);

#endif
