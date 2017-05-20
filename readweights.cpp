#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sozio_defs.h"

#define WEIGHT_INTERVAL 1000

int main(int argc, char** args) {
	if (argc<2) {
		fprintf(stderr,"Useage: %s weightfile.dat\n",args[0]);
		exit(-1);
	}

	char fWeightsName[128];
	sprintf(fWeightsName,"temp/%s",args[1]);
	FILE* fWeights=fopen(fWeightsName,"rb");
	if (!fWeights) {
		fprintf(stderr,"Could not open %s\n",fWeightsName);
		exit(-1);
	}

	char fWeightsTxtName[128];
	sprintf(fWeightsTxtName,"temp/%s.txt",args[1]);
	FILE* fWeightsTxt=fopen(fWeightsTxtName,"wt");
	if (!fWeightsTxt) {
		fprintf(stderr,"Cound not open %s\n",fWeightsTxtName);
		exit(-1);
	}
	
	/**
	 * weight left eye to dstep
	 **/
	float* left2dStep;

	/**
	 * weight right eye to dstep
	 **/
	float* right2dStep;

	/**
	 * weight left eye to dPhi
	 **/
	float* left2dPhi;

	/**
	 * weight right eye to dPhi
	 **/
	float* right2dPhi;

	/**
	 * weight dStep to dPhi
	 **/
	float* dStep2dPhi;

	/**
	 * weight dPhi to dStep
	 **/
	float* dPhi2dStep;

	/**
	 * sound2dPhi: sound-weight
	 **/
	float* sound2dPhi;

	left2dStep=new float[BUMP_N_FILTERS];
	right2dStep=new float[BUMP_N_FILTERS];
	left2dPhi=new float[BUMP_N_FILTERS];
	right2dPhi=new float[BUMP_N_FILTERS];
	dStep2dPhi=new float[BUMP_N_FILTERS];
	dPhi2dStep=new float[BUMP_N_FILTERS];
	sound2dPhi=new float[N_FILTERS_FOOD];
		
	int counter=0;
	int steps=0;
	
	while(!feof(fWeights)) {
		fread(left2dStep,
		      sizeof(left2dStep[0]),
		      BUMP_N_FILTERS,
		      fWeights);
		fread(right2dStep,
		      sizeof(right2dStep[0]),
		      BUMP_N_FILTERS,fWeights);
		fread(left2dPhi,
		      sizeof(left2dPhi[0]),
		      BUMP_N_FILTERS,
		      fWeights);
		fread(right2dPhi,
		      sizeof(right2dPhi[0]),
		      BUMP_N_FILTERS,
		      fWeights);
		
		// write out the weights from the vision-sensors as raw data
		fread(sound2dPhi,
		      sizeof(sound2dPhi[0]),
		      N_FILTERS_FOOD,
		      fWeights);
		counter--;
		if (counter<=0) {
			counter=WEIGHT_INTERVAL;
			fprintf(stderr,"%d\r",steps);
			fprintf(fWeightsTxt,"%d ",steps);
			for(int i=0;i<BUMP_N_FILTERS;i++) {
				fprintf(fWeightsTxt,"%f ",left2dStep[i]);
			}
			for(int i=0;i<BUMP_N_FILTERS;i++) {
				fprintf(fWeightsTxt,"%f ",right2dStep[i]);
			}
			for(int i=0;i<BUMP_N_FILTERS;i++) {
				fprintf(fWeightsTxt,"%f ",left2dPhi[i]);
			}
			for(int i=0;i<BUMP_N_FILTERS;i++) {
				fprintf(fWeightsTxt,"%f ",right2dPhi[i]);
			}
			for(int i=0;i<N_FILTERS_FOOD;i++) {
				fprintf(fWeightsTxt,"%f ",sound2dPhi[i]);
			}
			fprintf(fWeightsTxt,"\n");
		}
		steps++;
	}
	fclose(fWeightsTxt);
	fclose(fWeights);
	fprintf(stderr,"\n");
	
	char gnuplotName[128];
	sprintf(gnuplotName,"%s_ds.plt",args[1]);
	FILE* fgnuplot=fopen(gnuplotName,"wt");
	if (!fgnuplot) {
		fprintf(stderr,"Could not open %s\n",gnuplotName);
		exit(-1);
	}
	fprintf(fgnuplot,"set term postscript color solid\n");
	fprintf(fgnuplot,"set output \"temp/%s_ds.ps\"\n",args[1]);
	fprintf(fgnuplot,"set key\n");
	fprintf(fgnuplot,"plot ");
	int n=BUMP_N_FILTERS*2;
	for(int i=0;i<n;i++) {
		fprintf(fgnuplot,
			"\"%s\" using 1:%d with lines",
			fWeightsTxtName,2+i);
		if (i<(n-1)) {
			fprintf(fgnuplot,",");
		}
	}
	fprintf(fgnuplot,"\n");
	fclose(fgnuplot);
	char tmp[128];
	sprintf(tmp,"gnuplot %s",gnuplotName);

	if (system(tmp)) {
		fprintf(stderr,
			"gnuplot exec went wrong:%s\n",
			strerror(errno));
	}


	sprintf(gnuplotName,"%s_dphi.plt",args[1]);
	fgnuplot=fopen(gnuplotName,"wt");
	if (!fgnuplot) {
		fprintf(stderr,"Could not open %s\n",gnuplotName);
		exit(-1);
	}
	fprintf(fgnuplot,"set term postscript color solid\n");
	fprintf(fgnuplot,"set output \"temp/%s_dphi.ps\"\n",args[1]);
	fprintf(fgnuplot,"set key\n");
	fprintf(fgnuplot,"plot ");
	n=BUMP_N_FILTERS*2;
	for(int i=0;i<n;i++) {
		fprintf(fgnuplot,
			"\"%s\" using 1:%d with lines",
			fWeightsTxtName,2+i+BUMP_N_FILTERS*2);
		if (i<(n-1)) {
			fprintf(fgnuplot,",");
		}
	}
	fprintf(fgnuplot,"\n");
	fclose(fgnuplot);
	sprintf(tmp,"gnuplot %s",gnuplotName);

	if (system(tmp)) {
		fprintf(stderr,
			"gnuplot exec went wrong:%s\n",
			strerror(errno));
	}


	sprintf(gnuplotName,"%s_sound.plt",args[1]);
	fgnuplot=fopen(gnuplotName,"wt");
	if (!fgnuplot) {
		fprintf(stderr,"Could not open %s\n",gnuplotName);
		exit(-1);
	}
	fprintf(fgnuplot,"set term postscript color solid\n");
	fprintf(fgnuplot,"set output \"temp/%s_sound.ps\"\n",args[1]);
	fprintf(fgnuplot,"set key\n");
	fprintf(fgnuplot,"plot ");
	n=N_FILTERS_FOOD;
	for(int i=0;i<n;i++) {
		fprintf(fgnuplot,
			"\"%s\" using 1:%d with lines",
			fWeightsTxtName,
			2+i+BUMP_N_FILTERS*4);
		if (i<(n-1)) {
			fprintf(fgnuplot,",");
		}
	}
	fprintf(fgnuplot,"\n");
	fclose(fgnuplot);
	sprintf(tmp,"gnuplot %s",gnuplotName);

	if (system(tmp)) {
		fprintf(stderr,
			"gnuplot exec went wrong:%s\n",
			strerror(errno));
	}



}
