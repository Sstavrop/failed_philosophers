/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magnesium <magnesium@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:49:17 by magnesium         #+#    #+#             */
/*   Updated: 2025/05/11 12:50:17 by magnesium        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_numeric_arg(const char *arg_str, long *out_val)
{
	int	j;

	j = 0;
	if (arg_str[j] == '+')
		j++;
	if (arg_str[j] == '\0')
		return (printf(ERR_ARG_EMPTY, arg_str), 0);
	while (arg_str[j])
	{
		if (arg_str[j] < '0' || arg_str[j] > '9')
		{
			printf(ERR_ARG_NON_NUMERIC, arg_str);
			return (0);
		}
		j++;
	}
	*out_val = ft_atoi(arg_str);
	if (*out_val > INT_MAX)
	{
		printf(ERR_ARG_TOO_LARGE, arg_str);
		return (0);
	}
	return (1);
}

int	validate_argument_value(long val, int arg_idx, int argc)
{
	if (val <= 0)
	{
		if (arg_idx == 1)
			return (printf(ERR_NB_PHILO), 0);
		if (arg_idx >= 2 && arg_idx <= 4)
			return (printf(ERR_TIME_ARGS), 0);
		if (arg_idx == 5 && argc == 6)
			return (printf(ERR_NB_MEALS), 0);
	}
	return (1);
}

int	args_check(int argc, char **argv)
{
	int		i;
	long	val;

	if (argc < 5 || argc > 6)
		return (printf(ERR_USAGE), 0);
	i = 1;
	while (i < argc)
	{
		if (!is_valid_numeric_arg(argv[i], &val))
			return (0);
		if (!validate_argument_value(val, i, argc))
			return (0);
		i++;
	}
	return (1);
}

int	check_all_deaths(t_data *dt)
{
	int	i;

	i = 0;
	while (i < dt->nbr_philo)
	{
		if (check_philo_death(&dt->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

int	wait_for_simulation_ready(t_data *dt)
{
	int	sim_over_on_start_check;
	int	continue_waiting;

	continue_waiting = 1;
	while (continue_waiting)
	{
		pthread_mutex_lock(&dt->sim_lock);
		sim_over_on_start_check = dt->over;
		if (dt->ready || sim_over_on_start_check)
			continue_waiting = 0;
		pthread_mutex_unlock(&dt->sim_lock);
		if (continue_waiting)
			usleep(100);
	}
	pthread_mutex_lock(&dt->sim_lock);
	sim_over_on_start_check = dt->over;
	pthread_mutex_unlock(&dt->sim_lock);
	if (sim_over_on_start_check && !dt->ready)
		return (1);
	return (0);
}
