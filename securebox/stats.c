#include <stdio.h>
#include <time.h>
#include <pthread.h>

static int create_count = 0;
static int delete_count = 0;
static int modify_count = 0;
static int alert_count = 0;

static time_t start_time;

pthread_mutex_t stats_mutex =
    PTHREAD_MUTEX_INITIALIZER;

void init_stats()
{
    pthread_mutex_lock(&stats_mutex);

    create_count = 0;
    delete_count = 0;
    modify_count = 0;
    alert_count = 0;

    start_time = time(NULL);

    pthread_mutex_unlock(&stats_mutex);
}

void increment_create()
{
    pthread_mutex_lock(&stats_mutex);

    create_count++;

    pthread_mutex_unlock(&stats_mutex);
}

void increment_delete()
{
    pthread_mutex_lock(&stats_mutex);

    delete_count++;

    pthread_mutex_unlock(&stats_mutex);
}

void increment_modify()
{
    pthread_mutex_lock(&stats_mutex);

    modify_count++;

    pthread_mutex_unlock(&stats_mutex);
}

void increment_alert()
{
    pthread_mutex_lock(&stats_mutex);

    alert_count++;

    pthread_mutex_unlock(&stats_mutex);
}

void print_stats()
{
    pthread_mutex_lock(&stats_mutex);

    time_t end_time = time(NULL);

    int duration =
        (int)(end_time - start_time);

    printf("\n");

    printf("========== STATISTIQUES ==========\n");

    printf("Créations     : %d\n", create_count);
    printf("Suppressions  : %d\n", delete_count);
    printf("Modifications : %d\n", modify_count);
    printf("Alertes       : %d\n", alert_count);
    printf("Durée         : %d secondes\n", duration);

    printf("==================================\n");

    pthread_mutex_unlock(&stats_mutex);
}
