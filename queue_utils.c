/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:26:18 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/05 18:26:20 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_queue(t_sim *sim)
{
    sim->queue.size = 0;
    sim->queue.capacity = sim->nb_coders;
    sim->next_arrival = 0;
    sim->queue.requests = malloc(sizeof(t_request) * sim->queue.capacity);
    if(!sim->queue.requests)
        return 1;
    return 0;
}

int add_queue(t_sim *sim, t_coder *coder)
{
    t_request *request;

    pthread_mutex_lock(&sim->queue_mutex);
    if (sim->queue.size >= sim->queue.capacity)
    {
        pthread_mutex_unlock(&sim->queue_mutex);
        return (1);
    }
    request = &sim->queue.requests[sim->queue.size];
    request->coder = coder;
    sim->next_arrival++;
    request->arrival_coder = sim->next_arrival;
    if (sim->scheduler == edf)
    {
        request->deadline =
            (long long)coder->last_cmpl.start_time.tv_sec * 1000LL
            + coder->last_cmpl.start_time.tv_nsec / 1000000LL
            + sim->tm_b_out;
    }
    else
        request->deadline = 0;
    sim->queue.size++;
    pthread_cond_broadcast(&sim->queue_cond);
    pthread_mutex_unlock(&sim->queue_mutex);
    return (0);
}


int queue_pop(t_sim *sim, t_request *request)
{
    int i;
    int right;

    pthread_mutex_lock(&sim->queue_mutex);
    while (sim->queue.size == 0 && !sim->stop)
        pthread_cond_wait(&sim->queue_cond, &sim->queue_mutex);
    if (sim->stop)
    {
        pthread_mutex_unlock(&sim->queue_mutex);
        return (1);
    }
    right = 0;
    if (sim->scheduler == edf)
    {
        i = 1;
        while (i < sim->queue.size)
        {
            if (sim->queue.requests[i].deadline < sim->queue.requests[right].deadline)
                right = i;
            i++;
        }
    }
    *request = sim->queue.requests[right];
    i = right;
    while (i < sim->queue.size - 1)
    {
        sim->queue.requests[i] = sim->queue.requests[i + 1];
        i++;
    }
    sim->queue.size--;
    pthread_mutex_unlock(&sim->queue_mutex);
    return (0);
}
