#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) { panic("Not implemented"); }

void itoa(int num, char *buf) {
  int isNegative = 0;
  if (num == 0) {
    buf[0] = '0';
    buf[1] = '\0';
    return;
  }

  if (num < 0) {
    isNegative = 1;
    num = -num;
  }
  int i = 0;
  while (num != 0) {
    buf[i++] = num % 10 + '0';
    num = num / 10;
  }
  if (isNegative == 1) {
    buf[i++] = '-';
  }
  buf[i] = '\0';
  for (int j = 0; j <= (i - 1) / 2; j++) {
    char tmp = buf[j];
    buf[j] = buf[i - 1 - j];
    buf[i - 1 - j] = tmp;
  }
  return;
}

void sprintf_base(char **pout, char **pin, va_list *args) {
  // switch (**pin) {
  //   case 'd': {
  //     char buf[32] = {0};
  //     int num = va_arg(*args, int);
  //     itoa(num, buf);

  //     memcpy(*pout, buf, strlen(buf));
  //     (*pout) += strlen(buf);
  //     break;
  //   }
  //   case 's': {
  //     char *s = va_arg(*args, char *);
  //     memcpy(*pout, s, strlen(s));
  //     (*pout) += strlen(s);

  //     break;
  //   }
  //   default:
  //     // printf("dfsfd\n");
  //     break;
  // }
}

int vsprintf(char *buf, const char *fmt, va_list args) {
  char *pout = buf;
  char *pin = (char *)fmt;
  while (*pin) {
    switch (*pin) {
      case '%': {
        pin++;
        // sprintf_base(&pout, &pin, &args);
        switch (*pin) {
          case 'd': {
            char buf[32] = {0};
            int num = va_arg(args, int);
            itoa(num, buf);

            memcpy(pout, buf, strlen(buf));
            (pout) += strlen(buf);
            break;
          }
          case 's': {
            char *s = va_arg(args, char *);
            memcpy(pout, s, strlen(s));
            (pout) += strlen(s);

            break;
          }
          default:
            printf("dfsfd\n");
            break;
        }
        pin++;
        break;
      }

      default: {
        *pout = *pin;
        pout++;
        pin++;
        break;
      }
    }
  }
  *pout = '\0';
  return pout - buf;
}

int sprintf(char *buf, const char *fmt, ...) {
  va_list args;
  int i;

  va_start(args, fmt);
  i = vsprintf(buf, fmt, args);
  va_end(args);

  return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

// https://elixir.bootlin.com/linux/latest/source/lib/vsprintf.c#L2712
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args) {
  return 0;
}

#endif
