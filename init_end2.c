/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_end2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:00:01 by sstavrop          #+#    #+#             */
/*   Updated: 2025/05/13 11:03:07 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_n_fork_mutexes(t_data *data, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		pthread_mutex_destroy(&data->forks[k]);
		k++;
	}
}

void	destroy_n_philo_meal_locks(t_philo *philos, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		pthread_mutex_destroy(&philos[k].meal_lock);
		k++;
	}
}

void	join_n_created_philo_threads(t_philo *philos, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		if (philos[k].life_tid != 0)
			pthread_join(philos[k].life_tid, NULL);
		k++;
	}
}
