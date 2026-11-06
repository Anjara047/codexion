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

void    *routine(void *args)
{
    t_sim *sim;
    int nb_compiles;
    
    sim = (t_sim *)args;
    nb_compiles = 0;
    while(sim->tm_cmp_rqrd > nb_compiles)
    {
        trying_to_take_dongles();
        compiles();
        nb_compiles++;
        debug();
        refactor();
    }
    return NULL;
}
