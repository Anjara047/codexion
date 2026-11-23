/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 08:53:30 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:54:25 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	run_simulation(t_sim *sim, pthread_t *threads, t_args *args)
{
	pthread_t	scheduler_thread;
	pthread_t	monitor_thread;
	int			i;

	if (pthread_create(&scheduler_thread, NULL, scheduler_routine, sim) != 0)
		return (1);
	if (pthread_create(&monitor_thread, NULL, monitoring_routine, sim) != 0)
		return (1);
	i = 0;
	while (i < sim->nb_coders)
	{
		args[i].sim = sim;
		args[i].coder = &sim->coders[i];
		if (pthread_create(&threads[i], NULL, routine, &args[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	if (all_coders_finished(sim) && !getting_stop(sim))
		stop_setting(sim);
	pthread_join(scheduler_thread, NULL);
	pthread_join(monitor_thread, NULL);
	return (0);
}

int	initialize_simulation(t_sim *sim)
{
	sim->dongles = allocate_dongles(sim->nb_coders);
	if (!sim->dongles)
		return (1);
	if (create_dongles(sim->dongles, sim->nb_coders) != 0)
		return (1);
	sim->coders = allocate_coders(sim->nb_coders);
	if (!sim->coders)
		return (1);
	if (create_coders(sim->coders, sim->nb_coders, sim->start_time) != 0)
		return (1);
	return (0);
}

int	allocate_thread_data(pthread_t **threads, t_args **args, int nb_coders)
{
	*threads = malloc(sizeof(pthread_t) * nb_coders);
	if (!*threads)
		return (1);
	*args = malloc(sizeof(t_args) * nb_coders);
	if (!*args)
	{
		free(*threads);
		*threads = NULL;
		return (1);
	}
	return (0);
}

void	cleanup_simulation(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_cond_destroy(&sim->coders[i].scheduler_cond);
		i++;
	}
	free(sim->coders);
	destroy_dongles(sim->dongles, sim->nb_coders);
	free(sim->dongles);
	free(sim->queue->requests);
	free(sim->queue);
	destroy_mutexes(sim);
}

int	all_coders_finished(t_sim *sim)
{
	int	i;
	int	finished;

	i = 0;
	finished = 1;
	pthread_mutex_lock(&sim->monitor_mutex);
	while (i < sim->nb_coders)
	{
		if (sim->coders[i].compile_count < sim->tm_cmp_rqrd)
		{
			finished = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->monitor_mutex);
	return (finished);
}
