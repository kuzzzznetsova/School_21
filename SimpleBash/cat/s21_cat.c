#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arg_s_21 {
  int b, n, s, E, T, v;
} arg_s_21;

arg_s_21 argument_parser_21(int argc, char** argv) {
  arg_s_21 arg = {0};
  struct option long_opt[] = {{"number", no_argument, NULL, 'n'},
                              {"number-nonblank", no_argument, NULL, 'b'},
                              {"squeeze-blank", no_argument, NULL, 's'},
                              {0, 0, 0, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "bnsEeTtsv", long_opt, 0)) != -1) {
    switch (opt) {
      case 'b':
        arg.b = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'e':
        arg.E = 1;
        arg.v = 1;
        break;
      case 'E':
        arg.E = 1;
        break;
      case 'T':
        arg.T = 1;
        break;
      case 't':
        arg.T = 1;
        arg.v = 1;
        break;
      case '?':
        perror("ERROR");
        exit(1);
      default:
        break;
    }
  }
  return arg;
}

char v_output(char ch) {
  if (ch == '\n' || ch == '\t') return ch;

  if (ch > 0 && ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

void outline(arg_s_21* arg, char* line, int n) {
  for (int i = 0; i < n; i++) {
    if (arg->T && line[i] == '\t') {
      printf("^I");
    }
    if (arg->E && line[i] == '\n') {
      putchar('$');
    }
    if (arg->v) line[i] = v_output(line[i]);
    if (!(arg->T && line[i] == '\t')) {
      putchar(line[i]);
    }
  }
}

void output(arg_s_21* arg, int argc, char** argv) {
  for (int i = optind; i < argc; i++) {
    FILE* f = fopen(argv[i], "r");
    if (f) {
      char* line = NULL;
      size_t memoryline = 0;
      int read = 0;

      int empty_count = 0;
      int line_count = 1;
      read = getline(&line, &memoryline, f);
      while (read != -1) {
        if (arg->s) {
          if (line[0] == '\n' && empty_count == 0) {
            empty_count++;
            printf("\n");
          } else if (line[0] != '\n') {
            outline(arg, line, read);
            empty_count = 0;
          }
        } else {
          if (arg->n || arg->b) {
            if (arg->b && line[0] != '\n') {
              printf("%6d\t", line_count);
              line_count++;
            } else {
              if (arg->n) {
                printf("%6d\t", line_count);
                line_count++;
              }
            }
          }
          outline(arg, line, read);
        }
        read = getline(&line, &memoryline, f);
      }
      free(line);
    }
    fclose(f);
  }
}

int main(int argc, char** argv) {
  arg_s_21 arg = argument_parser_21(argc, argv);

  output(&arg, argc, argv);

  return 0;
}
