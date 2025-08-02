/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:03:33 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/21 12:50:54 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_t			thread;
	t_rules				*rules;
}						t_philo;

typedef struct s_rules
{
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	int					dead;
	long long			start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print;
	pthread_mutex_t		state_lock;
	t_philo				*philos;
}						t_rules;

long long				get_time(void);
void					precise_sleep(long long duration, t_rules *rules);
int						has_simulation_ended(t_rules *rules);
void					print_status(t_philo *philo, char *msg);
void					*philo_routine(void *arg);
void					*one_philo_routine(void *arg);
void					*monitor(void *arg);
int						init_rules(t_rules *rules, int argc, char **argv);
void					cleanup(t_rules *rules);

#endif