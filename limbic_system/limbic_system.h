#ifndef _LIMBIC_SYSTEM
#define _LIMBIC_SYSTEM

#include <unistd.h>

#include <filter.h>


class Limbic_system {
public:
	// constructor
	Limbic_system();
	
	// destructor
	~Limbic_system();
	
	// this feeds into the limbic system:
	// we have a place field around the light green (LG) and
	// dark green (DG) objects. There is a reflex input on contact
	// and a distance signal from the "eyes" of the agent seeing the
	// light or dark green object.
	void doStep(float reward,
		    float placefield_LG,
		    float placefield_DG,
		    float on_contact_direction_LG,
		    float on_contact_direction_DG,
		    float visual_direction_LG,
		    float visual_direction_DG
		);
	
	// output to the motor system
	// codes approach behaviour towards the light green object
	// 0 is no and 1 is max speed
	float getLGOutput() {return CoreLGOut;};
	
	// codes approach behaviour towards the dark green object
	float getDGOutput() {return CoreDGOut;};

private:
	// motor activities
	float CoreLGOut;
	float CoreDGOut;

	// simulation step counter
	long int step;

	// shunting inhibition is implemented as:
	// neuronal_activity / ( 1 + inhibition * shunting_inhibition_factor )
	float shunting_inhibition_factor = 100;

	//////////////////////////////////////////////////////////////
	// weights for the motor system
	
	// I see the light green object and I appoach the light
	// green one
	float core_weight_lg2lg = 0;

	// I see the light green object but I approach the dark green one!
	float core_weight_lg2dg = 0;

	// I see the dark green object and I approach the dark green one
	float core_weight_dg2dg = 0;

	// I see the dark green object and I approach the light green one
	float core_weight_dg2lg = 0;

	// learning rate of the core
	const float learning_rate_core = 0.05;

	////////////////////////////////////////////////////////////////
	// l-shell

	// weights for the shell system
	float lShell_weight_pflg = 0;
	float lShell_weight_pfdg = 0;

	const float learning_rate_lshell = 0.001;


	////////////////////////////////////////////////////////////////
	// VTA parameters
	float VTA_baseline_activity = 0.2;

	////////////////////////////////////////////////////////////////
	// RMTg
	float RMTg = 0;

	///////////////////////////////////////////////////////////////
	// mPFC
	SecondOrderLowpassFilter* visual_direction_LG_mPFC_filter;
	SecondOrderLowpassFilter* visual_direction_DG_mPFC_filter;

private:
	// changes the weight w by the amount delta
	// hard limits at -1 and +1
	void weightChange(float &w, float delta);

	// smoothes the signal when touching the object and
	// creates a curiosity reaction
	SecondOrderLowpassFilter* on_contact_direction_LG_filter;
	SecondOrderLowpassFilter* on_contact_direction_DG_filter;

};

#endif
