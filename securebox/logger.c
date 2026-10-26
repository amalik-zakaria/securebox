#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *log_file = NULL;

void init_logger()
{
    system("mkdir -p logs");

    log_file =
        fopen("logs/securebox.log", "a");

    if (log_file == NULL)
    {
        perror("fopen");

        exit(1);
    }
}

void log_event(
    const char *type,
    const char *file
)
{
    time_t now;

    struct tm *timeinfo;

    time(&now);

    timeinfo = localtime(&now);

    const char *level = "[INFO]";

    if (
        strcmp(type, "ALERT") == 0
    )
    {
        level = "[ALERTE]";
    }

    fprintf(
        log_file,
        "[%04d-%02d-%02d %02d:%02d:%02d] %s %s : %s\n",
        timeinfo->tm_year + 1900,
        timeinfo->tm_mon + 1,
        timeinfo->tm_mday,
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec,
        level,
        type,
        file
    );

    fflush(log_file);
}

void close_logger()
{
    if (log_file != NULL)
    {
        fclose(log_file);
    }
}
