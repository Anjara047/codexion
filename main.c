/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:26:02 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/05 18:26:05 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "codexion.h"

int main(int argc, char **argv)
{
    t_sim       sim;
    t_dongle    *dongles;
    t_coder     *coders;
    int         i;

    if (!validate_paramaters(argc, argv, &sim))
        return (1);
    if (init_mutexes(&sim) != 0)
        return (1);
    if (init_queue(&sim) != 0)
    {
        destroy_mutexes(&sim);
        return (1);
    }
    if (clock_gettime(CLOCK_MONOTONIC,
            &sim.start_time.start_time) != 0)
    {
        free(sim.queue.requests);
        destroy_mutexes(&sim);
        return (1);
    }
    dongles = allocate_dongles(sim.nb_coders);
    if (!dongles)
    {
        free(sim.queue.requests);
        destroy_mutexes(&sim);
        return (1);
    }
    sim.dongles = dongles;
    if (create_dongles(dongles, sim.nb_coders) != 0)
    {
        free(dongles);
        free(sim.queue.requests);
        destroy_mutexes(&sim);
        return (1);
    }
    coders = allocate_coders(sim.nb_coders);
    if (!coders)
    {
        destroy_dongles(dongles, sim.nb_coders);
        free(dongles);
        free(sim.queue.requests);
        destroy_mutexes(&sim);
        return (1);
    }
    create_coders(coders, sim.nb_coders, sim.start_time);

    printf("\n--- test resa ---\n");
    printf("Coder 1: %s\n",
        trying_to_take_dongles(&sim, &coders[0]) == 0
            ? "SUCCESS" : "UNAVAILABLE");
    printf("Coder 2: %s\n",
        trying_to_take_dongles(&sim, &coders[1]) == 0
            ? "SUCCESS" : "UNAVAILABLE");

    printf("\n--- release before ---\n");
    i = 0;
    while (i < sim.nb_coders)
    {
        printf("Dongle %d: %s\n",
            dongles[i].dongle_id,
            dongles[i].state == AVAILABLE
                ? "AVAILABLE" : "UNAVAILABLE");
        i++;
    }

    printf("\nReleasing Coder 1 dongles...\n");
    realising_dongle(&sim, &coders[0]);

    printf("\n--- realese after ---\n");
    i = 0;
    while (i < sim.nb_coders)
    {
        printf("Dongle %d: %s\n",
            dongles[i].dongle_id,
            dongles[i].state == AVAILABLE
                ? "AVAILABLE" : "UNAVAILABLE");
        i++;
    }

    printf("\n--- CODER 2 RETRY ---\n");
    printf("Coder 2: %s\n",
        trying_to_take_dongles(&sim, &coders[1]) == 0
            ? "SUCCESS" : "UNAVAILABLE");

    free(coders);
    destroy_dongles(dongles, sim.nb_coders);
    free(dongles);
    free(sim.queue.requests);
    destroy_mutexes(&sim);
    return (0);
}


