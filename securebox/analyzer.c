#include <stdio.h>
#include <string.h>
#include "logger.h"
static int delete_counter = 0;

void analyze_event(const char *event, const char *file)
{
    if (strcmp(event, "DELETE") == 0)
    {
        delete_counter++;

        if (delete_counter >= 5)
        {
            printf("\n");
            printf("==============================\n");
            printf("[ALERTE] Activité suspecte !\n");
            printf("Trop de suppressions détectées\n");
            printf("==============================\n");
            printf("\n");
            log_event("ALERT", "Suppressions massives détectées");

            delete_counter = 0;
        }
    }

    if (strstr(file, ".exe") != NULL)
    {
        printf("\n");
        printf("[ALERTE] Extension suspecte détectée : %s\n", file);
        printf("\n");
        log_event("ALERT", file);
    }
}
