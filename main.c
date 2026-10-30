/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 21:28:11 by tsanjara          #+#    #+#             */
/*   Updated: 2026/10/26 21:28:23 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
    t_sim       sim;
    t_dongle    *dongles;
    t_coder     *coders;
    int         i = 1;

    if (!validate_paramaters(argc, argv, &sim))
        return (1);
    if (init_mutexes(&sim) != 0)
        return (1);
    if (clock_gettime(CLOCK_MONOTONIC,
            &sim.start_time.start_time) != 0)
    {
        destroy_mutexes(&sim);
        return (1);
    }
    dongles = allocate_dongles(sim.nb_coders);
    if (!dongles)
    {
        destroy_mutexes(&sim);
        return (1);
    }
    if (create_dongles(dongles, sim.nb_coders) != 0)
    {
        free(dongles);
        destroy_mutexes(&sim);
        return (1);
    }
    coders = allocate_coders(sim.nb_coders);
    if (!coders)
    {
        destroy_dongles(dongles, sim.nb_coders);
        free(dongles);
        destroy_mutexes(&sim);
        return (1);
    }
    create_coders(coders, sim.nb_coders, sim.start_time);
    free(coders);
    destroy_dongles(dongles, sim.nb_coders);
    free(dongles);
    destroy_mutexes(&sim);
    while (i <= sim.nb_coders)
    {
        printf("Coder %d: dongle %d and dongle %d\n",
            i,
            get_first_dongle(i) + 1,
            get_second_dongle(i, sim.nb_coders) + 1);
        i++;
    }
    if (create_queue(&sim) != 0)
    {
        destroy_mutexes(&sim);
        return 1;
    }
    return (0);
}
