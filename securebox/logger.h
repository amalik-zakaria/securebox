#ifndef LOGGER_H
#define LOGGER_H

void init_logger();
void log_event(const char *type, const char *file);
void close_logger();

#endif
