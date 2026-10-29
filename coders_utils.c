/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_coders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 10:00:08 by tsanjara          #+#    #+#             */
/*   Updated: 2026/10/28 10:56:37 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder *allocate_coders(int nb_coders)
{
	t_coder *coders;

	coders = malloc(sizeof(t_coder) * nb_coders);
	if (!coders)
		return NULL;
	return (coders);
}

void create_coders(t_coder *coders, int nb_coders, t_time start_time)
{
	int i;

	i = 0;
	while(i < nb_coders)
	{
		coders[i].coder_id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_cmpl = start_time;
		coders[i].state = WAITING;
		i++;
	}
}