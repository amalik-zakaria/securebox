#ifndef STATS_H
#define STATS_H

void init_stats();

void increment_create();
void increment_delete();
void increment_modify();
void increment_alert();

void print_stats();

#endif
