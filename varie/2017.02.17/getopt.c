
// Program to illustrate the getopt()
// function in C

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int opt;

	// put ':' in the starting of the
	// string so that program can
	//distinguish between '?' and ':'
	while((opt = getopt(argc, argv, "f:hx:")) != -1)
	{
		switch(opt)
		{
			/* case 'i': */
			/* case 'l': */
			case 'h':
				printf("option: %c\n", opt);
				printf("\nHELP OPTION");
				exit(EXIT_SUCCESS);
				break;
			case 'f':
				printf("filename: %s\n", optarg);
				break;
			case 'x':
				printf("x: %s\n", optarg);
				break;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}

	// optind is for the extra arguments
	// which are not parsed
	for(; optind < argc; optind++){
		printf("extra arguments: %s\n", argv[optind]);
	}

	return 0;
}
