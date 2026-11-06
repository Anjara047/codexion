/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedule_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 21:07:05 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/05 21:07:14 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int trying_to_take_dongles(t_sim *sim, t_coder *coder)
{
    int left;
    int right;

    left = get_first_dongle(coder->coder_id);
    right = get_second_dongle(coder->coder_id, sim->nb_coders);
    pthread_mutex_lock(&sim->dongles_mutex);
    if (sim->nb_coders == 1)
    {
        sim->dongles[left].state = UNAVAILABLE;
        return(1);
    }
    if (sim->dongles[left].state == UNAVAILABLE || sim->dongles[right].state == UNAVAILABLE)
    {
        pthread_mutex_unlock(&sim->dongles_mutex);
        return (1);
    }
    sim->dongles[left].state = UNAVAILABLE;
    sim->dongles[right].state = UNAVAILABLE;
    pthread_mutex_unlock(&sim->dongles_mutex);
    return (0);
}

//int compiling(t_sim  *sim, t_coder *coder)
//{
//    (void)sim;
//    (void)coder;
//    return (0);
//
//}

void realising_dongle(t_sim *sim, t_coder *coder)
{
    int left;
    int right;

    left = get_first_dongle(coder->coder_id);
    right = get_second_dongle(coder->coder_id, sim->nb_coders);
    pthread_mutex_lock(&sim->dongles_mutex);
    if (sim->nb_coders == 1)
    {
        sim->dongles[left].state = UNAVAILABLE;
    }
    sim->dongles[left].state = AVAILABLE;
    sim->dongles[right].state = AVAILABLE;
    if(clock_gettime(CLOCK_MONOTONIC, &sim->dongles[left].last_release.start_time) != 0)
        print("failure in clock_gettime");
    if(clock_gettime(CLOCK_MONOTONIC, &sim->dongles[right].last_release.start_time) != 0)
        print("failure in clock_gettime");
    pthread_mutex_unlock(&sim->dongles_mutex);

}
