#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include "../common/s21_common.h"
#ifndef _REGEX_H_
#include <regex.h>
#endif
#ifndef _STDLIB_H_
#include <stdlib.h>
#endif
struct fls {
  int i;
  int ef;
  int n;
  int s;
  int c;
  int v;
  int h;
  int l;
  int f;
  int o;
  int line;
  int same;
};

void reset(struct fls *flags);
void fls_re(struct fls *flags, char res);
void flags_pars(struct fls *flags, char *argv[], int argc, char *regs);
void filename(struct fls *flags, int argc, char *argv, char s);
void dothings(struct fls *flags, FILE *f, int argc, char *argv, char *regs);

#endif  // SRC_GREP_S21_GREP_H_"
