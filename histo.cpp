#include<stdio.h>
#include<stdlib.h>

#define BIN 50

int main(int nargs, char ** filename) {
	if (nargs<2) {
		printf("usage: %s <contacts>.dat <histofile>.dat\n",
		       filename[0]);
		exit(1);
	}
	FILE *f=fopen(filename[1],"rt");
	if (!f) {
		printf("%s missing\n",filename[1]);
		exit(1);
	}
	FILE *h=fopen(filename[2],"wt");
	if (!h) {
		printf("%s err\n",filename[2]);
	}
	int counts=0;
	int binCtr=BIN;
	float histMu=-1;
	while (!feof(f)) {
		float mu;
		int n;
		fscanf(f,"%f %d\n",&mu,&n);
		if (histMu<0) { // frist mu
			histMu=mu;
		}
		printf("%f %d\n",mu,n);
		if (n<4) { // got a failure
			counts++;
		}
		binCtr--;
		if (!binCtr) {
			fprintf(h,"%f %d\n",(mu+histMu)/2.0,counts);
			counts=0;
			histMu=mu;
			binCtr=BIN;
		}
	}
	fclose(f);
	fclose(h);
}
