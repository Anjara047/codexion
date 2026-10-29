/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 10:44:52 by tsanjara          #+#    #+#             */
/*   Updated: 2026/10/28 10:44:55 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void print(const char *mess)
{
    printf("%s", mess);
    return;
}

void sleeping(long time_in_ms)
{
    usleep((useconds_t)(time_in_ms) * 1000);
}
