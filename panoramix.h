/*
** EPITECH PROJECT, 2024
** B-CCP-400-PAR-4-1-panoramix-naira.awadin
** File description:
** panoramix
*/

#ifndef PANORAMIX_H_
    #define PANORAMIX_H_
    #include <pthread.h>
    #include <semaphore.h>

typedef struct {
    int pot_size;
    int max_refills;
    int refill_count;
    int nb_villagers;
    int nb_fights;
    pthread_mutex_t pot_mutex;
    sem_t pot_empty;
    sem_t refill_done;
}druid_t;

typedef struct {
    int id;
    int nb_fights;
    druid_t *ddata;
}villager_t;

void *druid_func(void *arg);
void *villager_func(void *arg);

#endif /* !PANORAMIX_H_ */
