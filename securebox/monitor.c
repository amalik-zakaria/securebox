#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <string.h>
#include <limits.h>

#include "monitor.h"
#include "logger.h"
#include "stats.h"
#include "analyzer.h"

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LEN (1024 * (EVENT_SIZE + 16))
#define MAX_WATCHES 100

typedef struct
{
    int wd;
    char path[PATH_MAX];

} Watch;

Watch watches[MAX_WATCHES];

int watch_count = 0;

void *monitor_thread(void *arg)
{
    char *path = (char *)arg;

    printf("[THREAD] Surveillance démarrée\n");

    start_monitor(path);

    return NULL;
}

void add_watch(int fd, const char *path)
{
    int wd = inotify_add_watch(
        fd,
        path,
        IN_CREATE |
        IN_DELETE |
        IN_MODIFY |
        IN_MOVED_FROM |
        IN_MOVED_TO
    );

    if (wd < 0)
    {
        perror("inotify_add_watch");

        return;
    }

    watches[watch_count].wd = wd;

    strncpy(
        watches[watch_count].path,
        path,
        PATH_MAX
    );

    watch_count++;

    printf("[WATCHING] %s\n", path);
}

const char *get_watch_path(int wd)
{
    for (int i = 0; i < watch_count; i++)
    {
        if (watches[i].wd == wd)
        {
            return watches[i].path;
        }
    }

    return "";
}

void start_monitor(const char *path)
{
    int fd;

    char buffer[BUFFER_LEN];

    fd = inotify_init();

    if (fd < 0)
    {
        perror("inotify_init");

        return;
    }

    add_watch(fd, path);

    printf("Surveillance active...\n");

    while (1)
    {
        int length =
            read(fd, buffer, BUFFER_LEN);

        if (length < 0)
        {
            perror("read");

            break;
        }

        int i = 0;

        while (i < length)
        {
            struct inotify_event *event =
                (struct inotify_event *)&buffer[i];

            const char *base_path =
                get_watch_path(event->wd);

            char full_path[PATH_MAX];

            snprintf(
                full_path,
                PATH_MAX,
                "%s/%s",
                base_path,
                event->name
            );

            if (event->len)
            {
                if (event->mask & IN_CREATE)
                {
                    printf(
                        "[CREATE] %s\n",
                        full_path
                    );

                    log_event(
                        "CREATE",
                        full_path
                    );

                    increment_create();

                    analyze_event(
                        "CREATE",
                        full_path
                    );

                    if (event->mask & IN_ISDIR)
                    {
                        add_watch(
                            fd,
                            full_path
                        );
                    }
                }

                if (event->mask & IN_DELETE)
                {
                    printf(
                        "[DELETE] %s\n",
                        full_path
                    );

                    log_event(
                        "DELETE",
                        full_path
                    );

                    increment_delete();

                    analyze_event(
                        "DELETE",
                        full_path
                    );
                }

                if (event->mask & IN_MODIFY)
                {
                    printf(
                        "[MODIFY] %s\n",
                        full_path
                    );

                    log_event(
                        "MODIFY",
                        full_path
                    );

                    increment_modify();

                    analyze_event(
                        "MODIFY",
                        full_path
                    );
                }
                if (event->mask & IN_MOVED_FROM)
                {
                    printf(
                        "[MOVED_FROM] %s\n",
                        full_path
                    );

                    log_event(
                        "MOVED_FROM",
                        full_path
                    );
                }

                if (event->mask & IN_MOVED_TO)
                {
                    printf(
                        "[MOVED_TO] %s\n",
                        full_path
                    );

                    log_event(
                        "MOVED_TO",
                        full_path
                    );

                    analyze_event(
                        "MOVED_TO",
                        full_path
                    );
                }
            }

            i += EVENT_SIZE + event->len;
        }
    }

    close(fd);
}
