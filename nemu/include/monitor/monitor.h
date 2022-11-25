#ifndef __MONITOR_MONITOR_H__
#define __MONITOR_MONITOR_H__

#include <common.h>

#define NR_WP 32

enum {
  STATE_UNUSED = 0,
  STATE_USED,
};

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char what[32];
  int state;
} WP;

static WP wp_pool[NR_WP] __attribute__((unused)) = {};
// static WP *head = NULL, *free_ = NULL;

word_t expr(char *e, bool *success);
void all_wp_display();
WP *new_wp();
WP *get_num_of_wp(int num);
void free_wp(WP *wp);

#endif
