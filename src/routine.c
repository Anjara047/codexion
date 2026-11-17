#include "codexion.h"

void    compiles(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->log_mutex);
    if (getting_stop(sim))
    {
        pthread_mutex_unlock(&sim->log_mutex);
        return;
    }
    pthread_mutex_lock(&sim->monitor_mutex);
    clock_gettime(CLOCK_MONOTONIC, &coder->last_cmpl.start_time);
    coder->state = COMPILING;
    coder->compile_count++;
    pthread_mutex_unlock(&sim->monitor_mutex);
    log_statement(sim, coder, "is compiling");
    pthread_mutex_unlock(&sim->log_mutex);
    sleeping(sim->tm_cmpl);
}

void    debug(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->log_mutex);
    if (getting_stop(sim))
    {
        pthread_mutex_unlock(&sim->log_mutex);
        return;
    }
    pthread_mutex_lock(&sim->monitor_mutex);
    coder->state = DEBUGGING;
    pthread_mutex_unlock(&sim->monitor_mutex);
    log_statement(sim, coder, "is debugging");
    pthread_mutex_unlock(&sim->log_mutex);
    sleeping(sim->tm_dbg);
}

void    refactor(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->log_mutex);
    if (getting_stop(sim))
    {
        pthread_mutex_unlock(&sim->log_mutex);
        return;
    }
    pthread_mutex_lock(&sim->monitor_mutex);
    coder->state = REFACTORING;
    pthread_mutex_unlock(&sim->monitor_mutex);
    log_statement(sim, coder, "is refactoring");
    pthread_mutex_unlock(&sim->log_mutex);
    sleeping(sim->tm_rfctr);
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
    //printf("Coder %d: routine started\n", coder->coder_id);
    nb_compiles = 0;
    while (sim->tm_cmp_rqrd > nb_compiles)
    {
        if (add_queue(sim, coder) != 0)
            return (NULL);
        waiting_selection(sim, coder);
        //printf("Coder %d: received selection\n", coder->coder_id);
        if (getting_stop(sim))
            break;
        if (trying_to_take_dongles(sim, coder) != 0)
            break;
        //printf("Coder %d: acquired both dongles\n", coder->coder_id);
        compiles(sim, coder);
        nb_compiles++;
        realising_dongle(sim, coder);
        //printf("Coder %d: released both dongles\n", coder->coder_id);
        debug(sim, coder);
        refactor(sim, coder);
    }
    return (NULL);
}