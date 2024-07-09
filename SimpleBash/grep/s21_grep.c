#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buff_size 999999

struct flags {
  int e, i, v, c, l, n, h, s, f, o;
};

int argument_parser_21(int argc, char *argv[], struct flags *flags,
                       char *pattern);
void filework(struct flags *flags, FILE *f, regex_t reg, char *file);
void grep_file(struct flags *flags, char *pattern, char *file);
int save_patt_file(char *pattern, char *temp);
void grep_main(struct flags *flags, int argc, char **argv, char *temp);

int argument_parser_21(int argc, char *argv[], struct flags *flags,
                       char *pattern) {
  int i = 0, result = 0;
  while ((i = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    if (i == 'e') {
      flags->e = 1;
      snprintf(pattern, buff_size, optarg);
    } else if (i == 'i') {
      flags->i = 1;
    } else if (i == 'v') {
      flags->v = 1;
    } else if (i == 'c') {
      flags->c = 1;
    } else if (i == 'l') {
      flags->l = 1;
    } else if (i == 'n') {
      flags->n = 1;
    } else if (i == 'h') {
      flags->h = 1;
    } else if (i == 's') {
      flags->s = 1;
    } else if (i == 'f') {
      flags->f = 1;
      snprintf(pattern, buff_size, optarg);
    } else if (i == 'o') {
      flags->o = 1;
    } else {
      result = 1;
    }
  }
  return result;
}

void grep_main(struct flags *flags, int argc, char **argv, char *temp) {
  char patternL[buff_size] = {0};
  int cols = 0;

  if (!flags->f && !flags->e) snprintf(patternL, buff_size, argv[optind++]);
  if (flags->f) cols = save_patt_file(patternL, temp);
  if (!flags->f && flags->e) snprintf(patternL, buff_size, temp);

  if (cols != -1) {
    int file_c = (argc - optind > 1) ? 1 : 0;

    for (int i = optind; i < argc; i++) {
      if (file_c && !flags->h && !flags->l) printf("%s:", argv[i]);
      grep_file(flags, patternL, argv[i]);
    }
  }
}

int save_patt_file(char *pattern, char *temp) {
  FILE *f;
  f = fopen(temp, "r");
  int i = 0;

  if (f == NULL) {
    i = -1;
  } else {
    int ch;
    while ((ch = fgetc(f)) != EOF) {
      if (ch == 13 || ch == 10) pattern[i++] = '|';
      if (ch != 13 && ch != 10) pattern[i++] = ch;
    }

    if (pattern[i - 1] == '|') pattern[i - 1] = '\0';
    fclose(f);
  }
  return (i == -1) ? -1 : (i + 1);
}

void grep_file(struct flags *flags, char *pattern, char *file) {
  int cflags = (flags->i) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
  regex_t reg;
  FILE *f;
  f = fopen(file, "r");
  if (f == NULL) {
    if (flags->s) {
      fprintf(stderr, "ERROR\t");
    }
  } else {
    regcomp(&reg, pattern, cflags);
    filework(flags, f, reg, file);
    regfree(&reg);
    fclose(f);
  }
}

void filework(struct flags *flags, FILE *f, regex_t reg, char *file) {
  char text[buff_size] = {0};
  regmatch_t pmatch[1];
  int line_matches = 0, nline = 1;

  while (fgets(text, buff_size - 1, f) != NULL) {
    int status = regexec(&reg, text, 1, pmatch, 0);
    int match = 0;
    if (status == 0 && !flags->v) match = 1;
    if (status == REG_NOMATCH && flags->v) match = 1;
    if (match && !flags->l && !flags->c && flags->n) printf("%d:", nline);
    if (match && !flags->l && !flags->c && !flags->o) printf("%s", text);

    if (flags->o && match) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", text[i]);
      }
      printf("\n");
    }
    line_matches += match;
    nline++;
  }

  if (flags->l && line_matches > 0) printf("%s\n", file);
  if (flags->c && !flags->l) printf("%d\n", line_matches);
}

int main(int argc, char *argv[]) {
  struct flags flags;
  int returnResult = 0;
  char pattern[buff_size] = {0};
  memset(&flags, 0, sizeof(flags));
  returnResult = argument_parser_21(argc, argv, &flags, pattern);
  if ((argc >= 3) && (returnResult != 1)) {
    grep_main(&flags, argc, argv, pattern);
    return returnResult;
  }
}
