/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstavrop <sstavrop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:45:52 by sstavrop          #+#    #+#             */
/*   Updated: 2025/05/30 16:46:57 by sstavrop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* -------------------------------------------------------------------------- */
/* philo.h */
/* -------------------------------------------------------------------------- */
#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

/*---------------------- Messages ---------------------- */

# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define FORK "has taken a fork"
# define DIE "died"
# define ERR_ARG_EMPTY "\033[0;31mErr: Arg '%s' empty.\033[0m\n"
# define ERR_ARG_NON_NUMERIC "\033[0;31mErr: Arg '%s' non-numeric.\033[0m\n"
# define ERR_ARG_TOO_LARGE "\033[0;31mErr: Arg '%s' > INT_MAX.\033[0m\n"
# define ERR_USAGE "\033[0;31mErr:./philo<nb><die><eat><sleep>[meals]\033[0m\n"
# define ERR_NB_PHILO "\033[0;31mErr: Num philos <= 0.\033[0m\n"
# define ERR_TIME_ARGS "\033[0;31mErr: Time args <= 0.\033[0m\n"
# define ERR_NB_MEALS "\033[0;31mErr: Num meals <= 0.\033[0m\n"
/* -------------------- Structures -------------------- */

// Forward declaration

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int					nbr_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					loop;
	int					check_meal;

	long int			start_time;
	int					ready;
	int					over;

	pthread_mutex_t		*forks;
	pthread_mutex_t		sim_lock;
	pthread_mutex_t		print_lock;
	t_philo				*philos;
}						t_data;

struct					s_philo
{
	int					id;
	int					meals_eaten;
	long int			last_meal_time;

	pthread_t			life_tid;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		meal_lock;

	t_data				*data;
};

/* -------------------- functions -------------------- */

/* checks.c && checks2.c */
int						is_valid_numeric_arg(const char *arg_str,
							long *out_val);
int						validate_argument_value(long val, int arg_idx,
							int argc);
int						args_check(int argc, char **argv);
int						check_all_deaths(t_data *dt);
int						wait_for_simulation_ready(t_data *dt);
void					monitor_philos(t_data *dt);
int						check_philo_death(t_philo *ph);
int						all_philos_completed_meals(t_data *dt);
int						check_all_philos_eaten(t_data *dt);
int						is_simulation_over(t_data *data);

/* init_end.c && init_end2.c && init_end3.c */
int						init_data(int argc, char **argv, t_data *data);
void					destroy_n_fork_mutexes(t_data *data, int count);
int						init_fork_mutex_array(t_data *data);
int						init_mutexes(t_data *data);
void					destroy_n_philo_meal_locks(t_philo *philos, int count);
int						init_philos(t_data *data);
void					join_n_created_philo_threads(t_philo *philos,
							int count);
int						start_threads(t_data *data);
void					actual_join_all_philosopher_threads(t_data *data);
void					actual_destroy_all_fork_mutexes(t_data *data);
void					actual_destroy_all_philo_meal_locks(t_data *data);
void					join_threads_and_cleanup(t_data *data);

/* utils.c */
int						ft_atoi(const char *str);
void					*ft_calloc(size_t count, size_t size);
long int				get_time(void);
int						ft_usleep(long int milliseconds, t_data *data);
void					error_msg(char *str, t_data *dt);
void					print_action(t_philo *ph, char *action);

/* routine.c */
void					*philosopher_routine(void *arg);
void					routine_eat(t_philo *ph);
void					routine_sleep(t_philo *ph);
void					routine_think(t_philo *ph);
void					execute_philosopher_routine(t_philo *ph);
void					take_forks(t_philo *ph);

#endif
