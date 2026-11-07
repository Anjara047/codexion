
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
    t_args      *args;
    pthread_t   *threads;
    pthread_t   scheduler_thread;
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
    if (create_coders(coders, sim.nb_coders, sim.start_time) != 0)
    {
        free(coders);
        destroy_dongles(dongles, sim.nb_coders);
        free(dongles);
        free(sim.queue.requests);
        destroy_mutexes(&sim);
        return (1);
    }
    threads = malloc(sizeof(pthread_t) * sim.nb_coders);
    args = malloc(sizeof(t_args) * sim.nb_coders);
    if (!threads || !args)
    {
        free(threads);
        free(args);
        i = 0;
        while (i < sim.nb_coders)
            pthread_cond_destroy(&coders[i++].scheduler_cond);
        free(coders);
        destroy_dongles(dongles, sim.nb_coders);
        free(dongles);
        free(sim.queue.requests);
        destroy_mutexes(&sim);
        return (1);
    }
    if (pthread_create(&scheduler_thread, NULL,
            scheduler_routine, &sim) != 0)
        return (1);
    i = 0;
    while (i < sim.nb_coders)
    {
        args[i].sim = &sim;
        args[i].coder = &coders[i];
        if (pthread_create(&threads[i], NULL, routine, &args[i]) != 0)
            return (1);
        i++;
    }
    i = 0;
    while (i < sim.nb_coders)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    pthread_mutex_lock(&sim.queue_mutex);
    sim.stop = 1;
    pthread_cond_signal(&sim.queue_cond);
    pthread_mutex_unlock(&sim.queue_mutex);
    pthread_join(scheduler_thread, NULL);
    i = 0;
    while (i < sim.nb_coders)
        pthread_cond_destroy(&coders[i++].scheduler_cond);
    free(threads);
    free(args);
    free(coders);
    destroy_dongles(dongles, sim.nb_coders);
    free(dongles);
    free(sim.queue.requests);
    destroy_mutexes(&sim);
    return (0);
}
