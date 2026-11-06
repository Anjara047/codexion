/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:25:25 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/05 18:25:27 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle *allocate_dongles(int nb_coders)
{
	t_dongle *dongles;

	dongles = malloc(sizeof(t_dongle) * nb_coders);
	if (!dongles)
		return NULL;
	return (dongles);
}


int create_dongles(t_dongle *dongles, int nb_coders)
{
    int i;

    i = 0;
    while (i < nb_coders)
    {
        dongles[i].dongle_id = i + 1;
        dongles[i].state = AVAILABLE;
        if (clock_gettime(CLOCK_MONOTONIC, &dongles[i].last_release.start_time) != 0)
            break;
        if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
            break;
        i++;
    }
    if (i != nb_coders)
    {
        while (i > 0)
        {
            i--;
            pthread_mutex_destroy(&dongles[i].mutex);
        }
        return (1);
    }
    return (0);
}

void destroy_dongles(t_dongle *dongles, int nb_coders)
{
    int i;

    i = 0;
    while (i < nb_coders)
    {
        pthread_mutex_destroy(&dongles[i].mutex);
        i++;
    }
}

int get_first_dongle(int coder_id)
{
    return (coder_id - 1);
}

int get_second_dongle(int coder_id, int nb_coders)
{
    return(coder_id % nb_coders);
}
