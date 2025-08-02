/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:55:42 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/31 11:20:36 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	wait_for_threads(t_rules *rules)
{
	int			i;
	pthread_t	monitor_thread;

	monitor_thread = 0;
	if (rules->nb_philos > 1)
	{
		if (pthread_create(&monitor_thread, NULL, monitor, rules) != 0)
			return ;
	}
	i = 0;
	while (i < rules->nb_philos)
	{
		pthread_join(rules->philos[i].thread, NULL);
		i++;
	}
	if (monitor_thread)
		pthread_join(monitor_thread, NULL);
}

static int	start_simulation(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philos)
	{
		if (rules->nb_philos == 1)
		{
			if (pthread_create(&rules->philos[i].thread, NULL,
					one_philo_routine, &rules->philos[i]) != 0)
				return (1);
		}
		else
		{
			if (pthread_create(&rules->philos[i].thread, NULL,
					philo_routine, &rules->philos[i]) != 0)
				return (1);
		}
		i++;
	}
	wait_for_threads(rules);
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (argc < 5 || argc > 6)
	{
		write(2, "Invalid\n", 8);
		return (1);
	}
	if (init_rules(&rules, argc, argv) != 0)
	{
		write(2, "Invalid\n", 8);
		return (1);
	}
	if (start_simulation(&rules) != 0)
	{
		cleanup(&rules);
		write(2, "Invalid\n", 8);
		return (1);
	}
	cleanup(&rules);
	return (0);
}
