#include "codexion.h"


void manage_edf(t_queue *queue)
{
    int         i;
    t_request   request;

    i = 0;
    while (i < queue->size - 1)
    {
        if (queue->requests[i].deadline > queue->requests[i + 1].deadline)
        {
            request = queue->requests[i];
            queue->requests[i] = queue->requests[i + 1];
            queue->requests[i + 1] = request;
            i = 0;
        }
        else
            i++;
    }
}

//int try_to_find_available_dongles(t_sim *sim)
//{
//    int i;
//    t_queue *queue;
//
//    i = 0;
//    queue = &sim->queue;
//    while(i < queue->size)
//    {
//        if (trying_to_take_dongles(sim, sim->queue.requests[i].coder) == 0)
//            return (i);
//        i++;
//    }
//    return(-1);
//}

void remove_queue_request(t_queue *queue, int index)
{
    int i;

    i = index;
    while(i < queue->size - 1)
    {
        queue->requests[i] = queue->requests[i + 1];
        i++;
    }
    queue->size--;
}

void    waiting_selection(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->scheduler_mutex);
    printf("Coder %d: waiting for selection\n", coder->coder_id);
    while(!coder->selected && !sim->stop)
    {
        pthread_cond_wait(&coder->scheduler_cond, &sim->scheduler_mutex);
    }
    printf("Coder %d: received selection\n", coder->coder_id);
    coder->selected = 0;
    pthread_mutex_unlock(&sim->scheduler_mutex);
}
