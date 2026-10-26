#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "stats.h"

void handle_signal(int sig)
{
    printf("\n");

    if (sig == SIGINT)
    {
        printf("[SIGNAL] SIGINT reçu\n");
    }

    if (sig == SIGTERM)
    {
        printf("[SIGNAL] SIGTERM reçu\n");
    }

    printf("Arrêt propre du programme...\n");

    print_stats();

    printf("Programme terminé proprement\n");

    exit(0);
}

void setup_signals()
{
    signal(SIGINT, handle_signal);

    signal(SIGTERM, handle_signal);
}
