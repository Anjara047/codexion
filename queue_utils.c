#include "codexion.h"

int create_queue(t_sim *sim)
{
    sim->queue.size = 0;
    sim->queue.capacity = sim->nb_coders;
    sim->next_arrival = 0;
    sim->queue.requests = malloc(sizeof(t_request) * sim->queue.capacity);
    if(!sim->queue.requests)
        return 1;
    return 0;
}

int add_queue(t_sim *sim, t_coder *coders)
{
    t_request *requests;

    
}