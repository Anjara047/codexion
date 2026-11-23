/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 10:44:52 by tsanjara          #+#    #+#             */
/*   Updated: 2026/11/18 22:36:08 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print(const char *mess)
{
	printf("%s", mess);
	return ;
}

void	sleeping(long time_in_ms)
{
	usleep((useconds_t)(time_in_ms) * 1000);
}

void	log_statement(t_sim *sim, t_coder *coder, const char *mess)
{
	t_time		current;
	long long	timestamp;

	clock_gettime(CLOCK_MONOTONIC, &current.start_time);
	timestamp = timestamp_in_ms(current) - timestamp_in_ms(sim->start_time);
	printf("%lld %d %s\n", timestamp, coder->coder_id, mess);
}
