/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedule_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:57:25 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_can_proceed(t_sim *sim, t_coder *coder)
{
	int	left;
	int	right;
	int	available;

	if (sim->nb_coders == 1)
		return (0);
	left = get_first_dongle(coder->coder_id);
	right = get_second_dongle(coder->coder_id, sim->nb_coders);
	pthread_mutex_lock(&sim->dongles_mutex);
	available = (sim->dongles[left].state == AVAILABLE
			&& sim->dongles[right].state == AVAILABLE && remaining_cooldown(sim,
				&sim->dongles[left]) == 0 && remaining_cooldown(sim,
				&sim->dongles[right]) == 0);
	pthread_mutex_unlock(&sim->dongles_mutex);
	return (available);
}

int	trying_to_take_dongles(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->log_mutex);
	log_statement(sim, coder, "has taken a dongle");
	if (sim->nb_coders > 1)
		log_statement(sim, coder, "has taken a dongle");
	pthread_mutex_unlock(&sim->log_mutex);
	return (0);
}

void	realising_dongle(t_sim *sim, t_coder *coder)
{
	int	left;
	int	right;

	left = get_first_dongle(coder->coder_id);
	right = get_second_dongle(coder->coder_id, sim->nb_coders);
	pthread_mutex_lock(&sim->dongles_mutex);
	sim->dongles[left].state = AVAILABLE;
	sim->dongles[right].state = AVAILABLE;
	clock_gettime(CLOCK_MONOTONIC, &sim->dongles[left].last_release.start_time);
	clock_gettime(CLOCK_MONOTONIC,
		&sim->dongles[right].last_release.start_time);
	pthread_mutex_unlock(&sim->dongles_mutex);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
}

void	dispatch_batch(t_sim *sim, t_coder *first_coder)
{
	t_coder	*coder;
	int		i;
	int		selected;

	coder = first_coder;
	while (coder != NULL)
	{
		trying_to_take_dongles(sim, coder);
		coder = try_select_coder(sim);
	}
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->scheduler_mutex);
		selected = sim->coders[i].selected;
		pthread_mutex_unlock(&sim->scheduler_mutex);
		if (selected)
			coder_selection(sim, &sim->coders[i]);
		i++;
	}
}

void	*scheduler_routine(void *args)
{
	t_sim		*sim;
	t_request	request;

	sim = (t_sim *)args;
	while (!getting_stop(sim))
	{
		if (queue_pop(sim, &request) != 0)
			break ;
		dispatch_batch(sim, request.coder);
	}
	return (NULL);
}
