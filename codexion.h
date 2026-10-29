#ifndef CODEXION_H
# define CODEXION_H

# define fifo 0
# define edf 1

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <time.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_time
{
    struct timespec start_time;
}t_time;

typedef enum state_coder
{
    WAITING,
    COMPILING,
    DEBUGGING,
    REFACTORING,
    BURNED_OUT
}   t_state_coder;

typedef struct s_coder
{
    int     coder_id;
    int     compile_count;
    t_time  last_cmpl;
    t_state_coder state;
}           t_coder;

typedef struct s_request
{
    t_coder     *coder;
    long        arrival_coder;
    long long   deadline;
}               t_request;

typedef struct s_queue
{
    t_request   *requests;
    int         size;
    int         capacity;
}   t_queue;

typedef struct s_sim
{
    int             nb_coders;
    int             tm_b_out;
    int             tm_cmpl;
    int             tm_dbg;
    int             tm_rfctr;
    int             tm_cmp_rqrd;
    int             cooldown;
    int             scheduler;
    t_time          start_time;
    t_queue         queue;
    pthread_mutex_t log_mutex;
    pthread_mutex_t stop_mutex;
    pthread_mutex_t queue_mutex;
    pthread_cond_t  queue_cond;
    int             stop;
}   t_sim;

typedef enum state_dongle
{
    AVAILABLE = 0,
    UNAVAILABLE
} t_state_dongle;

typedef struct s_dongle
{
    int             dongle_id;
    t_state_dongle  state;
    pthread_mutex_t mutex;
    t_time          last_release;
}   t_dongle;

void        parameters_valid(char *param, int *dest);
int         validate_paramaters(int argc, char **argv, t_sim *sim);
int         main(int argc, char **argv);
void        print(const char *mess);
void        sleeping(long time_in_ms);
t_coder     *allocate_coders(int nb_coders);
void        create_coders(t_coder *coders, int nb_coders, t_time start_time);
int         create_dongles(t_dongle *dongles, int nb_coders);
t_dongle    *allocate_dongles(int nb_coders);
void        *routine(void *args);
int         init_mutexes(t_sim *sim);
void        destroy_mutexes(t_sim *sim);
void        destroy_dongles(t_dongle *dongles, int nb_coders);
int         get_first_dongle(int coder_id);
int         get_second_dongle(int coder_id, int nb_coders);

#endif
