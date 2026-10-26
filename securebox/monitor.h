#ifndef MONITOR_H
#define MONITOR_H

void *monitor_thread(void *arg);

void start_monitor(const char *path);

#endif
