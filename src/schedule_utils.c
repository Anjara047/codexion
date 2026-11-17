#include "codexion.h"

static long long remaining_cooldown(t_sim *sim, t_dongle *dongle)
{
    t_time      current;
    long long   elapsed_time;

    if (dongle->last_release.start_time.tv_sec == 0
        && dongle->last_release.start_time.tv_nsec == 0)
        return(0);
    clock_gettime(CLOCK_MONOTONIC, &current.start_time);
    elapsed_time = timesatamps_ms(current) - timesatamps_ms(dongle->last_release);
    if(elapsed_time >= sim->cooldown)
        return (0);
    return(sim->cooldown - elapsed_time);
}

int trying_to_take_dongles(t_sim *sim, t_coder *coder)
{
    int                 left;
    int                 right;
    long long           left_wait;
    long long           right_wait;
    long long           wait_time;
    struct timespec     timeout;

    left = get_first_dongle(coder->coder_id);
    right = get_second_dongle(coder->coder_id, sim->nb_coders);
    if (sim->nb_coders == 1)
    {
        pthread_mutex_lock(&sim->dongles_mutex);
        while (!getting_stop(sim))
            pthread_cond_wait(&sim->dongles_cond, &sim->dongles_mutex);
        pthread_mutex_unlock(&sim->dongles_mutex);
        return (1);
    }
    pthread_mutex_lock(&sim->dongles_mutex);
    while (!getting_stop(sim))
    {
        if (sim->dongles[left].state == UNAVAILABLE
            || sim->dongles[right].state == UNAVAILABLE)
            pthread_cond_wait(&sim->dongles_cond,
                &sim->dongles_mutex);
        else
        {
            left_wait = remaining_cooldown(sim, &sim->dongles[left]);
            right_wait = remaining_cooldown(sim, &sim->dongles[right]);
            if (left_wait > right_wait)
                wait_time = left_wait;
            else
                wait_time = right_wait;
            if (wait_time == 0)
                break;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += wait_time / 1000;
            timeout.tv_nsec += (wait_time % 1000) * 1000000;
            if (timeout.tv_nsec >= 1000000000)
            {
                timeout.tv_sec++;
                timeout.tv_nsec -= 1000000000;
            }
            pthread_cond_timedwait(&sim->dongles_cond,
                &sim->dongles_mutex, &timeout);
        }
    }
    if (getting_stop(sim))
    {
        pthread_mutex_unlock(&sim->dongles_mutex);
        return (1);
    }
    sim->dongles[left].state = UNAVAILABLE;
    sim->dongles[right].state = UNAVAILABLE;
    pthread_mutex_unlock(&sim->dongles_mutex);
    pthread_mutex_lock(&sim->log_mutex);
    log_statement(sim, coder, "has taken a dongle");
    log_statement(sim, coder, "has taken a dongle");
    pthread_mutex_unlock(&sim->log_mutex);
    pthread_mutex_lock(&sim->scheduler_mutex);
    sim->acquired = 1;
    pthread_cond_signal(&sim->scheduler_cond);
    pthread_mutex_unlock(&sim->scheduler_mutex);
    return (0);
}

void realising_dongle(t_sim *sim, t_coder *coder)
{
    int left;
    int right;

    left = get_first_dongle(coder->coder_id);
    right = get_second_dongle(coder->coder_id, sim->nb_coders);
    pthread_mutex_lock(&sim->dongles_mutex);
    sim->dongles[left].state = AVAILABLE;
    sim->dongles[right].state = AVAILABLE;
    clock_gettime(CLOCK_MONOTONIC,
        &sim->dongles[left].last_release.start_time);
    clock_gettime(CLOCK_MONOTONIC,
        &sim->dongles[right].last_release.start_time);
    pthread_cond_signal(&sim->dongles_cond);
    pthread_mutex_unlock(&sim->dongles_mutex);
}

void    *scheduler_routine(void *args)
{
    t_sim       *sim;
    t_request   request;

    sim = (t_sim *)args;
    //printf("Scheduler started\n");
    while (!getting_stop(sim))
    {
        if (queue_pop(sim, &request) != 0)
            break;
        //printf("Scheduler selected Coder %d\n", request.coder->coder_id);
        coder_selection(sim, request.coder);
    }
    return (NULL);
}