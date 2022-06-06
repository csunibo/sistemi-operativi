#include <stdio.h>
 
int slen(char *s) { /* in real programs use strlen instead */
  size_t rval;
  for (rval = 0; *s != 0; s++, rval++)
    ;
  return rval;
}
 
void echoargs(int argc, char *argv[]) {
  int i;
  for (i = 0; i < argc; i++)
    printf("argv[%d] = \"%s\"\n",i,argv[i]);
  printf("\n");
}
 
enum state {SPACE, CHAR};
 
int splitargv(char *s, char **argv) {
  enum state state = SPACE;
  int count = 0;
  for (; *s != 0; s++) {
    if (*s == ' ' || *s == '\t' || *s == '\n') {
      if (state != SPACE)
        count++;
      if (argv != NULL)
        *s = 0;
      state = SPACE;
    } else {
      if (state == SPACE && argv != NULL)
        *argv++ = s;
      state = CHAR;
    }
  };
  if (state != SPACE)
    count++;
  if (argv != NULL)
    *argv = NULL;
  return count;
}
 
void splitargs(char *args) {
  int newargc = splitargv(args, NULL);
  char *newargv[newargc + 1];
  splitargv(args, newargv);
  echoargs(newargc, newargv);
}
 
int main(int argc, char *argv[1]) {
  echoargs(argc, argv);
  for ( ; *argv != NULL; argv++) {
    printf("Split \"%s\"\n",*argv);
    splitargs(*argv);
  }
}
