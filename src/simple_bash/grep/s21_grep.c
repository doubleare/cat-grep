#include "s21_grep.h"

void flags_init(struct fls *flags) {
  flags->i = 0;
  flags->ef = 0;
  flags->n = 0;
  flags->s = 0;
  flags->c = 0;
  flags->v = 0;
  flags->h = 0;
  flags->l = 0;
  flags->f = 0;
  flags->o = 0;
  flags->line = 1;
  flags->same = 0;
}

void reset(struct fls *flags) {
  flags->line = 1;
  flags->same = 0;
}

void fls_re(struct fls *flags, char res) {
  if (res == 'i')
    flags->i = 2;
  else if (res == 'n')
    flags->n = 1;
  else if (res == 'e')
    flags->ef = 1;
  else if (res == 'f')
    flags->f = 1;
  else if (res == 's')
    flags->s = 1;
  else if (res == 'c')
    flags->c = 1;
  else if (res == 'v')
    flags->v = 1;
  else if (res == 'h')
    flags->h = 1;
  else if (res == 'l')
    flags->l = 1;
  else if (res == 'o')
    flags->o = 1;
}

void addstr(char *str, char *add, int i) {
  strcat(str, add);
  if (i == 1) strcat(str, "\n");
}

void fpars(char *regs, char *file) {
  FILE *f;
  if ((f = fopen(file, "r")) == NULL) {
    fprintf(stderr, "%s: %s\n", file, strerror(errno));
  } else {
    char buf[1024];
    for (; fgets(buf, 1024, f) != NULL;)
      buf[strlen(buf) - 1] != '\n' ? addstr(regs, buf, 1)
                                   : addstr(regs, buf, 0);
    fclose(f);
  }
}

void flags_pars(struct fls *flags, char *argv[], int argc, char *regs) {
  int res;
  while ((res = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    fls_re(flags, res);
    if (optarg != NULL)
      res == 'e' ? addstr(regs, optarg, 1) : fpars(regs, optarg);
  }
}

void filename(struct fls *flags, int argc, char *argv, char s) {
  char *pattern = s == 1 ? "%s\n" : "%s:";
  if ((argc - optind > 1 && flags->h == 0) || s == 1) printf(pattern, argv);
}

void getsfroms(size_t *index, char *str, char *tmp) {
  for (int i = 0;; *index = *index + 1, i++) {
    if (str[*index] == '\n') {
      tmp[i] = '\0';
      *index = *index + 1;
      break;
    }
    tmp[i] = str[*index];
  }
}

void printres(struct fls *flags, char *argv, char *buf, int argc, int c) {
  if (c == 0 || OSIS == 1) {
    filename(flags, argc, argv, 0);
    if (flags->n == 1) printf("%d:", flags->line);
  }
  printf("%s", buf);
  if (buf[strlen(buf) - 1] != '\n') printf("%c", '\n');
}

char *strremove(char *str, const char *sub, int *c) {
  size_t l = strlen(sub);
  if (l > 0) {
    char *ptr = str;
    while ((ptr = strstr(ptr, sub)) != NULL) {
      memmove(ptr, ptr + l, strlen(ptr + l) + 1);
      *c = *c + 1;
    }
  }
  return str;
}

void flago(struct fls *flags, regmatch_t sub, char *argv, char *buf, int argc,
           int line) {
  char *word = calloc(100, sizeof(char));
  int c = line == flags->line ? 1 : 0;
  memmove(word, &buf[sub.rm_so], sub.rm_eo - sub.rm_so);
  buf = strremove(buf, word, &c);
  for (int i = line == flags->line ? 1 : 0; i < c; i++)
    printres(flags, argv, word, argc, i);
  free(word);
}

void dothings(struct fls *flags, FILE *f, int argc, char *argv, char *regs) {
  char buf[1024];
  for (int prevline = 0; fgets(buf, 1024, f) != NULL; flags->line++) {
    int fs = 0, s = 0, j = 0, fl = 0;
    for (size_t i = 0; i < strlen(regs) - 1; j++) {
      char temp[1024];
      regex_t re;
      regmatch_t sub;
      getsfroms(&i, regs, temp);
      regcomp(&re, temp, flags->i | 0 | REG_NEWLINE);
      if (regexec(&re, buf, 1, &sub, 0) == flags->v) {
        s++;
        fs = 1;
        if (flags->v == 0 && flags->c == 0 && flags->l == 0 && flags->o == 1) {
          flago(flags, sub, argv, buf, argc, prevline);
          prevline = flags->line;
          fl = 1;
        }
      }
      regfree(&re);
    }
    if (fs == 1 && fl == 0) {
      if (flags->v == 0 || (flags->v == 1 && s == j)) {
        flags->same++;
        if (flags->l == 1 && flags->same == 1) {
          break;
        } else if (flags->l == 0 && flags->c == 0) {
          if (OSIS == 0 || (OSIS == 1 && flags->o == 0))
            printres(flags, argv, buf, argc, 0);
        }
      }
    }
  }
  if (flags->c == 1) {
    if (OSIS == 0 || (OSIS == 1 && flags->l == 0)) {
      filename(flags, argc, argv, 0);
      printf("%d\n", flags->same);
    }
  }
  if (flags->l == 1 && flags->same == 1) filename(flags, argc, argv, 1);
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    FILE *f;
    int file_c;
    struct fls flags;
    char regs[1024] = {"\0"};
    flags_init(&flags);
    if (argv[1][0] == '-') flags_pars(&flags, argv, argc, regs);
    if (flags.ef == 0 && flags.f == 0) addstr(regs, argv[optind++], 1);
    file_c = optind;
    for (; argv[file_c]; file_c++) {
      if ((f = fopen(argv[file_c], "r")) == NULL) {
        if (flags.s == 0)
          fprintf(stderr, "%s: %s: %s\n", argv[0], argv[file_c],
                  strerror(errno));
      } else {
        dothings(&flags, f, argc, argv[file_c], regs);
        reset(&flags);
        fclose(f);
      }
    }
  }
  return errno;
}
