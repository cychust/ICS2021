#include "sdb.h"

#include <cpu/cpu.h>
#include <isa.h>
#include <memory/paddr.h>
#include <monitor/monitor.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "stdlib.h"

static int is_batch_mode = false;

vaddr_t brk_pool[NR_WP] = {};

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin.
 */
static char *rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
  char *arg = strtok(NULL, " ");
  int N;
  if (arg == NULL) {
    N = 1;
  } else {
    N = atoi(arg);
  }
  cpu_exec(N);
  return 0;
}

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    printf("info param is error");
    return -1;
  }
  switch (arg[0]) {
    case /* constant-expression */ 'r':
      /* code */
      isa_reg_display();
      break;
    case 'w':
      all_wp_display();
      break;
    default:
      printf("info param is error");
      break;
  }
  return 0;
}

static int cmd_b(char *args) {
  char *arg1 = strtok(NULL, " ");
  if (arg1 == NULL) {
    return -1;
  }
  bool success;
  word_t addr = expr(arg1, &success);
  for (int i = 0; i < NR_WP; i++) {
    if (0 == brk_pool[i]) {
      brk_pool[i] = addr;
      printf(" i = %d,break at 0x%08x\n", i, addr);
      return 0;
    }
  }
  printf("err add brk\n");
  return 0;
}

static int cmd_x(char *args) {
  char *arg1 = strtok(NULL, " ");
  if (arg1 == NULL) {
    return -1;
  }
  int n = atoi(arg1);
  char *arg2 = strtok(NULL, "\0");
  if (arg2 == NULL) {
    return -1;
  }
  bool success;
  word_t addr = expr(arg2, &success);
  for (int i = 0; i < n; i++) {
    /* code */
    word_t mem = paddr_read(addr + 4 * i, 4);
    printf("mem[0x%8x] = %08x\n", addr + 4 * i, mem);
  }
  return 0;
}
static int cmd_p(char *args) {
  bool success;
  char *arg = strtok(NULL, "\0");
  if (arg == NULL) {
    return -1;
  }
  printf("expr = %s\n", arg);

  word_t res = expr(arg, &success);
  if (!success) {
    return -1;
  }
  printf("%d\n", res);
  return 0;
}
static int cmd_w(char *args) {
  char *arg = strtok(NULL, "\0");
  if (arg == NULL) {
    return -1;
  }
  printf("w what = %s\n", arg);
  WP *nwp = new_wp();
  memccpy(nwp->what, arg, 1, strlen(arg));
  return 0;
}
static int cmd_d(char *args) {
  char *arg = strtok(NULL, "\0");
  if (arg == NULL) {
    return -1;
  }
  int num = atoi(arg);
  free_wp(get_num_of_wp(num));
  return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"b", "Set breakpoints", cmd_b},
    {"q", "Exit NEMU", cmd_q},
    {"si", "Single Step Execution", cmd_si},  // si [N]
    {"info", "Print reg state", cmd_info},    // info r: reg , w: watchpoint
    {"x", "Scan memory", cmd_x},
    {"p", "Get result of expr", cmd_p},
    {"w", "Set watchpoint", cmd_w},
    {"d", "Delete watchpoint", cmd_d}
    /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  } else {
    for (i = 0; i < NR_CMD; i++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() { is_batch_mode = true; }

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL;) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) {
      continue;
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) {
          return;
        }
        break;
      }
    }

    if (i == NR_CMD) {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
