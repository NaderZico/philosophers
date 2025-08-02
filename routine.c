/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:02:40 by nakhalil          #+#             */
/*   Updated: 2025/07/02 17:26:34 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}

static void	eat_sleep_think(t_philo *philo)
{
	usleep(500);
	take_forks(philo);
	pthread_mutex_lock(&philo->rules->state_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->rules->state_lock);
	print_status(philo, "is eating");
	precise_sleep(philo->rules->time_to_eat, philo->rules);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_status(philo, "is sleeping");
	precise_sleep(philo->rules->time_to_sleep, philo->rules);
	print_status(philo, "is thinking");
	usleep(500);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		usleep(100);
	if (philo->rules->nb_philos % 2 != 0 && philo->id % 2 != 0)
		precise_sleep(philo->rules->time_to_eat, philo->rules);
	else
		precise_sleep((philo->id % 2) * philo->rules->time_to_eat / 2,
			philo->rules);
	while (!has_simulation_ended(philo->rules))
	{
		eat_sleep_think(philo);
		if (philo->rules->must_eat_count != -1
			&& philo->meals_eaten >= philo->rules->must_eat_count)
			break ;
	}
	return (NULL);
}

void	*one_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	precise_sleep(philo->rules->time_to_die, philo->rules);
	print_status(philo, "died");
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

// static void	take_forks(t_philo *philo)
// {
// 	pthread_mutex_t *L = philo->left_fork;
// 	pthread_mutex_t *R = philo->right_fork;

// 	if (philo->id % 2 == 0)
// 	{
// 		pthread_mutex_lock(L);
// 		print_status(philo, "has taken a fork");
// 		pthread_mutex_lock(R);
// 		print_status(philo, "has taken a fork");
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(R);
// 		print_status(philo, "has taken a fork");
// 		pthread_mutex_lock(L);
// 		print_status(philo, "has taken a fork");
// 	}
// }

// static void	eat_sleep_think(t_philo *philo)
// {
// 	usleep(100);
// 	take_forks(philo);

// 	pthread_mutex_lock(&philo->rules->state_lock);
// 	philo->last_meal   = get_time();
// 	philo->meals_eaten++;
// 	pthread_mutex_unlock(&philo->rules->state_lock);

// 	print_status(philo, "is eating");
// 	precise_sleep(philo->rules->time_to_eat, philo->rules);

// 	pthread_mutex_unlock(philo->left_fork);
// 	pthread_mutex_unlock(philo->right_fork);

// 	print_status(philo, "is sleeping");
// 	precise_sleep(philo->rules->time_to_sleep, philo->rules);

// 	print_status(philo, "is thinking");
// }

// void	*philo_routine(void *arg)
// {
// 	t_philo    *philo  = arg;
// 	t_rules    *rules  = philo->rules;
// 	long long  micro   = rules->time_to_eat / 4;

// 	while (!has_simulation_ended(rules))
// 	{
// 		/* tiny micro-stagger for odd IDs when count is odd */
// 		if ((rules->nb_philos % 2 != 0) && (philo->id % 2 != 0))
// 			precise_sleep(micro, rules);

// 		eat_sleep_think(philo);

// 		if (rules->must_eat_count != -1
// 		 && philo->meals_eaten >= rules->must_eat_count)
// 			break ;
// 	}
// 	return (NULL);
// }

// void	*one_philo_routine(void *arg)
// {
// 	t_philo *philo = arg;

// 	pthread_mutex_lock(philo->left_fork);
// 	print_status(philo, "has taken a fork");
// 	precise_sleep(philo->rules->time_to_die, philo->rules);
// 	print_status(philo, "died");
// 	pthread_mutex_unlock(philo->left_fork);
// 	return (NULL);
// }
