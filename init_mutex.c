#include "codexion.h"

int init_mutexes(t_sim *sim)
{
    if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&sim->stop_mutex);
        pthread_mutex_destroy(&sim->log_mutex);
        return (1);
    }
    if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&sim->queue_mutex);
        pthread_mutex_destroy(&sim->stop_mutex);
        pthread_mutex_destroy(&sim -> log_mutex);
        return (1);
    }
    sim -> stop = 0;
    return (0);
}

void destroy_mutexes(t_sim *sim)
{
    pthread_cond_destroy(&sim->queue_cond);
    pthread_mutex_destroy(&sim->queue_mutex);
    pthread_mutex_destroy(&sim->log_mutex);
    pthread_mutex_destroy(&sim->stop_mutex);
}