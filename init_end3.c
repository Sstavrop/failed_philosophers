/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_end3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magnesium <magnesium@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:02:10 by sstavrop          #+#    #+#             */
/*   Updated: 2025/05/24 11:24:38 by magnesium        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	actual_join_all_philosopher_threads(t_data *data)
{
	int	i;

	if (data->philos)
	{
		i = 0;
		while (i < data->nbr_philo)
		{
			if (data->philos[i].life_tid != 0)
				pthread_join(data->philos[i].life_tid, NULL);
			i++;
		}
	}
}

void	actual_destroy_all_fork_mutexes(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->nbr_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
}

void	actual_destroy_all_philo_meal_locks(t_data *data)
{
	int	i;

	if (data->philos)
	{
		i = 0;
		while (i < data->nbr_philo)
		{
			pthread_mutex_destroy(&data->philos[i].meal_lock);
			i++;
		}
	}
}

void	join_threads_and_cleanup(t_data *data)
{
	actual_join_all_philosopher_threads(data);
	actual_destroy_all_fork_mutexes(data);
	actual_destroy_all_philo_meal_locks(data);
	pthread_mutex_destroy(&data->sim_lock);
	pthread_mutex_destroy(&data->print_lock);
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}
