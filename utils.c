/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:47:04 by magensium         #+#    #+#             */
/*   Updated: 2025/05/30 15:24:41 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	i;
	int	res;

	sign = 1;
	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		i++;
	}
	return (sign * res);
}

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*tab;
	unsigned long	i;
	unsigned long	total_size;

	total_size = count * size;
	if (count != 0 && total_size / count != size)
		return (NULL);
	tab = malloc(total_size);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < total_size)
	{
		tab[i] = 0;
		i++;
	}
	return (tab);
}

long int	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		perror("gettimeofday failed");
		return (0);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(long int milliseconds, t_data *data)
{
	long int	start_time;
	long int	time_elapsed;
	long int	time_to_wait;

	start_time = get_time();
	while (1)
	{
		if (is_simulation_over(data))
			return (0);
		time_elapsed = get_time() - start_time;
		if (time_elapsed >= milliseconds)
			break ;
		time_to_wait = (milliseconds - time_elapsed);
		if (time_to_wait > 5)
			usleep(500);
		else if (time_to_wait > 0)
			usleep(time_to_wait * 1000);
		else
			usleep(100);
	}
	return (1);
}

void	print_action(t_philo *ph, char *action)
{
	long int	current_time_ms;
	int			sim_is_over;

	pthread_mutex_lock(&ph->data->print_lock);
	sim_is_over = is_simulation_over(ph->data);
	if (!sim_is_over || strcmp(action, DIE) == 0)
	{
		current_time_ms = get_time() - ph->data->start_time;
		printf("%ld %d %s\n", current_time_ms, ph->id + 1, action);
	}
	pthread_mutex_unlock(&ph->data->print_lock);
}
