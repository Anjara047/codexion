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

int	create_service_threads(t_sim *sim, pthread_t *scheduler,
		pthread_t *monitor)
{
	if (pthread_create(scheduler, NULL, scheduler_routine, sim) != 0)
		return (1);
	if (pthread_create(monitor, NULL, monitoring_routine, sim) != 0)
	{
		stop_setting(sim);
		pthread_join(*scheduler, NULL);
		return (1);
	}
	return (0);
}

int	create_coder_threads(t_sim *sim, pthread_t *threads, t_args *args,
		int *created)
{
	int	i;

	i = 0;
	*created = 0;
	while (i < sim->nb_coders)
	{
		args[i].sim = sim;
		args[i].coder = &sim->coders[i];
		if (pthread_create(&threads[i], NULL, routine, &args[i]) != 0)
			return (1);
		(*created)++;
		i++;
	}
	return (0);
}

void	join_coder_threads(pthread_t *threads, int created)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void	join_service_threads(pthread_t scheduler, pthread_t monitor)
{
	pthread_join(scheduler, NULL);
	pthread_join(monitor, NULL);
}

int	run_simulation(t_sim *sim, pthread_t *threads, t_args *args)
{
	pthread_t	scheduler;
	pthread_t	monitor;
	int			created;

	if (create_service_threads(sim, &scheduler, &monitor) != 0)
		return (1);
	if (create_coder_threads(sim, threads, args, &created) != 0)
	{
		stop_setting(sim);
		join_coder_threads(threads, created);
		join_service_threads(scheduler, monitor);
		return (1);
	}
	join_coder_threads(threads, created);
	if (all_coders_finished(sim) && !getting_stop(sim))
		stop_setting(sim);
	join_service_threads(scheduler, monitor);
	return (0);
}
