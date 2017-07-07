#ifndef _sozio_defs_h
#define _sozio_defs_h
#include <quicktime.h>

#define DAMPING 50

#define FOOD_DEFAULT_LEARNING_RATE 2e-04

/**
 * Experiment
 **/
#define EXPERIMENT 100

/**
 * Number of input channels for the learner
 **/
#define MAXFEATURES 3

/**
 * Number of iterations of the simulation before its displayed
 **/
#define ITERATIONS_BEFORE_DISPLAY 20

/**
 * Move only some robots below the given step number
 **/
#define PARTIAL_MOVE_BELOW_STEP 1

/**
 * Max Number of Robots
 **/
#define MAXROBOT 1

/**
 * Number of robots which should be moved
 **/
#define ROBOTS_WHICH_MOVE 1

/**
 * X-playground
 **/
#define MAXX 600

/**
 * Y-playground
 **/
#define MAXY 400

/**
 * Starting angle
 **/
#define START_ANGLE +0.2

/**
 * ROBOT_SPEED of the robot
 **/
#define ROBOT_SPEED 0.750

// threshold to switch on a behaviour
#define MOTOR_THRES 0

/**
 * create PGMs
 **/
// #define DOC_PGM

/**
 * generate a QT film?
 **/
// #define DOC_QUICKTIME

/**
 * Frame-rate for the quicktime-film
 **/
#define FRAME_RATE 15

#define QT_CODEC QUICKTIME_PNG


/**
 * Doc only in chunks
 **/
#define DOC_EVERY_N_FRAMES  1000
#define DOC_FOR_N_FRAMES    1000



/**
 * shows the sim in a window
 **/
#define SHOW_SIM 1


/**
 * stops if an x0 file is there
 **/
// #define CHECK_FOR_X0_FILE 1



//////////////////////////////////////////////////////////////////////
// BUMP

/**
 * revese ROBOT_SPEED to let the robot turn
 **/
#define BUMP_REVERSE_GAIN 0.15

/**
 * gain of the steering wheel when the robot turns
 **/
#define BUMP_STEER_GAIN -0.005


#define THREE_FACTOR_RULE 1

/**
 * Learning rate for the bump: dphi
 **/
#define MU_PHI_BUMP 0.00000


/**
 * Learning rate for the bump: ds
 **/
#define MU_DS_BUMP  0.00000

  /**
   * Frequency for the standard reaction of the robot
   **/
#define BUMP_F_REFLEX 0.01

/**
 * Quality for the standard recation fo the robot
 **/
#define BUMP_Q_REFLEX 0.6

/**
 * Number of filters for the learned behaviour
 **/
#define BUMP_N_FILTERS 10

/**
 * Highest frequency for the learned reaction of the robot
 **/
#define BUMP_F_LEARN 0.1

/**
 * Factor for the development of the frequencies
 **/
#define BUMP_LEARN_FACT 1

/**
 * Q for the learned reaction of the robot
 **/
#define BUMP_Q_LEARN 1.0

/**
 * RF_DEPTH of the light sensors
 **/
#define RF_DEPTH 50
//#define RF_DEPTH 100

/**
 * time
 **/
#define STICK_LIMIT 100


//////////////////////////////////////////////////////////////////////
// FOOD

// the rest of the parameters are defined in "main" of soziobots

#define FOOD_DIAMETER 20

/**
 * Number of FOOD bits
 **/
#define MAXFOOD 2

/**
 * No new food after step no
 **/
#define NO_NEW_FOOD_AFTER 900000

/**
 * Draw the food
 **/
#define DRAW_FOOD 

/**
 * The gain for the food
 **/
#define FOOD_GAIN 0.001

/**
 * Lifetime of the food
 **/
#define MAX_FOOD_LIFE 100

/**
 * number of the filters
 **/
#define N_FILTERS_FOOD 5

// number of steps before a new food blob is placed
#define FOOD_DELAY 250

///////////////////////////////////////////////////////////////////////
// PLACE FIELDS

// number of place fields

#define MAXPLACEFIELD 2

#define PLACEFIELD_DIAMETER 50

#define PLACE_X1 450
#define PLACE_X2 450
#define PLACE_Y1 100
#define PLACE_Y2 300

///////////////////////////////////////////////////////////////////////
// Statistics
#define MAXSTEP 500000
#define N_BELOW 3
#define THRESHOLD_X0 0.2

///////////////////////////////////////////////////////////////////////
// Reversal learning
#define REVERSAL_STEP 100000

#define THRES_FOOD_CONTACTS 10



#endif

