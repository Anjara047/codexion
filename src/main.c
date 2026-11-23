/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:51:56 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim		sim;
	pthread_t	*threads;
	t_args		*args;

	if (!validate_paramaters(argc, argv, &sim))
		return (1);
	if (init_mutexes(&sim) != 0)
		return (1);
	if (init_queue(&sim) != 0)
		return (1);
	if (clock_gettime(CLOCK_MONOTONIC, &sim.start_time.start_time) != 0)
		return (1);
	if (initialize_simulation(&sim) != 0)
		return (1);
	if (allocate_thread_data(&threads, &args, sim.nb_coders) != 0)
		return (1);
	if (run_simulation(&sim, threads, args) != 0)
		return (1);
	free(threads);
	free(args);
	cleanup_simulation(&sim);
	return (0);
}
