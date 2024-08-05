/*
** EPITECH PROJECT, 2024
** B-CCP-400-PAR-4-1-panoramix-naira.awadin
** File description:
** main
*/
#include <stdio.h>
#include <stdlib.h>
#include "panoramix.h"

void init_druid_data(druid_t *data, int pot, int refills)
{
    data->pot_size = pot;
    data->max_refills = refills;
    data->refill_count = 0;
    pthread_mutex_init(&data->pot_mutex, NULL);
    sem_init(&data->pot_empty, 0, 0);
    sem_init(&data->refill_done, 0, 0);
}

void create_threads(int nb_villagers, int nb_fights, druid_t *data)
{
    pthread_t druid_thread;
    pthread_t villagers[nb_villagers];
    villager_t villager_data[nb_villagers];

    pthread_create(&druid_thread, NULL, druid_func, data);
    for (int i = 0; i < nb_villagers; i++) {
        villager_data[i].id = i;
        villager_data[i].nb_fights = nb_fights;
        villager_data[i].ddata = data;
        pthread_create(&villagers[i], NULL, villager_func,
        &villager_data[i]);
    }
    for (int i = 0; i < nb_villagers; i++) {
        pthread_join(villagers[i], NULL);
    }
    sem_post(&data->pot_empty);
    pthread_join(druid_thread, NULL);
}

void cleanup_resources(druid_t *data)
{
    pthread_mutex_destroy(&data->pot_mutex);
    sem_destroy(&data->pot_empty);
    sem_destroy(&data->refill_done);
}

int error_handle(int ac, char **av, druid_t *data)
{
    if (ac != 5) {
        printf("USAGE:./panoramix <nb_villagers> <pot_size> <nb_fights> "
        "<nb_refills>\n");
        return 84;
    }
    data->nb_villagers = atoi(av[1]);
    data->pot_size = atoi(av[2]);
    data->nb_fights = atoi(av[3]);
    data->max_refills = atoi(av[4]);
    if (data->nb_villagers <= 0 || data->pot_size <= 0 ||
    data->nb_fights <= 0 || data->max_refills < 0) {
        return 84;
    }
    data->refill_count = 0;
    return 0;
}

int main(int ac, char **av)
{
    druid_t data;
    int result = error_handle(ac, av, &data);

    if (result != 0) {
        return result;
    }
    init_druid_data(&data, data.pot_size, data.max_refills);
    create_threads(data.nb_villagers, data.nb_fights, &data);
    cleanup_resources(&data);
    return 0;
}
