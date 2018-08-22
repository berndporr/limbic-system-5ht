#ifndef _LIMBIC_SYSTEM
#define _LIMBIC_SYSTEM

#include <unistd.h>

#include <filter.h>

#define LHB_BIAS 0 //0.001
#define DRN_SUPPRESSION 0 //3
#define OFC_5HTR1_OFFSET 0
#define OFC_5HTR2_OFFSET 0 //1
#define DRN_BOOST 0 //1

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
	
	enum ExploreStates {EXPLORE_STRAIGHT, EXPLORE_LEFT, EXPLORE_RIGHT, EXPLORE_STOP, EXPLORE_NUM_ITEMS};

	ExploreStates exploreState = EXPLORE_STRAIGHT;

	// output to the motor system
	// codes approach behaviour towards the light green object
	// 0 is no and 1 is max speed
	float getLGOutput() {
		return CoreLGOut;
	};
	
	// codes approach behaviour towards the dark green object
	float getDGOutput() {
		return CoreDGOut;
	};
	

	float getExploreLeft() {return mPFC2CoreExploreLeft;};

	float getExploreRight() {return mPFC2CoreExploreRight;};

private:
	// simulation step counter
	long int step;

	// shunting inhibition is implemented as:
	// neuronal_activity / ( 1 + inhibition * shunting_inhibition_factor )
	float shunting_inhibition_factor = 200;

	//////////////////////////////////////////////////////////////
	// Nacc core
	
	// I see the light green object and I approach the light
	// green one
	float core_weight_lg2lg = 0.2;

	// I see the dark green object and I approach the dark green one
	float core_weight_dg2dg = 0.2;

private:
	// motor activities
	float CoreLGOut = 0;
	float CoreDGOut = 0;
	float mPFC2CoreExploreLeft = 0;
	float mPFC2CoreExploreRight = 0;

public:
	// learning rate of the core
	const float learning_rate_core = 0.075;

	float core_DA = 0;
	float core_plasticity = 0;

	int coreExploreCtr = 0;

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
	float VTA_baseline_activity = 0.10;
	// actual baseline for LTD/LTP
	float VTA_zero_val = VTA_baseline_activity/1.99;

	////////////////////////////////////////////////////////////////
	// RMTg
	float RMTg = 0;

	///////////////////////////////////////////////////////////////
	// mPFC
	SecondOrderLowpassFilter* visual_direction_LG_mPFC_filter;
	SecondOrderLowpassFilter* visual_direction_DG_mPFC_filter;

	float visual_direction_LG_trace = 0;
	float visual_direction_DG_trace = 0;
	
	float mPFC_LG = 0;
	float mPFC_DG = 0;
	
	float mPFC_receptor_5HT1 = 0;
	float mPFC_receptor_5HT2 = 0;

	// counters which are triggered at random moment and generate a bias then
	int mPFCspontLG = 0;
	int mPFCspontDG = 0;

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

	///////////////////////////////////////////////////////////////
	// DRN
	float DRN = 0;

	///////////////////////////////////////////////////////////////
	// OFC
	float OFC = 0;
	float OFCprev = 0;
	int OFCpersist = 0;

	// learning rate for the OFC, just now from HC to OFC
	const float learning_rate_OFC = 0.01;

	// weights from the hippocampus place fields to the OFC
	float pfLg2OFC = 0;
	float pfDg2OFC = 0;

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

	float ofc5HTreceptors(float x, float htR1, float htR2) {
		htR1 = htR1 + OFC_5HTR1_OFFSET;
		htR2 = htR2 + OFC_5HTR2_OFFSET;
		float r = (1-exp(-pow(x/htR1,htR1)))*htR2;
		if (r < 0.00001) return 0;
		return r;
	}
	
};

#endif
