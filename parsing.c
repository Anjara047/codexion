/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanjara <tsanjara@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 21:28:31 by tsanjara          #+#    #+#             */
/*   Updated: 2026/10/26 21:28:50 by tsanjara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void parameters_valid(char *param, int *dest)
{
    int i;
    long long num_param;
    if (!dest)
        return;
    *dest = -1;
    i = 0;
    if (param[0] == '\0')
        return;
    num_param = 0;
    while (param[i])
    {
        if (param[i] < '0' || param[i] > '9')
            return;
        num_param = (num_param * 10) + (param[i] - '0');
        if (num_param > INT_MAX)
            return;
        i++;
    }
    *dest = (int)num_param;
}
int validate_paramaters(int argc, char **argv, t_sim *sim)
{
    if (argc != 9)
    {
        print("Usage: ./codexion number_of_coders time_to_burnout "
            "time_to_compile time_to_debug time_to_refactor "
            "number_of_compiles_required dongle_cooldown scheduler\n");
        exit(0);
    }
    parameters_valid(argv[1], &sim->nb_coders);
    if (sim->nb_coders < 0)
    {
        print("number_of_coders must be a valid integer\n");
        exit(1);
    }
    parameters_valid(argv[2], &sim->tm_b_out);
    if (sim->tm_b_out < 0)
    {
        print("time_to_burnout must be a valid integer\n");
        exit(0);
    }
    parameters_valid(argv[3], &sim->tm_cmpl);
    if (sim->tm_cmpl < 0)
    {
        print("time_to_compile must be a valid integer\n");
        exit(0);
    }
    parameters_valid(argv[4], &sim->tm_dbg);
    if (sim->tm_dbg < 0)
    {
        print("time_to_debug must be a valid integer\n");
        exit(0);
    }
    parameters_valid(argv[5], &sim->tm_rfctr);
    if (sim->tm_rfctr < 0)
    {
        print("time_to_refactor must be a valid integer\n");
        exit(0);
    }
    parameters_valid(argv[6], &sim->tm_cmp_rqrd);
    if (sim->tm_cmp_rqrd < 0)
    {
        print("time_to_compare must be a valid integer\n");
        exit(0);
    }
    parameters_valid(argv[7], &sim->cooldown);
    if (sim->cooldown < 0)
    {
        print("cooldown must be a valid integer\n");
        exit(0);
    }
    if (strcmp(argv[8], "fifo") == 0)
        sim->scheduler = fifo;
    else if (strcmp(argv[8], "edf") == 0)
        sim->scheduler = edf;
    else
    {
        print("scheduler must be fifo or edf\n");
        exit(0);
    }
    return (1);
}