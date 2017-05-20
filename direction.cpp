  /*****************************************************************
        THE DIRECTION 
  ******************************************************************/
#include"direction.h"

//This is the treshold for switching direction
#define TRESHOLD 4

Direction::Direction() 
{
  char tmp[]="direction.dat";
  f=fopen(tmp,"wt");
  if (!f) 
    {
      fprintf(stderr,"Could not open %s\n",tmp);
      exit(1);
    }
  step=0; 
  dirleft1=0.0;
  dirright1=0.0;
  doDocu=0;
  bandpLeft1=new bandp();
  bandpLeft1->calcCoeffBandp(0.009,0.51);
  bandpRight1=new bandp();
  bandpRight1->calcCoeffBandp(0.009,0.51);
}


void Direction::doDirection(float l1,float r1,float sw) {
	if (sw<TRESHOLD) sw=0;
	dirleft1=bandpLeft1->filter(l1);
	dirright1=bandpRight1->filter(r1);
	Motorout=(dirleft1-dirright1)*0.1*sw;   
}
/**
 * Write gnuplot friendly output
 **/
void Direction::writeDocu(int step) 
{
	if (!doDocu) return;

	fprintf(f,"%d %f %f %f\n",step,Motorout,dirleft1,dirright1);
	step++;

}
	
Direction::~Direction()
 {
	if (doDocu) 
	  {
	    fclose(f);
	  }
	if (f) fclose(f);
	delete bandpLeft1;
	delete bandpRight1;
 }
