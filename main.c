
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
    t_request   request;
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
    printf("\n--- ADD REQUESTS TO QUEUE ---\n");
    i = 0;
    while (i < sim.nb_coders)
    {
        if (add_queue(&sim, &coders[i]) != 0)
            printf("Failed to add Coder %d\n", coders[i].coder_id);
        else
            printf("Added Coder %d\n", coders[i].coder_id);
        i++;
    }

    printf("\nQueue size: %d\n", sim.queue.size);

    if (sim.nb_coders == 1)
    {
        printf("\n--- ONE CODER TEST ---\n");
        printf("Coder 1: %s\n",
            trying_to_take_dongles(&sim, &coders[0]) == 0
                ? "SUCCESS" : "UNAVAILABLE");
        printf("No scheduler selection: only one dongle is available.\n");
    }
    else if (sim.nb_coders == 2)
    {
        printf("\n--- TWO CODERS TEST ---\n");
        printf("Coder 1: %s\n",
            trying_to_take_dongles(&sim, &coders[0]) == 0
                ? "SUCCESS" : "UNAVAILABLE");

        printf("Releasing Coder 1 dongles...\n");
        realising_dongle(&sim, &coders[0]);

        printf("\n--- SCHEDULER SELECTION ---\n");
        if (queue_pop(&sim, &request) == 0)
        {
            printf("Selected Coder: %d\n", request.coder->coder_id);
            printf("Remaining requests: %d\n", sim.queue.size);
        }
        else
            printf("No request selected\n");
    }
    else
    {
        printf("\n--- RESERVE CODER 1 ---\n");
        trying_to_take_dongles(&sim, &coders[0]);

        printf("\n--- FIRST SCHEDULER SELECTION ---\n");
        if (queue_pop(&sim, &request) == 0)
        {
            printf("Selected Coder: %d\n", request.coder->coder_id);
            printf("Remaining requests: %d\n", sim.queue.size);
        }
        else
            printf("No request selected\n");

        printf("\n--- RELEASE CODER 1 ---\n");
        realising_dongle(&sim, &coders[0]);

        printf("\n--- SECOND SCHEDULER SELECTION ---\n");
        if (queue_pop(&sim, &request) == 0)
        {
            printf("Selected Coder: %d\n", request.coder->coder_id);
            printf("Remaining requests: %d\n", sim.queue.size);
        }
        else
            printf("No request selected\n");
    }

    free(coders);
    destroy_dongles(dongles, sim.nb_coders);
    free(dongles);
    free(sim.queue.requests);
    destroy_mutexes(&sim);
    return (0);
}
