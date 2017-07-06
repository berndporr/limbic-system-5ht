#include <QTimer>
#include <QTimerEvent>
#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <QLayout> 
#include <QPixmap>


#include "world.h"
#include "robot.h"
#include "worldpoint.h"
#include "defs.h"
#include "reversal-learning-5ht.h"


#define BORDERS 1



SozioBots::SozioBots( QWidget *parent, 
		      const char *) : QWidget( parent ) {
	actualStep=0;
	maxx=MAXX;
	maxy=MAXY;
	stepsBelow=0;
	fX0=NULL;
	foodDelay=FOOD_DELAY;
	numOfFoodContactsDuringReversal=0;
	numOfFoodContactsFromReversal=0;
	isReversal=0;

	
	// Creates the world
	world=new World(maxx,maxy);
	world->setContactsFilename("contacts.dat");


	// array of robots
	robot=NULL;

	robot=new Robot*[MAXROBOT];
	for(int i=0; i<MAXROBOT;i++) {
		robot[i]=new Robot(world,i,0,maxx/2,maxy/2-i*30,10,20);
		robot[i]->setPhi(1);
	}

	//Borders, food!

#ifdef BORDERS
	fprintf(stderr,"Borders\n");
	world->drawBorders();
#endif
	fprintf(stderr,"Food\n");

#ifdef DRAW_FOOD

	world->drawFood(0,PLACE_X1-FOOD_DIAMETER/2,
			PLACE_Y1-FOOD_DIAMETER/2,
			FOOD_DIAMETER,0,1); // real food at index 0
	world->drawFood(0,
			PLACE_X2+FOOD_DIAMETER/2,
			PLACE_Y2+FOOD_DIAMETER/2,
			FOOD_DIAMETER,1,0); // fake food at index 1
#endif	
	// place field

	fprintf(stderr,"Place field\n");
	
	int index = 0;
	
	world->drawPlacefield(0,PLACE_X1,PLACE_Y1,PLACEFIELD_DIAMETER,index);
	index++;
	world->drawPlacefield(0,PLACE_X2,PLACE_Y2,PLACEFIELD_DIAMETER,index);
	index++;

   
	startTimer( 0 );
}


SozioBots::~SozioBots() {
	if (fX0) {
		fclose(fX0);
	}
	for(int i=0;i<MAXROBOT;i++) {
		delete robot[i];
	}
	delete robot;

}



//
// Handles paint events for the SozioBots widget.
//

void SozioBots::paintEvent( QPaintEvent * ) {
	// will paint in this object
	int w = width();
	int h = height();

	QPixmap pm( w, h );

	QPainter *paint = new QPainter( &pm );

	if(w <= 0 || h <= 0) 
		return;

	world->fillPainter(*paint);

	delete paint;

	QPainter paint2(this);
	paint2.drawPixmap(0,0,pm);
}

//
// Handles timer events for the SozioBots widget.
//

void SozioBots::timerEvent( QTimerEvent * )
{
	for ( int i=0; i<ITERATIONS_BEFORE_DISPLAY; i++ ) {
		doSimStep();
	}
	repaint();
}


void SozioBots::doSimStep() {

	if (actualStep>PARTIAL_MOVE_BELOW_STEP) { // 
		for(int j=0;j<ROBOTS_WHICH_MOVE;j++) {
			robot[j]->move(actualStep);
			robot[j]->docCoord(actualStep);
		}
	} else { // all robots one step
		for(int j=0;j<MAXROBOT;j++) {
			robot[j]->move(actualStep);
			robot[j]->docCoord(actualStep);
		}
	}
	//plot new food...

#ifdef DRAW_FOOD
	
	if (world->getNewFood()){
		if (foodDelay>0) {
			foodDelay--;
		} else {
			// fprintf(stderr,"step #%d: drawing new food\n",actualStep);
			int newIndex = world->getFreeFoodIndex();
			
			int cx1 = PLACE_X1;
			int cy1 = PLACE_Y1;
			
			int cx2 = PLACE_X2;
			int cy2 = PLACE_Y2;
			int dx,dy;
			int sub = PLACEFIELD_DIAMETER/2;
			
			// let's find out if there's still real food out there
			// this is the index number of the food bit which is actually rewarding
			int r=world->getRewardIndex();
			// get info if that has been eaten
			int reward_exists=world->getFoodValid(r);
			if (!(world->getSwapFlag())) {
				// real food is on the left
				if (isReversal) {
					fprintf(stderr,"Final reversal contacts: %d\n",
						numOfFoodContactsDuringReversal);
					close();
				}
			} else {
				// real food is on the right (reversal)
				if (!isReversal) {
					// first step
					isReversal=1;
					numOfFoodContactsFromReversal=
						world->getNumberOfFoodContacts();
					fprintf(stderr,"########### REVERSAL STARTED ###########\n");
				}
				numOfFoodContactsDuringReversal=
					world->getNumberOfFoodContacts()-
					numOfFoodContactsFromReversal;
				fprintf(stderr,"Reversal contacts so far: %d\n",
					numOfFoodContactsDuringReversal);
			}


			    
			if (reward_exists) {
				// reward is still out there
				// we have to place fake food
				if (world->getSwapFlag()) {
					// we place the fake food on the left
					dx=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					dy=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					world->drawFood(actualStep, 
							cx1+dx-sub, 
							cy1+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							0);
				} else {
					// we plae the fake food on the right
					dx=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					dy=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					world->drawFood(actualStep, 
							cx2+dx-sub, 
							cy2+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							0);
				}
			} else {
				// no rewarding food bit out there
				// we have to generate real rewarding food
				// if ((actualStep%(REVERSAL_STEP*2))<REVERSAL_STEP) {
				if (!(world->getSwapFlag())) {
					dx=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					dy=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					world->drawFood(actualStep, 
							cx1+dx-sub, 
							cy1+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							1);
				} else {
					dx=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					dy=(int)(float)rand()% (PLACEFIELD_DIAMETER/2);
					world->drawFood(actualStep, 
							cx2+dx-sub, 
							cy2+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							1); 
				}
			}
		}
	} else {
		foodDelay=FOOD_DELAY+(rand()%FOOD_DELAY);
	}
#endif	
	
#ifdef DOC_PGM
	char pgmName[256];
	sprintf(pgmName,"temp/img%09ld.pgm",actualStep);
	world->docPgm(actualStep,pgmName);
#endif

#ifdef DOC_QUICKTIME
	if (actualStep>50000) {
	int relPos=actualStep%DOC_EVERY_N_FRAMES;
	if (relPos<DOC_FOR_N_FRAMES) {
		if (relPos==0) {
			char tmp[]="temp/mov.tmp";
			fprintf(stderr,
				"Start writing: %s\n",
				tmp);
			world->openQuicktime(tmp,"soundList.txt");
		}
		world->docQuicktime(actualStep,robot[0]->getSensors());
		if (relPos==(DOC_FOR_N_FRAMES-1)) {
			world->closeQuicktime();
			char movName[256];
			sprintf(movName,
				"temp/all%03d.mov",
				(int)(actualStep/DOC_EVERY_N_FRAMES));
			fprintf(stderr,"Writing finished...");
			fprintf(stderr,"Rename mov.tmp to %s\n",
				movName);
			if (rename("temp/mov.tmp",movName)<0) {
				fprintf(stderr,"Rename failed!\n");
			}
		}
	}
}
#endif
        actualStep++;
	if (actualStep==MAXSTEP) {
		close();
	}
}



void single_food_run(int argc, char **argv,float mu) {
	QApplication a( argc, argv );
	SozioBots* soziobots=new SozioBots();	// create widget
	soziobots->robot[0]->setMuFood(mu);
     	//soziobots->robot[0]->setPhi(0.1);
	char tmp[128];
	sprintf(tmp,"temp/X0_%09d.dat",(int)ceil(mu*1000000000.0));
	// soziobots->setX0filename(tmp,mu);
	soziobots->resize( MAXX, MAXY );	// start up with size 400x250
       	//soziobots->setCaption(tmp);
#ifdef SHOW_SIM
	soziobots->show();				// show widget
#endif
	fprintf(stderr,"mu=%e\n",mu);
	a.exec();				// run event loop
}






void statistics_food_run(int argc, char **argv,float mu) {
	FILE* f=fopen("perf.dat","wt");
	QApplication* a=NULL;
	SozioBots* soziobots=NULL;
	a=new QApplication( argc, argv ); // create application object

	// loop through different learning rates. This hasn't been tested.
       	for(float phi=0.0001;phi<2*M_PI;phi=phi+M_PI/50) {
		fprintf(stderr,"phi=%e\n",phi);
		soziobots=new SozioBots();	// create widget
		if (!soziobots) {
			fprintf(stderr,"Cound not create soziobots class\n");
			exit(1);
		}
		soziobots->robot[0]->setPhi(phi);
		soziobots->robot[0]->setMuFood(mu);
		char tmp[128];
		sprintf(tmp,"temp/X0_%09d.dat",(int)ceil(phi*1000000000.0));
		soziobots->setX0filename(tmp,phi);
		soziobots->resize( MAXX, MAXY );	// start up with size 400x250
		sprintf(tmp,"reversal learning benchmark: phi=%e",phi);
		// soziobots->setCaption(tmp);
#ifdef SHOW_SIM
		soziobots->show();				// show widget
#endif
		a->exec();				// run event loop
		fprintf(f,"%e %d\n",phi,soziobots->numOfFoodContactsFromReversal);
		fflush(f);
		srandom((unsigned int)(soziobots->actualStep%65536));
		delete soziobots;
	}
	delete a;
	fclose(f);
}






int main( int argc, char **argv ) {
	float mu=FOOD_DEFAULT_LEARNING_RATE;

        int c;

        while (-1 != (c = getopt(argc, argv, "abr:"))) {
                switch (c) {
                case 'r':
                        mu = atof(optarg);
                        break;
                case 'a':
			single_food_run(argc,argv,mu);
                        return 0;
                case 'b':
			statistics_food_run(argc,argv,mu);
                        return 0;
		case 'h':
			fprintf(stderr,"%s: Please choose one of the runs -a,-b,-c, ...\n",argv[0]);
			fprintf(stderr," -r <learning rate>\n");
			fprintf(stderr," -a: single food run\n");
			fprintf(stderr," -b: statistics\n");
                default:
                        exit(1);
                }
        }
	// default behaviour without any valid arguments
	single_food_run(argc,argv,mu);
	return 0;	
}




void SozioBots::setX0filename(char* tmp,float mu) {
	fX0=fopen(tmp,"rt");
#ifdef CHECK_FOR_X0_FILE
	if (fX0) {
		fclose(fX0);
		fprintf(stderr,"filename %s exists!\n",tmp);
		exit(1);
	}
#endif
	fX0=fopen(tmp,"wt");
	if (!fX0) {
		fprintf(stderr,"Could not open x0.dat\n");
		exit(1);
	}
	fprintf(fX0,"# mu=%e\n",mu);
}


