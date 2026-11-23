/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/18 22:37:19 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_second_mutexes_partial(t_sim *sim)
{
	pthread_mutex_destroy(&sim->scheduler_mutex);
	pthread_mutex_destroy(&sim->dongles_mutex);
}

int	init_conditions(t_sim *sim)
{
	if (pthread_cond_init(&sim->queue_cond, NULL) != 0)
		return (1);
	if (pthread_cond_init(&sim->scheduler_cond, NULL) != 0)
	{
		pthread_cond_destroy(&sim->queue_cond);
		return (1);
	}
	if (pthread_cond_init(&sim->dongles_cond, NULL) != 0)
	{
		pthread_cond_destroy(&sim->scheduler_cond);
		pthread_cond_destroy(&sim->queue_cond);
		return (1);
	}
	return (0);
}

void	destroy_conditions(t_sim *sim)
{
	pthread_cond_destroy(&sim->dongles_cond);
	pthread_cond_destroy(&sim->scheduler_cond);
	pthread_cond_destroy(&sim->queue_cond);
}

int	init_mutexes(t_sim *sim)
{
	if (init_first_mutexes(sim) != 0)
		return (1);
	if (init_second_mutexes(sim) != 0)
		return (destroy_first_mutexes(sim), 1);
	if (init_conditions(sim) != 0)
	{
		destroy_second_mutexes(sim);
		destroy_first_mutexes(sim);
		return (1);
	}
	sim->selected_coder = 0;
	sim->acquired = 0;
	sim->stop = 0;
	return (0);
}

void	destroy_mutexes(t_sim *sim)
{
	destroy_conditions(sim);
	destroy_second_mutexes(sim);
	destroy_first_mutexes(sim);
}
