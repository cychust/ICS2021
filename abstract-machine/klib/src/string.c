#include <klib-macros.h>
#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *sc;
  for (sc = s; *sc != '\0'; ++sc)
    ;
  return sc - s;
}

char *strcpy(char *dst, const char *src) {
  char *tmp = dst;

  while ((*dst++ = *src++) != '\0') /* nothing */
    ;
  return tmp;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *tmp = dst;
  while ((*dst++ = *src++) && (--n))
    ;
  return tmp;
}

char *strcat(char *dst, const char *src) {
  char *rdst = dst;
  while (*dst) {
    dst++;
  }
  while ((*dst++ = *src++) != '\0')
    ;
  return rdst;
}

int strcmp(const char *s1, const char *s2) {
  unsigned char c1, c2;
  while (1) {
    c1 = *s1++;
    c2 = *s2++;
    if (c1 != c2) return c1 < c2 ? -1 : 1;
    if (!c1) break;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  unsigned char u1, u2;

  while (n-- > 0) {
    u1 = (unsigned char)*s1++;
    u2 = (unsigned char)*s2++;
    if (u1 != u2) return u1 - u2;
    if (u1 == '\0') return 0;
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  char *xs = (char *)s;
  while (n--) {
    *xs++ = c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  uint8_t *from = (uint8_t *)src;
  uint8_t *to = (uint8_t *)dst;

  if (from == to || n == 0) return dst;
  if (to > from && to - from < (int)n) {
    /* to overlaps with from */
    /*  <from......>         */
    /*         <to........>  */
    /* copy in reverse, to avoid overwriting from */
    int i;
    for (i = n - 1; i >= 0; i--) to[i] = from[i];
    return dst;
  }
  if (from > to && from - to < (int)n) {
    /* to overlaps with from */
    /*        <from......>   */
    /*  <to........>         */
    /* copy forwards, to avoid overwriting from */
    size_t i;
    for (i = 0; i < n; i++) to[i] = from[i];
    return dst;
  }
  memcpy(dst, src, n);
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  char *xout = (char *)out;
  char *xin = (char *)in;

  while (n--) {
    *xout++ = *xin++;
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char u1, u2;

  for (; n--; s1++, s2++) {
    u1 = *(unsigned char *)s1;
    u2 = *(unsigned char *)s2;
    if (u1 != u2) {
      return (u1 - u2);
    }
  }
  return 0;
}

#endif
