

#ifndef __KERVICE_H
#define __KERVICE_H

void *ly_memcpy(void *dst, const void *src, unsigned long count);

char *ly_strncpy(char *dst, const char *src, unsigned long n);

void *ly_memset(void *s, int c, unsigned long count);

void *ly_memmove(void *dest, const void *src, unsigned long n);

unsigned long ly_strlen(const char *s);

signed long ly_strncmp(const char *cs, const char *ct, unsigned long count);

void *rt_memmove(void *dest, const void *src,  unsigned long n);

signed long ly_memcmp(const void *cs, const void *ct,  unsigned long count);

signed long ly_strcmp(const char *cs, const char *ct);

#endif

