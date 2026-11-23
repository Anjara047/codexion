/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:24:56 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 09:56:05 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define FIFO 0
# define EDF 1

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_time
{
	struct timespec	start_time;
}					t_time;

typedef enum state_coder
{
	WAITING,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}					t_state_coder;

typedef struct s_coder
{
	int				coder_id;
	int				compile_count;
	t_time			last_cmpl;
	t_state_coder	state;
	int				selected;
	pthread_cond_t	scheduler_cond;
}					t_coder;

typedef struct s_request
{
	t_coder			*coder;
	long			arrival_coder;
	long long		deadline;
}					t_request;

typedef struct s_queue
{
	t_request		*requests;
	int				size;
	int				capacity;
}					t_queue;

typedef enum state_dongle
{
	AVAILABLE = 0,
	UNAVAILABLE
}					t_state_dongle;

typedef struct s_dongle
{
	int				dongle_id;
	t_state_dongle	state;
	pthread_mutex_t	mutex;
	t_time			last_release;
}					t_dongle;

typedef struct s_sim
{
	int				nb_coders;
	int				tm_b_out;
	int				tm_cmpl;
	int				tm_dbg;
	int				tm_rfctr;
	int				tm_cmp_rqrd;
	int				cooldown;
	int				scheduler;
	t_time			start_time;
	t_queue			*queue;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	dongles_mutex;
	pthread_cond_t	dongles_cond;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	queue_mutex;
	pthread_mutex_t	scheduler_mutex;
	pthread_mutex_t	monitor_mutex;
	pthread_cond_t	scheduler_cond;
	pthread_cond_t	queue_cond;
	int				selected_coder;
	int				acquired;
	int				stop;
	long			next_arrival;
}					t_sim;

typedef struct s_args
{
	t_sim			*sim;
	t_coder			*coder;
}					t_args;

void				parameters_valid(char *param, int *dest);
int					validate_paramaters(int argc, char **argv, t_sim *sim);
int					main(int argc, char **argv);
void				print(const char *mess);
void				sleeping(long time_in_ms);
t_coder				*allocate_coders(int nb_coders);
int					create_coders(t_coder *coders, int nb_coders,
						t_time start_time);
int					create_dongles(t_dongle *dongles, int nb_coders);
t_dongle			*allocate_dongles(int nb_coders);
void				*routine(void *args);
int					init_mutexes(t_sim *sim);
void				destroy_mutexes(t_sim *sim);
void				destroy_dongles(t_dongle *dongles, int nb_coders);
int					get_first_dongle(int coder_id);
int					get_second_dongle(int coder_id, int nb_coders);
int					init_queue(t_sim *sim);
int					add_queue(t_sim *sim, t_coder *coders);
int					queue_pop(t_sim *sim, t_request *request);
int					trying_to_take_dongles(t_sim *sim, t_coder *coder);
void				realising_dongle(t_sim *sim, t_coder *coder);
void				manage_edf(t_queue *queue);
int					try_to_find_available_dongles(t_sim *sim);
void				remove_queue_request(t_queue *queue, int index);
void				waiting_selection(t_sim *sim, t_coder *coder);
void				coder_selection(t_sim *sim, t_coder *coder);
void				*scheduler_routine(void *args);
void				compiles(t_sim *sim, t_coder *coder);
void				debug(t_sim *sim, t_coder *coder);
void				refactor(t_sim *sim, t_coder *coder);
long long			timestamp_in_ms(t_time time);
void				*monitoring_routine(void *args);
int					getting_stop(t_sim *sim);
void				stop_setting(t_sim *sim);
void				destroy_coders(t_coder *coders, int nb_coders);
void				log_statement(t_sim *sim, t_coder *coder, const char *mess);
int					request_can_proceed(t_sim *sim, t_coder *coder);
int					check_request_conflict(t_sim *sim, t_coder *coder_a,
						t_coder *coder_b);
int					is_first_in_both_queues(t_sim *sim, t_coder *coder);
int					reserve_dongles(t_sim *sim, t_coder *coder);
int					take_both_dongles(t_sim *sim, t_coder *coder);
void				sort_queues(t_sim *sim);
t_coder				*find_coder_available(t_sim *sim);
long long			remaining_cooldown(t_sim *sim, t_dongle *dongle);
long long			get_next_cooldown(t_sim *sim);
int					dongles_are_available(t_sim *sim, t_coder *coder);
void				wait_for_cooldown(t_sim *sim, long long wait_time);
t_request			create_request(t_sim *sim, t_coder *coder);
int					check_coder_burnout(t_sim *sim, t_coder *coder,
						long long current_time);
int					run_simulation(t_sim *sim, pthread_t *threads,
						t_args *args);
int					initialize_simulation(t_sim *sim);
int					allocate_thread_data(pthread_t **threads, t_args **args,
						int nb_coders);
void				cleanup_simulation(t_sim *sim);
int					all_coders_finished(t_sim *sim);
t_coder				*try_select_coder(t_sim *sim);
void				destroy_first_mutexes(t_sim *sim);
void				destroy_second_mutexes(t_sim *sim);
int					init_first_mutexes(t_sim *sim);
void				destroy_first_mutexes_partial(t_sim *sim);
int					init_second_mutexes(t_sim *sim);
void				destroy_second_mutexes_partial(t_sim *sim);
int					init_conditions(t_sim *sim);
void				destroy_conditions(t_sim *sim);
int					create_service_threads(t_sim *sim, pthread_t *scheduler,
						pthread_t *monitor);
int					create_coder_threads(t_sim *sim, pthread_t *threads,
						t_args *args, int *created);
void				join_coder_threads(pthread_t *threads, int created);
void				join_service_threads(pthread_t scheduler,
						pthread_t monitor);

#endif
