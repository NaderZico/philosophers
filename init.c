/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:04:01 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/20 15:18:55 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(const char *s)
{
	long	n;

	n = 0;
	if (!s || !*s)
		return (-1);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (-1);
		n = n * 10 + (*s - '0');
		if (n > 2147483647)
			return (-1);
		s++;
	}
	return ((int)n);
}

static int	init_mutexes(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philos);
	if (!rules->forks)
		return (1);
	i = 0;
	while (i < rules->nb_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&rules->print, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&rules->state_lock, NULL) != 0)
		return (1);
	return (0);
}

static int	init_philosophers(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->nb_philos);
	if (!rules->philos)
		return (1);
	i = 0;
	while (i < rules->nb_philos)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].last_meal = rules->start_time;
		rules->philos[i].left_fork = &rules->forks[i];
		rules->philos[i].right_fork = &rules->forks[(i + 1) % rules->nb_philos];
		rules->philos[i].rules = rules;
		i++;
	}
	return (0);
}

int	init_rules(t_rules *rules, int argc, char **argv)
{
	rules->nb_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		rules->must_eat_count = ft_atoi(argv[5]);
	else
		rules->must_eat_count = -1;
	if ((rules->nb_philos <= 0 || rules->nb_philos > 250)
		|| rules->time_to_die <= 0 || rules->time_to_eat <= 0
		|| rules->time_to_sleep <= 0 || (argc == 6
			&& rules->must_eat_count <= 0))
		return (1);
	rules->dead = 0;
	rules->start_time = get_time();
	if (init_mutexes(rules))
		return (1);
	if (init_philosophers(rules))
	{
		cleanup(rules);
		return (1);
	}
	return (0);
}
