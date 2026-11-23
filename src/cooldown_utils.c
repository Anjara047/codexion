/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cooldown_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 08:54:43 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:55:07 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	remaining_cooldown(t_sim *sim, t_dongle *dongle)
{
	t_time		current;
	long long	elapsed_time;

	if (dongle->last_release.start_time.tv_sec == 0
		&& dongle->last_release.start_time.tv_nsec == 0)
		return (0);
	clock_gettime(CLOCK_MONOTONIC, &current.start_time);
	elapsed_time = timestamp_in_ms(current)
		- timestamp_in_ms(dongle->last_release);
	if (elapsed_time >= sim->cooldown)
		return (0);
	return (sim->cooldown - elapsed_time);
}

long long	get_next_cooldown(t_sim *sim)
{
	int			i;
	long long	wait;
	long long	next_wait;

	i = 0;
	next_wait = 0;
	pthread_mutex_lock(&sim->dongles_mutex);
	while (i < sim->nb_coders)
	{
		wait = remaining_cooldown(sim, &sim->dongles[i]);
		if (wait > 0 && (next_wait == 0 || wait < next_wait))
			next_wait = wait;
		i++;
	}
	pthread_mutex_unlock(&sim->dongles_mutex);
	return (next_wait);
}

void	wait_for_cooldown(t_sim *sim, long long wait_time)
{
	t_time	timeout;

	clock_gettime(CLOCK_REALTIME, &timeout.start_time);
	timeout.start_time.tv_sec += wait_time / 1000;
	timeout.start_time.tv_nsec += (wait_time % 1000) * 1000000;
	if (timeout.start_time.tv_nsec >= 1000000000)
	{
		timeout.start_time.tv_sec++;
		timeout.start_time.tv_nsec -= 1000000000;
	}
	pthread_cond_timedwait(&sim->queue_cond, &sim->queue_mutex,
		&timeout.start_time);
}
