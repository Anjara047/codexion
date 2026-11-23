/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_reservation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:55:38 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	reserve_dongles(t_sim *sim, t_coder *coder)
{
	int	left;
	int	right;
	int	available;

	if (sim->nb_coders == 1)
		return (1);
	left = get_first_dongle(coder->coder_id);
	right = get_second_dongle(coder->coder_id, sim->nb_coders);
	pthread_mutex_lock(&sim->dongles_mutex);
	available = (sim->dongles[left].state == AVAILABLE
			&& sim->dongles[right].state == AVAILABLE && remaining_cooldown(sim,
				&sim->dongles[left]) == 0 && remaining_cooldown(sim,
				&sim->dongles[right]) == 0);
	if (available)
	{
		sim->dongles[left].state = UNAVAILABLE;
		sim->dongles[right].state = UNAVAILABLE;
	}
	pthread_mutex_unlock(&sim->dongles_mutex);
	return (!available);
}

int	take_both_dongles(t_sim *sim, t_coder *coder)
{
	int	first;
	int	second;

	if (reserve_dongles(sim, coder) != 0)
		return (1);
	first = get_first_dongle(coder->coder_id);
	second = get_second_dongle(coder->coder_id, sim->nb_coders);
	remove_queue_request(&sim->queue[first], 0);
	if (second != first)
		remove_queue_request(&sim->queue[second], 0);
	pthread_mutex_lock(&sim->scheduler_mutex);
	coder->selected = 1;
	pthread_mutex_unlock(&sim->scheduler_mutex);
	return (0);
}

int	dongles_are_available(t_sim *sim, t_coder *coder)
{
	int	left;
	int	right;
	int	available;

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
