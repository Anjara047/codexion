/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/18 22:29:02 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	getting_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stop);
}

void	stop_setting(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->scheduler_mutex);
		pthread_cond_signal(&sim->coders[i].scheduler_cond);
		pthread_mutex_unlock(&sim->scheduler_mutex);
		i++;
	}
}

long long	timestamp_in_ms(t_time time)
{
	return ((long long)time.start_time.tv_sec * 1000 + time.start_time.tv_nsec
		/ 1000000);
}

int	check_coder_burnout(t_sim *sim, t_coder *coder, long long current_time)
{
	long long	last_cmple;

	pthread_mutex_lock(&sim->monitor_mutex);
	last_cmple = timestamp_in_ms(coder->last_cmpl);
	if (coder->state != BURNED_OUT && coder->compile_count < sim->tm_cmp_rqrd
		&& current_time - last_cmple >= sim->tm_b_out)
	{
		coder->state = BURNED_OUT;
		pthread_mutex_unlock(&sim->monitor_mutex);
		stop_setting(sim);
		log_statement(sim, coder, "burned out");
		return (1);
	}
	pthread_mutex_unlock(&sim->monitor_mutex);
	return (0);
}

void	*monitoring_routine(void *args)
{
	t_sim		*sim;
	t_time		current;
	long long	current_time;
	int			i;

	sim = (t_sim *)args;
	while (!getting_stop(sim))
	{
		clock_gettime(CLOCK_MONOTONIC, &current.start_time);
		current_time = timestamp_in_ms(current);
		i = 0;
		while (i < sim->nb_coders && !getting_stop(sim))
		{
			if (check_coder_burnout(sim, &sim->coders[i], current_time))
				return (NULL);
			i++;
		}
		sleeping(1);
	}
	return (NULL);
}
