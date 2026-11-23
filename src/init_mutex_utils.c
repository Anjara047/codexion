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

void	destroy_first_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&sim->queue_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
}

void	destroy_second_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&sim->monitor_mutex);
	pthread_mutex_destroy(&sim->scheduler_mutex);
	pthread_mutex_destroy(&sim->dongles_mutex);
}

int	init_first_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		return (1);
	}
	if (pthread_mutex_init(&sim->queue_mutex, NULL) != 0)
	{
		destroy_first_mutexes_partial(sim);
		return (1);
	}
	return (0);
}

void	destroy_first_mutexes_partial(t_sim *sim)
{
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
}

int	init_second_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->dongles_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->scheduler_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->dongles_mutex);
		return (1);
	}
	if (pthread_mutex_init(&sim->monitor_mutex, NULL) != 0)
	{
		destroy_second_mutexes_partial(sim);
		return (1);
	}
	return (0);
}
