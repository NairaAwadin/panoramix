/*
** EPITECH PROJECT, 2024
** B-CCP-400-PAR-4-1-panoramix-naira.awadin
** File description:
** panormix
*/

#include <stdio.h>
#include <stdlib.h>
#include "panoramix.h"

void *druid_func(void *arg)
{
    druid_t *data = (druid_t *)arg;

    printf("Druid: I'm ready... but sleepy...\n");
    while (data->refill_count < data->max_refills) {
        sem_wait(&data->pot_empty);
        pthread_mutex_lock(&data->pot_mutex);
        if (data->refill_count < data->max_refills) {
            data->pot_size = data->pot_size;
            data->refill_count++;
            printf("Druid: Ahs! Yes, yes, I'm awake! Working on it! "
            "Beware I can only make %d more refills after this one.\n",
            data->max_refills - data->refill_count);
        }
        pthread_mutex_unlock(&data->pot_mutex);
        sem_post(&data->refill_done);
    }
    printf("Druid: I'm out of viscum. I'm going back to... zZz\n");
    sem_post(&data->refill_done);
    return NULL;
}

void handle_emptpot(villager_t *vdata)
{
    if (vdata->ddata->refill_count >= vdata->ddata->max_refills) {
        pthread_mutex_unlock(&vdata->ddata->pot_mutex);
        return;
    }
    printf("Villager %d: Hey Pano wake up! "
    "We need more potion.\n", vdata->id);
    sem_post(&vdata->ddata->pot_empty);
    pthread_mutex_unlock(&vdata->ddata->pot_mutex);
    sem_wait(&vdata->ddata->refill_done);
}

void drink_potion(villager_t *vdata)
{
    pthread_mutex_lock(&vdata->ddata->pot_mutex);
    if (vdata->ddata->pot_size == 0) {
        handle_emptpot(vdata);
        return;
    }
    printf("Villager %d: I need a drink... "
    "I see %d servings left.\n", vdata->id, vdata->ddata->pot_size - 1);
    vdata->ddata->pot_size--;
    pthread_mutex_unlock(&vdata->ddata->pot_mutex);
}

void fight(villager_t *vdata)
{
    printf("Villager %d: Take that roman scum! Only %d fights left.\n",
    vdata->id, --vdata->nb_fights);
}

void *villager_func(void *arg)
{
    villager_t *vdata = (villager_t *)arg;

    printf("Villager %d: Going into battle!\n", vdata->id);
    while (vdata->nb_fights > 0) {
        drink_potion(vdata);
        if (vdata->ddata->pot_size == 0
        && vdata->ddata->refill_count >= vdata->ddata->max_refills) {
            break;
        }
        fight(vdata);
    }
    printf("Villager %d: I'm going to sleep now.\n", vdata->id);
    return NULL;
}
