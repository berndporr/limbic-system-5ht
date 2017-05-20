#ifndef SOZIOBOTS_H
#define SOZIOBOTS_H

#include <qwidget.h>
#include "world.h"
#include "robot.h"
#include <stdio.h>


class SozioBots : public QWidget
{
    Q_OBJECT

 public:
    SozioBots( QWidget *parent=0, 
	       const char *name=0);

 public:
    ~SozioBots();


 protected:
    void	paintEvent( QPaintEvent * );
    
 protected:
    void	timerEvent( QTimerEvent * );

 public:
    long int actualStep;

 private:
    void doSimStep();
    
 private:
    int argc;
    char** argv;

    // max x coordinate
    int maxx;

    // max y coordinate
    int maxy;

    int foodDelay;

    int isReversal;
    
    int numOfFoodContactsDuringReversal;

 public:
    int numOfFoodContactsFromReversal;

 public:
    // Creates the world
    World* world;
    
    // array of robots
    Robot** robot;

    // the X0 file
    FILE* fX0;

    int stepsBelow;

    void setX0filename(char* tmp,float mu);
};


#endif
