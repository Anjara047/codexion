/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_org.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:58:45 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	manage_edf(t_queue *queue)
{
	int			i;
	t_request	request;

	i = 0;
	while (i < queue->size - 1)
	{
		if (queue->requests[i].deadline > queue->requests[i + 1].deadline
			|| (queue->requests[i].deadline == queue->requests[i + 1].deadline
				&& queue->requests[i].arrival_coder
				> queue->requests[i + 1].arrival_coder))
		{
			request = queue->requests[i];
			queue->requests[i] = queue->requests[i + 1];
			queue->requests[i + 1] = request;
			i = 0;
		}
		else
			i++;
	}
}

void	sort_queues(t_sim *sim)
{
	int	i;

	if (sim->scheduler != EDF)
		return ;
	i = 0;
	while (i < sim->nb_coders)
	{
		manage_edf(&sim->queue[i]);
		i++;
	}
}

void	remove_queue_request(t_queue *queue, int index)
{
	int	i;

	i = index;
	while (i < queue->size - 1)
	{
		queue->requests[i] = queue->requests[i + 1];
		i++;
	}
	queue->size--;
}

void	waiting_selection(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->scheduler_mutex);
	while (!coder->selected && !getting_stop(sim))
		pthread_cond_wait(&coder->scheduler_cond, &sim->scheduler_mutex);
	coder->selected = 0;
	pthread_mutex_unlock(&sim->scheduler_mutex);
}

int	is_first_in_both_queues(t_sim *sim, t_coder *coder)
{
	int	first;
	int	second;

	first = get_first_dongle(coder->coder_id);
	second = get_second_dongle(coder->coder_id, sim->nb_coders);
	if (sim->queue[first].size == 0 || sim->queue[second].size == 0)
		return (0);
	if (sim->queue[first].requests[0].coder != coder)
		return (0);
	if (sim->queue[second].requests[0].coder != coder)
		return (0);
	return (1);
}
