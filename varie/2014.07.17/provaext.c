#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
const char *get_filename_name(char *path){
	/* https://stackoverflow.com/questions/5309471/getting-file-extension-in-c */
	const char *dot = strrchr(path, '.');
	if(!dot || dot == path) return path;
	char *buf = malloc(PATH_MAX*sizeof(char));
	snprintf(buf, dot - path + 1, "%s", path);
	return buf;
}

const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if(!dot || dot == filename) return "";
	return dot + 1;
}
int main(int argc, char **argv){

	printf("%s", argv[1]);
	/* printf("%s\n", get_filename_name("test.tiff")); */
	/* printf("%s\n", get_filename_name("test.blah.tiff")); */
	/* printf("%s\n", get_filename_name("test.")); */
	/* printf("%s\n", get_filename_name("test")); */
	/* printf("%s\n", get_filename_name("...")); */
}
