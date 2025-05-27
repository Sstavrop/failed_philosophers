/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magnesium <magnesium@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:41:41 by magensium         #+#    #+#             */
/*   Updated: 2025/05/11 12:50:09 by magnesium        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor_philos(t_data *dt)
{
	if (wait_for_simulation_ready(dt))
		return ;
	while (1)
	{
		if (check_all_deaths(dt))
			return ;
		if (dt->check_meal)
		{
			if (check_all_philos_eaten(dt))
				return ;
		}
		if (is_simulation_over(dt))
			return ;
		usleep(1000);
	}
}

int	check_philo_death(t_philo *ph)
{
	long int	time_since_last_meal;
	long int	local_last_meal_time;

	pthread_mutex_lock(&ph->meal_lock);
	local_last_meal_time = ph->last_meal_time;
	pthread_mutex_unlock(&ph->meal_lock);
	time_since_last_meal = get_time() - local_last_meal_time;
	if (time_since_last_meal >= ph->data->time_to_die)
	{
		pthread_mutex_lock(&ph->data->sim_lock);
		if (ph->data->over)
		{
			pthread_mutex_unlock(&ph->data->sim_lock);
			return (0);
		}
		ph->data->over = 1;
		pthread_mutex_unlock(&ph->data->sim_lock);
		print_action(ph, DIE);
		return (1);
	}
	return (0);
}

int	all_philos_completed_meals(t_data *dt)
{
	int	i;
	int	one_philo_still_eating;

	i = 0;
	one_philo_still_eating = 0;
	while (i < dt->nbr_philo)
	{
		pthread_mutex_lock(&dt->philos[i].meal_lock);
		if (dt->philos[i].meals_eaten < dt->loop)
		{
			one_philo_still_eating = 1;
			pthread_mutex_unlock(&dt->philos[i].meal_lock);
			break ;
		}
		pthread_mutex_unlock(&dt->philos[i].meal_lock);
		i++;
	}
	if (one_philo_still_eating == 1)
		return (0);
	return (1);
}

int	check_all_philos_eaten(t_data *dt)
{
	if (!dt->check_meal)
		return (0);
	if (all_philos_completed_meals(dt))
	{
		pthread_mutex_lock(&dt->sim_lock);
		if (dt->over)
		{
			pthread_mutex_unlock(&dt->sim_lock);
			return (0);
		}
		dt->over = 1;
		pthread_mutex_unlock(&dt->sim_lock);
		pthread_mutex_lock(&dt->print_lock);
		printf("All philosophers have eaten %d time(s).\n", dt->loop);
		pthread_mutex_unlock(&dt->print_lock);
		return (1);
	}
	return (0);
}

int	is_simulation_over(t_data *data)
{
	int	over_status;

	pthread_mutex_lock(&data->sim_lock);
	over_status = data->over;
	pthread_mutex_unlock(&data->sim_lock);
	return (over_status);
}
