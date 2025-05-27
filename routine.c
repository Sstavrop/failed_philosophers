/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:46:12 by magensium         #+#    #+#             */
/*   Updated: 2025/05/27 16:08:58 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine_eat(t_philo *ph)
{
	if (ph->data->nbr_philo == 1)
	{
		pthread_mutex_lock(ph->left_fork);
		print_action(ph, FORK);
		ft_usleep(ph->data->time_to_die + 10, ph->data);
		pthread_mutex_unlock(ph->left_fork);
		return ;
	}
	take_forks(ph);
	pthread_mutex_lock(&ph->meal_lock);
	ph->last_meal_time = get_time();
	ph->meals_eaten++;
	print_action(ph, EAT);
	pthread_mutex_unlock(&ph->meal_lock);
	ft_usleep(ph->data->time_to_eat, ph->data);
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
}

void	routine_sleep(t_philo *ph)
{
	print_action(ph, SLEEP);
	ft_usleep(ph->data->time_to_sleep, ph->data);
}

void	routine_think(t_philo *ph)
{
	print_action(ph, THINK);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*ph;
	int		sim_is_ready;

	ph = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&ph->data->sim_lock);
		sim_is_ready = ph->data->ready;
		if (ph->data->over)
		{
			pthread_mutex_unlock(&ph->data->sim_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&ph->data->sim_lock);
		if (sim_is_ready)
			break ;
		usleep(100);
	}
	if (ph->id % 2 == 0)
		ft_usleep(1, ph->data);
	execute_philosopher_routine(ph);
	return (NULL);
}

void	execute_philosopher_routine(t_philo *ph)
{
	while (!is_simulation_over(ph->data))
	{
		routine_eat(ph);
		if (is_simulation_over(ph->data))
			break ;
		if (ph->data->check_meal)
		{
			pthread_mutex_lock(&ph->meal_lock);
			if (ph->meals_eaten >= ph->data->loop)
			{
				pthread_mutex_unlock(&ph->meal_lock);
				break ;
			}
			pthread_mutex_unlock(&ph->meal_lock);
		}
		routine_sleep(ph);
		if (is_simulation_over(ph->data))
			break ;
		routine_think(ph);
	}
}
