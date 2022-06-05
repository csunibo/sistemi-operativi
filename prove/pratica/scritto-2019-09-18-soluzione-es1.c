#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct cmd {
  char **cmd;
  size_t len;

  struct cmd *next;
  struct cmd *prev;
} cmd_t;

int parse(cmd_t *list, int argc, char *argv[]) {
  size_t len = 0;
  while (len < argc && strcmp(argv[len], "//") != 0)
    ++len;
  if (len == 0)
    return -1;
  cmd_t *e = malloc(sizeof(cmd_t));
  e->cmd = malloc(sizeof(char *) * (len + 1));
  e->len = len;

  for (size_t i = 0; i < len; ++i)
    e->cmd[i] = argv[i];
  e->cmd[len] = NULL;

  // tail insert
  e->prev = list->prev;
  e->next = list;
  list->prev->next = e;
  list->prev = e;
  return len + 1;
}

int main(int argc, char *argv[]) {
  size_t max;
  cmd_t head = {NULL, 0};
  head.next = &head;
  head.prev = &head;
  if (argc < 3) {
    printf("Usage: %s MAX_CURR [...CMD1] // [...CMD2]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  max = atoi(argv[1]);

  size_t pos = 2, inc = 0;
  while (pos < argc && (inc = parse(&head, argc - pos, &argv[pos])) != -1)
    pos += inc;

  cmd_t *it = head.next;
  while (it != &head) {
    printf("cmd ");
    for (size_t i = 0; i < it->len; ++i)
      printf("%s ", it->cmd[i]);
    printf("\n");
    it = it->next;
  }

  it = head.next;
  size_t running = 0;
  while (it != &head) {
    pid_t p;
    if (++running == max + 1) {
      if (wait(NULL) > 0)
        --running;
    }
    switch ((p = fork())) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
      break;
    case 0:;
      // child
      execvp(it->cmd[0], it->cmd);
      break;
    }
    it = it->next;
  }

  while (wait(NULL) > 0)
    --running;

  // free
  it = head.next;
  while (it != &head) {
    cmd_t *tmp = it->next;
    free(it->cmd);
    free(it);
    it = tmp;
  }

  exit(EXIT_SUCCESS);
}
