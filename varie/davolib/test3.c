#include "davolib.h"

int main(){
	
	int len = getNumDirElems(".");
	char *result[len];
	init_array(result); //RICORDARSI DI INIZIALIZZARE SEMPRE GLI ARRAY!!!!!!


	lsFilesDirTree(".", result, 0);
	
	for (int i=0; i< getNumDirElems("."); i++) {
		printf("\n%s, %d", *(result + i), strlen(*(result + i)));	
	}

	return 0;

}
