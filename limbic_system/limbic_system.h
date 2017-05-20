#ifndef _LIMBIC_SYSTEM
#define _LIMBIC_SYSTEM

#include "isolearning/isolearning.h"
#include "isolearning/trace.h"
#include <unistd.h>



// This is the D2 factor 
#define DFACT 0.3

// baseline of the VTA
#define BASELINE_VTA 1

// this is the time constant of the D2 receptor
#define TAU_D2 0.000001

// this is the pathway from the Nacc to the LH
#define NACC_LH 0.01

// this is the pathway from the Nacc to the VTA
#define NACC_VTA 0.005

// this is the pathway from the Nacc to the VP
#define NACC_VP 0.1

// this is the pathway from the VP to the VTA
#define VP_VTA 1

// the actual gain for LTP
#define LTP_GAIN 1

// the actual gain for LTD
#define LTD_GAIN 10

// set this to 4 for Cannabis, 0 for normal cond
#define CB_DISINH 0

// switch third factors on
#define THIRD_FACTOR 1

// also unlearning
#define WITH_LTD 1

// switches on learning in the shell
#define SHELL_LEARNING 1

// no traces so far
#define TRACES 0

// no reflex traces so far
//#define ReflexTRACES 1

/**
 * frequ for the food reflex
 **/
#define F_FOOD_FIXED 0.01

/**
 * Q for the food reflex
 **/
#define Q_FOOD_FIXED 0.51

/**
 * The highest frequency for the filter-bank
 **/
#define F_FOOD_LEARN 0.01

/**
 * The distance between 2 different frequencies in the filter bank
 **/
#define F_FOOD_FACTOR 1

/**
 * The quality of the resonators in the filter-bank
 **/
#define Q_FOOD 0.51


/// place cell...

/**
* frequ for the place cell  reflex
 **/
#define F_PLACEFIELD_FIXED 0.01

/**
 * Q for the place cell reflex
 **/
#define Q_PLACEFIELD_FIXED 0.51

/**
 * The highest frequency for the filter-bank
 **/
#define F_PLACEFIELD_LEARN 0.1

/**
 * The distance between 2 different frequencies in the filter bank
 **/
#define F_PLACEFIELD_FACTOR 1

/**
 * The quality of the resonators in the filter-bank
 **/
#define Q_PLACEFIELD 1



/**
 * The learning rate for the sound thing
 **/
#define MU_SHELL 0.5





class Limbic_system {
public:
	Limbic_system();
	~Limbic_system();

	void doStep(float us,
		    float cs1,
		    float cs2,
		    float x0LG,
		    float x0DG, 
		    float xLG, 
		    float xDG
		    );

	float getLGOutput() {return CoreLGOut;};
	float getDGOutput() {return CoreDGOut;};
	void setLearningRate(float mu);

	Isolearning* isolearningShell;
	Isolearning* isolearningCoreLG;
	Isolearning* isolearningCoreDG;

	double lastVTA;
	double d2_auto;
	double d2_receptor;
	double da_reup;

	double lastVTA_sh;
	double d2_auto_sh;
	double d2_receptor_sh;
	double da_reup_sh;

	float CoreLGOut;
	float CoreDGOut;
	int posCS;
	int ltdCounter;
	FILE* f;
	FILE* fcore;
	long int step;
};

#endif
