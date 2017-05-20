#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sozio_defs.h"


void mov2rm() {
	struct dirent **namelist;
	int n;
		
	n = scandir("temp/.", &namelist, 0, alphasort);
	if (n < 0)
		perror("scandir");
	else {
		while(n--) {
			char* movPos=strstr(namelist[n]->d_name,".mov");
			if (movPos) { // movPos
				*movPos=0;
				char movName[128];
				sprintf(movName,
					"temp/%s.mov",
					namelist[n]->d_name);
				char rmName[128];
				sprintf(rmName,
					"temp/%s.rm",
					namelist[n]->d_name);
				free(namelist[n]);
				printf("%s --> %s\n",movName,rmName);
				char command[1024];
				sprintf(command,"realproducer -b \"ISO-learning\" -h \"Bernd Porr and Florentin Woergoetter\" -c \"University of Stirling\" -q \"ISO-learning\" -n \"Sequence Learning, Robotics, Constructivism, Autonomy\" -r 1 -k 1 -f 0 -ow -v 2 -t 6 -i %s -o %s",movName,rmName);
				if (system(command)==-1) {
					fprintf(stderr,
						"realproducer %s\n",
						command);
					fprintf(stderr,
						"realpr. did not start:%s\n",
						strerror(errno));
				}
				FILE* f=fopen(rmName,"rb");
				if (f) {
					fclose(f);
					if (unlink(movName)==-1) {
						fprintf(stderr,
							"Could not delete %s\n",
							movName);
					}
				} else {
					fprintf(stderr,
						"File %s is not gen!\n",
						movName);
				}
			}
		}
		free(namelist);
	}
}



void main(int,char**) {
	while (1) {
		mov2rm();
		sleep(10);
	}
}
