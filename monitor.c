/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:03:55 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/07 19:32:49 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	should_skip_philo(t_rules *rules, int i)
{
	int	skip;

	pthread_mutex_lock(&rules->state_lock);
	if (rules->must_eat_count > 0
		&& rules->philos[i].meals_eaten >= rules->must_eat_count)
		skip = 1;
	else
		skip = 0;
	pthread_mutex_unlock(&rules->state_lock);
	return (skip);
}

static int	check_philo_death(t_rules *rules, int i)
{
	long long	time_since_meal;
	int			died;

	if (should_skip_philo(rules, i))
		return (0);
	pthread_mutex_lock(&rules->state_lock);
	time_since_meal = get_time() - rules->philos[i].last_meal;
	if (time_since_meal > rules->time_to_die)
	{
		rules->dead = 1;
		died = 1;
	}
	else
		died = 0;
	pthread_mutex_unlock(&rules->state_lock);
	if (died)
	{
		pthread_mutex_lock(&rules->print);
		printf("%lld %d died\n", get_time() - rules->start_time,
			rules->philos[i].id);
		pthread_mutex_unlock(&rules->print);
	}
	return (died);
}

static int	check_meal_completion(t_rules *rules)
{
	int	i;
	int	all_full;

	all_full = 1;
	pthread_mutex_lock(&rules->state_lock);
	i = 0;
	while (i < rules->nb_philos)
	{
		if (rules->philos[i].meals_eaten < rules->must_eat_count)
		{
			all_full = 0;
			break ;
		}
		i++;
	}
	if (all_full)
		rules->dead = 1;
	pthread_mutex_unlock(&rules->state_lock);
	return (all_full);
}

void	*monitor(void *arg)
{
	t_rules	*rules;
	int		i;

	rules = (t_rules *)arg;
	while (!has_simulation_ended(rules))
	{
		i = 0;
		while (i < rules->nb_philos)
		{
			if (has_simulation_ended(rules))
				break ;
			if (check_philo_death(rules, i))
				return (NULL);
			i++;
		}
		if (rules->must_eat_count > 0 && check_meal_completion(rules))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
