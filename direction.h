#ifndef _Direction
#define _Direction

#include "bandp.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Implements a direction circuit.
 **/

class Direction 
{
 public:
  //construct
  Direction();

 public:
  //destruct
  ~Direction();

 public:
  //filters the left and right input for the motor direction

  void doDirection(float left1,float right1,float sw); 

 public:
  //document direction circuit.

  void writeDocu(int step);


public:
  /**
   * The output of the direction circuit
   **/
  float getOutput() 
  {
    return Motorout;
  };
	
private:
  /**
   * Docu of the signals and output
   **/
  FILE* f;	

 private:
  /**
   * The actual motor output direction circuit
   **/
  float Motorout;
 
 private:
  // the step time.
  long int step;

 private:
	float dirleft1; 
 private:
	float dirright1;
	
 /**
  * LP for the left direction
  **/
	bandp* bandpLeft1;

 /**
  * LP for the right direction
  **/
	bandp* bandpRight1;

 private:
  int doDocu;

};

#endif
