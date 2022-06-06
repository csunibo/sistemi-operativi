#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
int main(int argc, char * argv[]) {
	int ngid = getgroups(0,NULL);
	gid_t gids[ngid];

	getgroups(ngid, gids);
	for (int i = 0; i < ngid; i++)
		printf("%d\n", gids[i]);
}
