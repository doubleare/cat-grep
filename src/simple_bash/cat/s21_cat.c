#include "s21_cat.h"

void flags_init(struct fls *flags) {
  flags->b = 0;
  flags->e = 0;
  flags->n = 0;
  flags->s = 0;
  flags->t = 0;
  flags->v = 0;
  flags->count = 1;
}

void fls_mac(struct fls *flags, int ch) {
  if (ch == 'b')
    flags->b = 1;
  else if (ch == 'v')
    flags->v = 1;
  else if (ch == 'n')
    flags->n = 1;
  else if (ch == 's')
    flags->s = 1;
  if (ch == 'e') {
    flags->e = 1;
    flags->v = 1;
  }
  if (ch == 'E') flags->e = 1;
  if (ch == 't') {
    flags->t = 1;
    flags->v = 1;
  }
  if (ch == 'T') flags->t = 1;
}

void flags_pars(struct fls *flags, char *argv[], int argc) {
  int res;
  const struct option long_options[] = {
      {"number-nonblank", no_argument, &flags->b, 1},
      {"number", no_argument, &flags->n, 1},
      {"squeeze-blank", no_argument, &flags->s, 1},
      {NULL, 0, NULL, 0}};
  while ((res = getopt_long(argc, argv, "benstTEv", long_options, 0)) != -1)
    fls_mac(flags, res);
}

void vet_put(struct fls *flags, char ch) {
  if (flags->e == 1 && ch == '\n') printf("$\n");
  if (flags->t == 1 && ch == '\t') printf("^%c", ch + 64);
  if (flags->v == 1) {
    if (ch < -127)
      printf("M-^%c", ch + 192);
    else if (ch < 0)
      printf("M-%c", ch + 128);
    else if (ch < 32 && ch != 9 && ch != 10)
      printf("^%c", ch + 64);
    else if (ch >= 127)
      printf("^?");
  }
  if ((ch > 31 && ch < 127) || (flags->e == 0 && ch == '\n') ||
      (flags->t == 0 && ch == '\t') ||
      ((flags->v == 0 && ch != '\n') && (flags->v == 0 && ch != '\t')))
    printf("%c", ch);
}

void print_with_flags(struct fls *flags, FILE *f) {
  char ch = getc(f);
  for (int tmp = -1, befo = tmp; ch != EOF; ch = getc(f)) {
    if (flags->s == 1) {
      if (ch == '\n' && tmp == '\n' && befo == '\n') {
        for (; ch == '\n' && ch != EOF; ch = getc(f))
          ;
      }
    }
    if (flags->b == 1 &&
        ((tmp == -1 && ch != '\n') || (tmp == '\n' && ch != '\n'))) {
      printf("%6d\t", flags->count);
      flags->count++;
    } else if (flags->b == 0 && flags->n == 1 && (tmp == -1 || tmp == '\n')) {
      printf("%6d\t", flags->count);
      flags->count++;
    }
    if (flags->e == 1 || flags->v == 1 || flags->t == 1)
      vet_put(flags, ch);
    else
      printf("%c", ch);
    befo = tmp;
    tmp = ch;
  }
  if (OSIS == 0) flags->count = 1;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    FILE *f;
    int file_c;
    struct fls flags;
    flags_init(&flags);
    if (argv[1][0] == '-') flags_pars(&flags, argv, argc);
    file_c = optind;
    for (; argv[file_c] && file_c <= argc; file_c++) {
      if ((f = fopen(argv[file_c], "r")) == NULL) {
        fprintf(stderr, "%s: %s: %s\n", argv[0], argv[file_c], strerror(errno));
      } else if (flags.b == 0 && flags.e == 0 && flags.n == 0 && flags.s == 0 &&
                 flags.t == 0 && flags.v == 0) {
        char ch = getc(f);
        for (; ch != EOF; ch = getc(f)) printf("%c", ch);
      } else {
        print_with_flags(&flags, f);
      }
      fclose(f);
    }
  }
  return errno;
}
