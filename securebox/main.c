#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>

#include "monitor.h"
#include "logger.h"
#include "stats.h"
#include "signals.h"

int main(int argc, char *argv[])
{
    pthread_t monitor_tid;

    if (argc != 2)
    {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    struct stat path_stat;

    if (stat(argv[1], &path_stat) != 0)
    {
        perror("stat");
        return 1;
    }

    if (!S_ISDIR(path_stat.st_mode))
    {
        printf("Le chemin fourni n'est pas un dossier\n");
        return 1;
    }

    init_logger();

    init_stats();

    setup_signals();

    printf("====================================\n");
    printf(" SecureBox démarre\n");
    printf(" Architecture multi-threads\n");
    printf(" Dossier surveillé : %s\n", argv[1]);
    printf("====================================\n");

    if (
        pthread_create(
            &monitor_tid,
            NULL,
            monitor_thread,
            argv[1]
        ) != 0
    )
    {
        perror("pthread_create");
        return 1;
    }

    pthread_join(monitor_tid, NULL);

    close_logger();

    return 0;
}
