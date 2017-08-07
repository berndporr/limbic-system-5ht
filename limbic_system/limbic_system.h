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
	void doStep(float _reward,
		    float _placefield_LG,
		    float _placefield_DG,
		    float _on_contact_direction_LG,
		    float _on_contact_direction_DG,
		    float _visual_direction_LG,
		    float _visual_direction_DG,
			float _visual_reward_LG,
			float _visual_reward_DG
		);
	
	// output to the motor system
	// codes approach behaviour towards the light green object
	// 0 is no and 1 is max speed
	float getLGOutput() {return CoreLGOut;};
	
	// codes approach behaviour towards the dark green object
	float getDGOutput() {return CoreDGOut;};

private:
	// simulation step counter
	long int step;

	// shunting inhibition is implemented as:
	// neuronal_activity / ( 1 + inhibition * shunting_inhibition_factor )
	float shunting_inhibition_factor = 100;

	//////////////////////////////////////////////////////////////
	// Nacc core
	
	// I see the light green object and I approach the light
	// green one
	float core_weight_lg2lg = 0;

	// I see the light green object but I approach the dark green one!
	float core_weight_lg2dg = 0;

	// I see the dark green object and I approach the dark green one
	float core_weight_dg2dg = 0;

	// I see the dark green object and I approach the light green one
	float core_weight_dg2lg = 0;

	// motor activities
	float CoreLGOut = 0;
	float CoreDGOut = 0;

	// learning rate of the core
	const float learning_rate_core = 0.05;

	float core_DA = 0;
	float core_plasticity = 0;

	////////////////////////////////////////////////////////////////
	// l-shell
	// l-shall activity
	float lShell = 0;

	// dopamine activity
	float shell_DA = 0;
	float shell_plasticity = 0;

	// weights for the shell system
	float lShell_weight_pflg = 0;
	float lShell_weight_pfdg = 0;

	// learning rate in the shell
	const float learning_rate_lshell = 0.001;


	////////////////////////////////////////////////////////////////
	// VTA parameters
	float VTA_baseline_activity = 0.2;

	////////////////////////////////////////////////////////////////
	// RMTg
	float RMTg = 0;

	///////////////////////////////////////////////////////////////
	// mPFC
	float mPFC_LG = 0;
	float mPFC_DG = 0;

	SecondOrderLowpassFilter* visual_direction_LG_mPFC_filter;
	SecondOrderLowpassFilter* visual_direction_DG_mPFC_filter;

	/////////////////////////////////////////////////////////////////
	// VTA
	float VTA = 0;

	/////////////////////////////////////////////////////////////////
	// Lateral Hypotyalamus
	float LH = 0;

	////////////////////////////////////////////////////////////////
	// dorsolateral ventral pallidum
	float dlVP = 0;

	///////////////////////////////////////////////////////////////
	// EP
	float EP=0;

	////////////////////////////////////////////////////////////////
	// Lateral habenula
	float LHb = 0;


private:
	// changes the weight w by the amount delta
	// hard limits at -1 and +1
	void weightChange(float &w, float delta);

	// smoothes the signal when touching the object and
	// creates a curiosity reaction
	SecondOrderLowpassFilter* on_contact_direction_LG_filter;
	SecondOrderLowpassFilter* on_contact_direction_DG_filter;

	// copies of the input signals
	float reward = 0;
	float placefieldLG = 0;
	float placefieldDG = 0;
	float on_contact_direction_LG = 0;
	float on_contact_direction_DG = 0;
	float visual_direction_LG = 0;
	float visual_direction_DG = 0;
	float visual_reward_LG = 0;
	float visual_reward_DG = 0;

	FILE* flog = NULL;
	void logging();

};

#endif
