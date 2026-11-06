#include "codexion.h"

void manage_edf(t_queue *queue)
{
    int         i;
    long long   request;

    i = 0;
    while(i < queue->size - 1)
    {
        if (queue->requests[i].deadline > queue->requests[i + 1].deadline)
        {
            request = queue->requests[i].deadline;
            queue->requests[i].deadline = queue->requests[i + 1].deadline;
            queue->requests[i + 1].deadline = request;
            i = 0;
        }
        else
            i++;
    }
}

int try_to_find_available_dongles(t_sim *sim)
{
    int i;
    t_queue *queue;

    i = 0;
    queue = &sim->queue;
    while(i < queue->size)
    {
        if (trying_to_take_dongles(sim, sim->queue.requests[i].coder) == 0)
            return (i);
        i++;
    }
    return(-1);
}

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
