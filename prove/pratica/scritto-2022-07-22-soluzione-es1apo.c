#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void list_dir(char *path, int indent) {
  DIR *dir;
  struct dirent *entry;
  dir = opendir(path);
  
  if (dir == NULL) {
    printf("Failed to open directory.\n");
    return;
  }
  while ((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_DIR) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
	
	for(int i=0; i < indent; i++) printf("\t");
        printf("%s\n", entry->d_name);
        
        char *new_path = (char *)malloc(strlen(path) + strlen(entry->d_name) + 1);
        
        sprintf(new_path, "%s/%s", path, entry->d_name);
        list_dir(new_path, indent + 1);
        free(new_path);
      }
    }else {
		for(int i=0; i < indent; i++) printf("\t");
		    printf("%s\n", entry->d_name);
	}
  }
    closedir(dir);
}

int main(int argc, char **argv) {
  if(argc < 2){
  	printf("Bad usage\n");
  }
  else{
	printf("Current directory structure:\n");
  	list_dir(argv[1], 0); 
  }
  return 0;
}
