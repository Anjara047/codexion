/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:26:29 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/05 18:26:34 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    compiles(t_sim *sim, t_coder *coder)
{
    (void)sim;
    (void)coder;
}

void    debug(t_sim *sim, t_coder *coder)
{
    (void)sim;
    (void)coder;
}

void    refactor(t_sim *sim, t_coder *coder)
{
    (void)sim;
    (void)coder;
}

void    *routine(void *args)
{
    t_sim   *sim;
    t_coder *coder;
    t_args  *thread_args;
    int     nb_compiles;

    thread_args = (t_args *)args;
    sim = thread_args->sim;
    coder = thread_args->coder;
    nb_compiles = 0;
    while (sim->tm_cmp_rqrd > nb_compiles)
    {
        if (add_queue(sim, coder) != 0)
            return (NULL);
        waiting_selection(sim, coder);
        if (sim->stop)
            break;
        if (trying_to_take_dongles(sim, coder) != 0)
            break;
        compiles(sim, coder);
        nb_compiles++;
        realising_dongle(sim, coder);
        debug(sim, coder);
        refactor(sim, coder);
    }
    return (NULL);
}