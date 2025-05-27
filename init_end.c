/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:45:41 by magensium         #+#    #+#             */
/*   Updated: 2025/05/13 11:02:32 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(int argc, char **argv, t_data *data)
{
	memset(data, 0, sizeof(t_data));
	data->nbr_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (data->nbr_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (0);
	data->loop = -1;
	data->check_meal = 0;
	if (argc == 6)
	{
		data->loop = ft_atoi(argv[5]);
		if (data->loop <= 0)
			return (0);
		data->check_meal = 1;
	}
	data->ready = 0;
	data->over = 0;
	return (1);
}

int	init_fork_mutex_array(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			destroy_n_fork_mutexes(data, i);
			free(data->forks);
			data->forks = NULL;
			error_msg("Err: Fork mutex init failed", data);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_mutexes(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	if (!data->forks)
		return (error_msg("Err: Fork mutex malloc failed", data), 0);
	if (!init_fork_mutex_array(data))
		return (0);
	if (pthread_mutex_init(&data->sim_lock, NULL) != 0)
	{
		destroy_n_fork_mutexes(data, data->nbr_philo);
		free(data->forks);
		data->forks = NULL;
		error_msg("Err: Sim_lock init failed", data);
		return (0);
	}
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		destroy_n_fork_mutexes(data, data->nbr_philo);
		free(data->forks);
		data->forks = NULL;
		pthread_mutex_destroy(&data->sim_lock);
		error_msg("Err: Print_lock init failed", data);
		return (0);
	}
	return (1);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nbr_philo);
	if (!data->philos)
		return (error_msg("Err: Philos array malloc failed", data), 0);
	i = 0;
	while (i < data->nbr_philo)
	{
		data->philos[i].id = i;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nbr_philo];
		data->philos[i].life_tid = 0;
		if (pthread_mutex_init(&data->philos[i].meal_lock, NULL) != 0)
		{
			destroy_n_philo_meal_locks(data->philos, i);
			free(data->philos);
			data->philos = NULL;
			return (error_msg("Err: Philo meal_lock init failed", data), 0);
		}
		i++;
	}
	return (1);
}

int	start_threads(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->nbr_philo)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		data->philos[i].last_meal_time = data->start_time;
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		if (pthread_create(&data->philos[i].life_tid, NULL,
				&philosopher_routine, &data->philos[i]) != 0)
		{
			pthread_mutex_lock(&data->sim_lock);
			data->over = 1;
			pthread_mutex_unlock(&data->sim_lock);
			join_n_created_philo_threads(data->philos, i);
			return (error_msg("Err: Thread creation failed", data), 0);
		}
		i++;
	}
	pthread_mutex_lock(&data->sim_lock);
	data->ready = 1;
	pthread_mutex_unlock(&data->sim_lock);
	return (1);
}
