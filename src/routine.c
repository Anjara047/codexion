/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 10:55:16 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiles(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (getting_stop(sim))
	{
		pthread_mutex_unlock(&sim->log_mutex);
		return ;
	}
	pthread_mutex_lock(&sim->monitor_mutex);
	clock_gettime(CLOCK_MONOTONIC, &coder->last_cmpl.start_time);
	coder->state = COMPILING;
	coder->compile_count++;
	pthread_mutex_unlock(&sim->monitor_mutex);
	log_statement(sim, coder, "is compiling");
	pthread_mutex_unlock(&sim->log_mutex);
	sleeping(sim->tm_cmpl);
}

void	debug(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (getting_stop(sim))
	{
		pthread_mutex_unlock(&sim->log_mutex);
		return ;
	}
	pthread_mutex_lock(&sim->monitor_mutex);
	coder->state = DEBUGGING;
	pthread_mutex_unlock(&sim->monitor_mutex);
	log_statement(sim, coder, "is debugging");
	pthread_mutex_unlock(&sim->log_mutex);
	sleeping(sim->tm_dbg);
}

void	refactor(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (getting_stop(sim))
	{
		pthread_mutex_unlock(&sim->log_mutex);
		return ;
	}
	pthread_mutex_lock(&sim->monitor_mutex);
	coder->state = REFACTORING;
	pthread_mutex_unlock(&sim->monitor_mutex);
	log_statement(sim, coder, "is refactoring");
	pthread_mutex_unlock(&sim->log_mutex);
	sleeping(sim->tm_rfctr);
}

int	coder_cycle(t_sim *sim, t_coder *coder, int *nb_compiles)
{
	if (add_queue(sim, coder) != 0)
		return (1);
	waiting_selection(sim, coder);
	if (getting_stop(sim))
		return (1);
	compiles(sim, coder);
	if (getting_stop(sim))
		return (1);
	(*nb_compiles)++;
	realising_dongle(sim, coder);
	if (sim->tm_cmp_rqrd <= *nb_compiles)
		return (1);
	debug(sim, coder);
	refactor(sim, coder);
	return (0);
}

void	*routine(void *args)
{
	t_sim	*sim;
	t_coder	*coder;
	t_args	*thread_args;
	int		nb_compiles;

	thread_args = (t_args *)args;
	sim = thread_args->sim;
	coder = thread_args->coder;
	nb_compiles = 0;
	while (sim->tm_cmp_rqrd > nb_compiles)
	{
		if (coder_cycle(sim, coder, &nb_compiles))
			break ;
	}
	return (NULL);
}
