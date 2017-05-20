/***********************************************************************
 * (C) Copyright 2005 Bernd Porr, http://www.berndporr.me.uk           *
 *                                                                     *
 * This is ISO learning, partially covered by a european patent        *
 *                                                                     *
 ***********************************************************************/

#ifndef _Isolearning
#define _Isolearning


#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include"trace.h"


// Limits the actual activity to a max value
#define LIMIT_WEIGHTS 0.1

// No negative weights
#define NO_NEG_WEIGHTS 1


/**
 * Implements ISO-learning with the help of an array of filters.
 **/
class Isolearning {

 public:
  /**
   * Constructor. Number of inputs=2 means that there is one reflex pathway
   * and one predictive pathway. The predictive pathway has a filter
   * bank with 10 filters. If there are more inputs they will be automatically
   * predictive inputs with filter banks. The first input (reflex) has only one
   * filter.
   **/
  Isolearning(int nInputs=2, int nFi=10);

 public:
  /**
   * Destructor
   **/
  ~Isolearning();

 public:
  /**
   * Sets the input value for input i to the value f. Input 0 is always
   * the reflex pathway. Any other inputs are predictive pathways.
   **/
  void setInput(int i,float f);

 public:
  /**
   * Sets the 3rd factor. This is a global multiplicative factor which affects
   * all weights. It can be positive or negative. It just scales the learning rate.
   **/
  void set3rdFactor(float tf=1) {thirdFactor=tf;};

 public:
  void setDecay(float d=0) {decay=d;};

 public:
  /**
   * Sets the learning rate
   **/
  void setLearningRate(float rate=0.0001) {learningRate=rate;};

 public:
  /**
   * Get the learning rate
   **/
  //  float getLearningRate(float rate) {return learningRate;};

  /**
   * sets a weight
   **/
  void setWeight(int channel, int index, float w) {
	  weight[channel][index]=w;
  }

  /**
   * get a pointer to a weight
   **/
  float getWeight(int channel, int index) {
	  return weight[channel][index];
  }
  
 public:
  /**
   * This is ISO learning. Every step the weights are updated and
   * the output is calculated.
   **/
  void prediction(int step);

 public:
  /**
   * Starts documentation. The prefix is used for all file names.
   **/
  void openDocu(char* prefix);

 public:
  /**
   * Documents the output, the Filters and the weights and the step.
   * This has to be called every step or whenever you want to
   * have written the values to the files. The output format
   * is gnuplot compatible.
   **/
  void writeDocu(int step);

 public:
  /**
   * The output of the learner circuit
   **/
  float getOutput() {return actualActivity;}; 

  /**
   * The derivative output of the learner circuit
   **/
  float getDeltaOutput() {return delta;};

 public:
  /**
   * sets the resonator for the reflex
   **/
  void setReflex(float f0=0.01, // frequency
		 float q0=0.6  // quality
		 );

 public:
  /**
   * set the Predictor as a filter bank of resonators.
   * The frequency defines highest resonator frequency. If there
   * is a filterbank the frequency is divided by the index numbrer
   * of the resonator: f_j=f1/j.
   **/
  void setPredictorsAsBandp(float f1=0.1,  // highest f in filterbank
			    float q1=0.6   // Q in filterbank for all resonators
			    );

 public:
  /**
   * set the Predictor as a filter bank of FIR traces: 
   * The number of taps define the maximum delay.
   * The number of taps and tau are devided by the filter index to
   * create a filter bank: nTaps/j, tau/j. Tau=0 creates a
   * square pulse of length nTaps.
   **/
  void setPredictorsAsTraces(int nTaps=10,  // number of taps
			     float tau=0,   // tau
			     int normalised=0);
  
 public:
  /**
   * Only changes (drivatives) drive the predictive inputs
   **/
  void detectOnlyChanges(int changes=1) {onlyChange=changes;};

  ////////////////////////////////////////////////////////////////////

 private:
  /**
   * Input-field
   **/
  float* inputs;

 private:
  /**
   * Last input value
   **/
  float* lastInputs;

 private:
  /**
   * Docu of the weights
   **/
  FILE* fWeights;

 private:
  /**
   * Docu of the weights
   **/
  FILE* fProd;

 private:
  /**
   * Docu of the filter responses
   **/
  FILE* fFilter;

 private:
  /**
   * Docu of the output
   **/
  FILE* fOutput;

 private:
  /**
   * Docu if the weighted filter responses
   **/
  FILE* fweightFilter;

 private:
  /**
   * The learning rate
   **/
  float learningRate;

 private:
  /**
   * The actual output-activity of the predicting neuron
   **/
  float actualActivity;

 private:
  /**
   * The activity one timestep before
   **/
  float lastActivity;

 private:
  /**
   * The difference between the activities
   **/
  float delta;

 private:
  /**
   * Number of Filter-filters per channel
   **/
  int nFilters;

 private:
  /**
   * Number of input-channels (comes from the Inputs-class)
   **/
  int nChannels;

 private:
  /**
   * An array of traces
   **/
  Trace*** iir;

 public:
  /**
   * gets a pointer to an Filter-Filter
   **/
  Trace* getFilter(int channel, int index);

 private:
  /**
   * The weight-matrix
   **/
  float** weight;

  /**
   * last correlation. Needed for a proper integration.
   **/
  float** lastCorrel;

  /**
   * The third factor
   **/
  float thirdFactor;

  /**
   * Decay term
   **/
  float decay;

 private:
  /**
   * log file
   **/
  FILE* fLog;

 private:
  /**
   * The way how the frequencies are calculated
   * f=f0/(i+1)
   **/
  float calcFrequ(float f0,int n);

 private:
  int doDocu;

 private:
  int onlyChange;

};




#endif






