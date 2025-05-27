/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:03:48 by sstavrop          #+#    #+#             */
/*   Updated: 2025/05/27 16:04:11 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *ph)
{
	if (ph->id == 0)
	{
		pthread_mutex_lock(ph->right_fork);
		print_action(ph, FORK);
		if (is_simulation_over(ph->data))
		{
			pthread_mutex_unlock(ph->right_fork);
			return ;
		}
		pthread_mutex_lock(ph->left_fork);
	}
	else
	{
		pthread_mutex_lock(ph->left_fork);
		print_action(ph, FORK);
		if (is_simulation_over(ph->data))
		{
			pthread_mutex_unlock(ph->left_fork);
			return ;
		}
		pthread_mutex_lock(ph->right_fork);
	}
	print_action(ph, FORK);
}
