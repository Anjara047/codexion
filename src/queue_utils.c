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

int	init_queue(t_sim *sim)
{
	int	i;

	sim->queue = malloc(sizeof(t_queue) * sim->nb_coders);
	if (!sim->queue)
		return (1);
	sim->next_arrival = 0;
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->queue[i].size = 0;
		sim->queue[i].capacity = 2;
		sim->queue[i].requests = malloc(sizeof(t_request) * 2);
		if (!sim->queue[i].requests)
		{
			while (i > 0)
				free(sim->queue[--i].requests);
			free(sim->queue);
			sim->queue = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

t_request	create_request(t_sim *sim, t_coder *coder)
{
	t_request	request;
	t_time		last_cmpl;

	pthread_mutex_lock(&sim->monitor_mutex);
	last_cmpl = coder->last_cmpl;
	pthread_mutex_unlock(&sim->monitor_mutex);
	request.coder = coder;
	sim->next_arrival++;
	request.arrival_coder = sim->next_arrival;
	if (sim->scheduler == EDF)
	{
		request.deadline = (long long)last_cmpl.start_time.tv_sec * 1000LL
			+ last_cmpl.start_time.tv_nsec / 1000000LL + sim->tm_b_out;
	}
	else
		request.deadline = 0;
	return (request);
}

int	add_queue(t_sim *sim, t_coder *coder)
{
	t_request	request;
	int			first;
	int			second;

	first = get_first_dongle(coder->coder_id);
	second = get_second_dongle(coder->coder_id, sim->nb_coders);
	pthread_mutex_lock(&sim->queue_mutex);
	if (sim->queue[first].size >= sim->queue[first].capacity
		|| sim->queue[second].size >= sim->queue[second].capacity)
	{
		pthread_mutex_unlock(&sim->queue_mutex);
		return (1);
	}
	request = create_request(sim, coder);
	sim->queue[first].requests[sim->queue[first].size] = request;
	sim->queue[first].size++;
	if (second != first)
	{
		sim->queue[second].requests[sim->queue[second].size] = request;
		sim->queue[second].size++;
	}
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
	return (0);
}

t_coder	*try_select_coder(t_sim *sim)
{
	t_coder	*coder;

	pthread_mutex_lock(&sim->queue_mutex);
	if (getting_stop(sim))
	{
		pthread_mutex_unlock(&sim->queue_mutex);
		return (NULL);
	}
	sort_queues(sim);
	coder = find_coder_available(sim);
	if (coder && take_both_dongles(sim, coder) != 0)
		coder = NULL;
	pthread_mutex_unlock(&sim->queue_mutex);
	return (coder);
}

int	queue_pop(t_sim *sim, t_request *request)
{
	t_coder		*coder;
	long long	wait_time;

	pthread_mutex_lock(&sim->queue_mutex);
	while (!getting_stop(sim))
	{
		sort_queues(sim);
		coder = find_coder_available(sim);
		if (coder && take_both_dongles(sim, coder) == 0)
		{
			request->coder = coder;
			pthread_mutex_unlock(&sim->queue_mutex);
			return (0);
		}
		wait_time = get_next_cooldown(sim);
		if (wait_time > 0)
			wait_for_cooldown(sim, wait_time);
		else
			pthread_cond_wait(&sim->queue_cond, &sim->queue_mutex);
	}
	pthread_mutex_unlock(&sim->queue_mutex);
	return (1);
}
