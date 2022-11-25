#include <monitor/monitor.h>

#include "sdb.h"

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i++) {
    wp_pool[i].state = STATE_UNUSED;
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  // head = NULL;
  // free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

void all_wp_display() {
  printf("%-5s%-32s\n", "Num", "What");
  for (int i = 0; i < NR_WP; i++) {
    if (wp_pool[i].state == STATE_USED) {
      printf("%-10d%-32s\n", wp_pool[i].NO, wp_pool[i].what);
    }
  }
}

WP *new_wp() {
  for (int i = 0; i < NR_WP; i++) {
    if (wp_pool[i].state == STATE_UNUSED) {
       wp_pool[i].state = STATE_USED;
      return &wp_pool[i];
    }
  }
  assert(-1);
  return NULL;
}

WP *get_num_of_wp(int num) {
  if (num >= NR_WP) {
    assert(-1);
  }
  return &(wp_pool[num]);
}

void free_wp(WP *wp) {
  wp->state = STATE_UNUSED;
  memset(wp->what, 0, sizeof(char));
}
