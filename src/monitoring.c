#include "codexion.h"

int getting_stop(t_sim *sim)
{
    int stop;

    pthread_mutex_lock(&sim->stop_mutex);
    stop = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);
    return(stop);
}

void    stop_setting(t_sim *sim)
{
    pthread_mutex_lock(&sim->stop_mutex);
    sim->stop = 1;
    pthread_mutex_unlock(&sim->stop_mutex);
}

long long timesatamps_ms(t_time time)
{
    return((long long)time.start_time.tv_sec * 1000
        + time.start_time.tv_nsec / 1000000);
}

void log_statement(t_sim *sim, t_coder *coder, const char *mess)
{
    t_time current;
    long long timesatamps;

    clock_gettime(CLOCK_MONOTONIC, &current.start_time);
    timesatamps = timesatamps_ms(current) - timesatamps_ms(sim->start_time);
    printf("%lld %d %s\n", timesatamps, coder->coder_id, mess);
}

void    *monitoring_routine(void *args)
{
    t_sim       *sim;
    t_time      current;
    long long   current_time;
    long long   last_cmple;
    t_coder     *burned_coder;
    int         i;

    sim = (t_sim *)args;
    while (!getting_stop(sim))
    {
        clock_gettime(CLOCK_MONOTONIC, &current.start_time);
        current_time = timesatamps_ms(current);
        i = 0;
        while (i < sim->nb_coders && !getting_stop(sim))
        {
            pthread_mutex_lock(&sim->monitor_mutex);
            last_cmple = timesatamps_ms(sim->coders[i].last_cmpl);
            if (sim->coders[i].state != BURNED_OUT
                && sim->coders[i].compile_count < sim->tm_cmp_rqrd
                && current_time - last_cmple >= sim->tm_b_out)
            {
                sim->coders[i].state = BURNED_OUT;
                burned_coder = &sim->coders[i];
                pthread_mutex_unlock(&sim->monitor_mutex);
                stop_setting(sim);
                pthread_mutex_lock(&sim->queue_mutex);
                pthread_cond_broadcast(&sim->queue_cond);
                pthread_mutex_unlock(&sim->queue_mutex);
                            
                pthread_mutex_lock(&sim->scheduler_mutex);
                pthread_cond_broadcast(&sim->scheduler_cond);
                i = 0;
                while (i < sim->nb_coders)
                {
                    pthread_cond_broadcast(&sim->coders[i].scheduler_cond);
                    i++;
                }
                pthread_mutex_unlock(&sim->scheduler_mutex);            
                pthread_mutex_lock(&sim->dongles_mutex);
                pthread_cond_broadcast(&sim->dongles_cond);
                pthread_mutex_unlock(&sim->dongles_mutex);
                //printf("%lld %d burned out\n",
                //    current_time - timesatamps_ms(sim->start_time), coder_id);
                log_statement(sim, burned_coder, "burned out");
                return (NULL);
            }
            pthread_mutex_unlock(&sim->monitor_mutex);
            i++;
        }
        sleeping(1);
    }
    return (NULL);
}
