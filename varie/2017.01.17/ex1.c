/* Scrivere un programma che conti I file ordinari e le directory presenti nel sottoalbero della directory passata come */
/* parametro (o della directory corrente se non viene passato alcun parametro). */
/* Ogni file o directory deve venir contato una sola volta anche se e’ presente con diversi nomi a causa dei link fisici. */
/* Ogni altro tipo di file che non sia file ordinario o directory (e.g. socket, named pipe, file speciali, link simbolici) */
/* devono essere ignorati. */
/* Completare il programma precedente in modo che classifichi I file in base al numero di nomi (link) che hanno. */
/* L’output deve indicare quanti file hanno un solo nome, quanti ne hanno due, tre e cosi’ via. */
/* Si devono tenere in considerazione solamente I link presenti all’interno del sottoalbero */

#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "dict.h"
#define MAX_FILES 4096
#define MAX_LINKS 128
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define BUFSIZE 512
#define FILES_TO_CMP 2
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

unsigned inodes[MAX_FILES];
int not_in_array(unsigned long inode){
  for (int j=0; j< MAX_FILES; j++){
    if (inodes[j] == inode) return FALSE;
  }
  return TRUE;
}

int files = 0, dirs = 0, i = 0;
void list_dir(int dirfd, struct dict *dd) {
  /* void list_dir(char *path, int indent) { */
  struct dirent *entry;
  DIR *dir = fdopendir(dirfd);
  /* DIR *dir = opendir(path); */

  if (dir == NULL) {
    printf("Failed to open directory.\n");
    return;
  }
  while ((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_DIR) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        if (not_in_array(entry->d_ino)) {

          dirs++;
          printf("\n%s contato, i vale %d", entry->d_name, i);

          inodes[i] = entry->d_ino; i++;
          int one = 1;
          char buf[256]; INIT_ARRAY(buf);
          snprintf(buf, sizeof(buf), "%ld", entry->d_ino);
          dict_set(dd, buf, (void *)&one);
          /* for(int i=0; i < indent; i++) printf("\t"); */
          /* printf("%s\n", entry->d_name); */

          /* char *new_path = (char *)malloc(strlen(path) + strlen(entry->d_name) + 1); */

          /* sprintf(new_path, "%s/%s", path, entry->d_name); */
          int newDirfd = openat(dirfd, entry->d_name, O_RDONLY);
          list_dir(newDirfd, dd);
        }
        else {
          char buf[256]; INIT_ARRAY(buf);
          snprintf(buf, sizeof(buf), "%ld", entry->d_ino);
          int val = *(int *)dict_get(dd, buf);
          val++;
          dict_set(dd, buf, (void *)&val);
        }
        /* free(new_path); */
      }
    }else {
      /* for(int i=0; i < indent; i++) printf("\t"); */
      /* printf("%s\n", entry->d_name); */
      if ( entry->d_type == DT_REG ){
        if ( not_in_array(entry->d_ino) ){
          files++;
          printf("\n%s contato, i vale %d", entry->d_name, i);

          inodes[i] = entry->d_ino; i++;
          int one = 1;
          char buf[256]; INIT_ARRAY(buf);
          snprintf(buf, sizeof(buf), "%ld", entry->d_ino);
          dict_set(dd, buf, (void *)&one);

        }
        else {
          char buf[256]; INIT_ARRAY(buf);
          snprintf(buf, sizeof(buf), "%ld", entry->d_ino);
          int val = *(int *)dict_get(dd, buf);
          val++;
          dict_set(dd, buf, (void *)&val);
        }
      }
    }
  }
  closedir(dir);
}

int main(int argc, char **argv) {
  INIT_ARRAY(inodes);
  struct dict *dd = dict_new(MAX_FILES, sizeof(unsigned long));

  if(argc < 2){
    printf("Bad usage\n");
  }

  else{
    /* printf("Current directory structure:\n"); */
    int fd = openat(AT_FDCWD, argv[1], O_RDONLY);
    /* printf("%d\n", fd); */

    list_dir(fd, dd);
    int counts[MAX_FILES]; INIT_ARRAY(counts);
    printf("\ntrovati %d files e %d dirs", files, dirs);
    for (int j=0; j < i; j++){
      char buf[256]; INIT_ARRAY(buf);
      snprintf(buf, sizeof(buf), "%d", inodes[j]);
      printf("\nkey: %s, value: %d", buf, *(int *)dict_get(dd, buf));

      counts[*(int *)dict_get(dd, buf)]++;
    }
    for (int j=0; j< i; j++){
      if ( counts[j] == 0 ) continue;
      printf("\nci sono %d file che hanno %d link", counts[j], j);
    }
    /* list_dir(argv[1], 0); */


  }


  exit(EXIT_SUCCESS);
}

