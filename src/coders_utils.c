/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 22:28:58 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/23 08:52:54 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	*allocate_coders(int nb_coders)
{
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * nb_coders);
	if (!coders)
		return (NULL);
	return (coders);
}

int	create_coders(t_coder *coders, int nb_coders, t_time start_time)
{
	int	i;

	i = 0;
	while (i < nb_coders)
	{
		coders[i].coder_id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_cmpl = start_time;
		coders[i].state = WAITING;
		coders[i].selected = 0;
		if (pthread_cond_init(&coders[i].scheduler_cond, NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_cond_destroy(&coders[i].scheduler_cond);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	coder_selection(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->scheduler_mutex);
	pthread_cond_signal(&coder->scheduler_cond);
	pthread_mutex_unlock(&sim->scheduler_mutex);
}

void	destroy_coders(t_coder *coders, int nb_coders)
{
	int	i;

	i = 0;
	while (i < nb_coders)
	{
		pthread_cond_destroy(&coders[i].scheduler_cond);
		i++;
	}
	free(coders);
}

t_coder	*find_coder_available(t_sim *sim)
{
	int	i;
	int	selected;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->scheduler_mutex);
		selected = sim->coders[i].selected;
		pthread_mutex_unlock(&sim->scheduler_mutex);
		if (!selected && is_first_in_both_queues(sim, &sim->coders[i])
			&& dongles_are_available(sim, &sim->coders[i]))
			return (&sim->coders[i]);
		i++;
	}
	return (NULL);
}
