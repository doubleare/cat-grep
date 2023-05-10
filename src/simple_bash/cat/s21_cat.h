#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include "../common/s21_common.h"
struct fls {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int count;
};

void flags_init(struct fls *flags);
void fls_mac(struct fls *flags, int ch);
void flags_pars(struct fls *flags, char *argv[], int argc);
void vet_put(struct fls *flags, char ch);
void print_with_flags(struct fls *flags, FILE *f);

#endif  // SRC_CAT_S21_CAT_H_"
