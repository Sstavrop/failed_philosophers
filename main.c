/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:45:59 by sstavrop          #+#    #+#             */
/*   Updated: 2025/05/30 16:46:41 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	perform_initial_setup(int argc, char **argv, t_data *data)
{
	int	i;

	if (!init_data(argc, argv, data))
		return (0);
	if (!init_mutexes(data))
		return (0);
	if (!init_philos(data))
	{
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
		pthread_mutex_destroy(&data->sim_lock);
		pthread_mutex_destroy(&data->print_lock);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		error_occurred_during_runtime;

	error_occurred_during_runtime = 0;
	memset(&data, 0, sizeof(t_data));
	if (!args_check(argc, argv))
		return (1);
	if (!perform_initial_setup(argc, argv, &data))
		return (1);
	if (!start_threads(&data))
		error_occurred_during_runtime = 1;
	if (error_occurred_during_runtime == 0)
		monitor_philos(&data);
	join_threads_and_cleanup(&data);
	if (error_occurred_during_runtime == 1)
		return (1);
	return (0);
}
