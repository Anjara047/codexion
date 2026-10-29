#include "codexion.h"

void *routine(void *args)
{
    t_sim *sim;
    int nb_compiles;
    
    sim = (t_sim *)args;
    nb_compiles = 0;
    while(sim->tm_cmp_rqrd > nb_compiles)
    {
        take_dongles();
        compiles();
        nb_compiles++;
        debug();
        refactor();
    }
    return NULL;
}