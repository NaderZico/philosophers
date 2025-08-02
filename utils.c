/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:02:49 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/07 19:35:39 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	precise_sleep(long long duration, t_rules *rules)
{
	long long	start;

	start = get_time();
	while (!has_simulation_ended(rules))
	{
		if (get_time() - start >= duration)
			break ;
		usleep(500);
	}
}

int	has_simulation_ended(t_rules *rules)
{
	int	ended;

	pthread_mutex_lock(&rules->state_lock);
	ended = rules->dead;
	pthread_mutex_unlock(&rules->state_lock);
	return (ended);
}

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print);
	if (!has_simulation_ended(philo->rules))
		printf("%lld %d %s\n", get_time() - philo->rules->start_time, philo->id,
			msg);
	pthread_mutex_unlock(&philo->rules->print);
}

void	cleanup(t_rules *rules)
{
	int	i;

	if (!rules)
		return ;
	if (rules->forks)
	{
		i = -1;
		while (++i < rules->nb_philos)
			pthread_mutex_destroy(&rules->forks[i]);
		free(rules->forks);
	}
	pthread_mutex_destroy(&rules->print);
	pthread_mutex_destroy(&rules->state_lock);
	if (rules->philos)
		free(rules->philos);
}
