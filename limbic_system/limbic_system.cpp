#include"limbic_system.h"

#include <stdio.h>

#include <filter.h>

// constructor
Limbic_system::Limbic_system()
{
	flog = fopen("log.dat","wt");

	// filter which creates a default behaviour when touching the landmarks
	on_contact_direction_LG_filter = new SecondOrderLowpassFilter(0.1);
	on_contact_direction_DG_filter = new SecondOrderLowpassFilter(0.1);

	// filter which smears out the visual input in time as a 1st attempt
	// to create persistent activity in the cortex
	visual_direction_LG_mPFC_filter = new SecondOrderLowpassFilter(0.01);
	visual_direction_DG_mPFC_filter = new SecondOrderLowpassFilter(0.01);

	// input step number
	step = 0;
};


// destructor
Limbic_system::~Limbic_system()
{
	fclose(flog);
}


// changes a synaptic weight "w" by the amount "delta"
// and makes sure it never goes below 0 and above 1
void Limbic_system::weightChange(float &w, float delta) {
	w += delta;
	if (w>1) w = 1;
	if (w<0) w = 0;
}

// that's what our limbic system does!
// remember there are two landmarks/place fields: light green and dark green
// only one of them contains the reward (controlled by the main program)
//
// It gets as inputs:
// - the reward
// - two place field signals (placefield*) which go from 0 to 1 when in the place field (where the reward shows up)
// - two signals when the agent touches the landmark: (on_contact_direction_*)
// - visual inputs when the agent sees a landmark which goes from 0 to 1 the closer the agent gets (_visual_direction_*)
// - visual inputs when the agent sees the reward (_visual_reward_*)
//
// It needs to set the outputs:
// - CoreLGOut and CoreDGOut which when set to non-zero generates a navigation behaviour towards
//   the landmarks
// - mPFC2CoreExploreLeft and - mPFC2CoreExploreRight to generate exploration behaviour
//   that is inhibited with other activities.
void Limbic_system::doStep(float _reward,
		float _placefieldLG,
		float _placefieldDG,
		float _on_contact_direction_LG,
		float _on_contact_direction_DG,
		float _visual_direction_LG,
		float _visual_direction_DG,
		float _visual_reward_LG,
		float _visual_reward_DG ) {

	reward = _reward;
	placefieldLG = _placefieldLG;
	placefieldDG = _placefieldDG;
	on_contact_direction_LG = _on_contact_direction_LG;
	on_contact_direction_DG = _on_contact_direction_DG;
	visual_direction_LG = _visual_direction_LG;
	visual_direction_DG = _visual_direction_DG;
	visual_reward_LG = _visual_reward_LG;
	visual_reward_DG = _visual_reward_DG;
	//fprintf(stderr,"%f,%f\n",_visual_reward_LG,_visual_reward_DG);

	visual_direction_LG_trace = visual_direction_LG_mPFC_filter->filter(visual_direction_LG);
	visual_direction_DG_trace = visual_direction_DG_mPFC_filter->filter(visual_direction_DG);

	double mPFC_LG_spont_act = 0;
	double mPFC_DG_spont_act = 0;
	double mPFC_spont_act_value = 0.2;

	if (mPFCspontLG>0) {
		mPFCspontLG--;
		mPFC_LG_spont_act = mPFC_spont_act_value;
	} else {
		mPFC_LG_spont_act = 0;
		if (random() < (RAND_MAX/1000)) {
			mPFCspontLG = 500;
		}
	}

	if (mPFCspontDG>0) {
		mPFCspontDG--;
		mPFC_DG_spont_act = mPFC_spont_act_value;
	} else {
		mPFC_DG_spont_act = 0;
		if (random() < (RAND_MAX/1000)) {
			mPFCspontDG = 500;
		}
	}

	// this is also generated in the mPFC and then fed down to the NAcc core with the command
	// to explore
	switch (exploreState) {
	case EXPLORE_LEFT:
		mPFC2CoreExploreLeft = 0.1;
		mPFC2CoreExploreRight = 0;
		if (((float)random()/((float)RAND_MAX))<0.03) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		//printf("left\n");
		break;
	case EXPLORE_RIGHT:
		mPFC2CoreExploreLeft = 0;
		mPFC2CoreExploreRight = 0.1;
		if (((float)random()/((float)RAND_MAX))<0.03) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		//printf("right\n");
		break;
	case EXPLORE_STOP:
		mPFC2CoreExploreLeft = 0;
		mPFC2CoreExploreRight = 0;
		if (((float)random()/((float)RAND_MAX))<0.01) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		break;
	case EXPLORE_STRAIGHT:
	default:
		mPFC2CoreExploreLeft = 0.1;
		mPFC2CoreExploreRight = 0.1;
		if (((float)random()/((float)RAND_MAX))<0.05) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		break;
	}

//#define HT5DEBUG
#ifdef HT5DEBUG
	mPFC_LG = visual_direction_LG_trace + visual_reward_LG + mPFC_LG_spont_act;
	mPFC_DG = visual_direction_DG_trace + visual_reward_DG + mPFC_DG_spont_act;
#else
	mPFC_LG = ofc5HTreceptors(visual_direction_LG_trace + visual_reward_LG*2 + mPFC_LG_spont_act,1+DRN,2+DRN);
	mPFC_DG = ofc5HTreceptors(visual_direction_DG_trace + visual_reward_DG*2 + mPFC_DG_spont_act,1+DRN,2+DRN);
#endif

	// the activity in the LH is literally that of the reward
	LH = reward;

	// the VTA gets its activity from the LH and is ihibited by the RMTg
	VTA = (LH + VTA_baseline_activity) / (1+RMTg * shunting_inhibition_factor);

	////////////////////////////////////////////////////////////////////
	// OFC
	// lace field -> Orbitofrontal Cortex weight.
	// The OFC remembers the reward value of a place.
	// So the higher the weight the higher the OFC activity
	// when the animal is inside that place field where there has been
	// reward experienced in the past.
	OFC = pfLg2OFC * placefieldLG + pfDg2OFC * placefieldDG;
	if (((placefieldLG-OFCprev) > 0.01)&&(OFCpersist==0)) OFCpersist = 200;
	OFCprev = placefieldLG;
	if (LH > 0.5) {
		OFCpersist = 0;
	}
	if (OFCpersist>0) OFCpersist--;
	//fprintf(stderr,"%d\n",OFCpersist);
	// weight change
	weightChange(pfLg2OFC, learning_rate_OFC * DRN * placefieldLG);
	weightChange(pfDg2OFC, learning_rate_OFC * DRN * placefieldDG);


	// massive weight decay if there is no reward after a long period!
//	if ((OFCpersist>0)&&(OFCpersist<100)) {
//		fprintf(stderr,"--");
//		pfLg2OFC = pfLg2OFC * 0.999;
//	}

	// the dorsal raphe activity is driven by the OFC in a positive way
	DRN = (LH + OFC * 4) / (1+RMTg * shunting_inhibition_factor + DRN_SUPPRESSION) * (DRN_BOOST + 1);

	//printf("%f\n",DRN);

	////////////////////////////////////////////////////
	// lateral shell activity
	// the place field feeds into the Nacc shell for the time being.
	lShell = placefieldLG * lShell_weight_pflg + placefieldDG * lShell_weight_pfdg;

	// Let's do heterosynaptic plasticity in the shell
	shell_DA = VTA;
	// shell plasticity can experience a "dip" where then the weights
	// decrease. That's when it's below its baseline.
	shell_plasticity = shell_DA - VTA_zero_val;
	weightChange(lShell_weight_pflg, learning_rate_lshell * shell_plasticity * placefieldLG);
	weightChange(lShell_weight_pfdg, learning_rate_lshell * shell_plasticity * placefieldDG);

	// the shell inhibits the dlVP
	dlVP = 1/(1+lShell * shunting_inhibition_factor);

	// another inhibition: the dlVP inhibits the EP
	EP = 1/(1+dlVP * shunting_inhibition_factor);

	// the EP excites the LHb
	LHb = EP + LHB_BIAS;

	// and the LHb excites the RMTg
	RMTg = LHb;

	////////////////////////////////////////////////////////////////////////////////////
	// core
	// we have two core units
	// if the LG is high then the rat approaches the LG marker
	CoreLGOut= (mPFC_LG * core_weight_lg2lg);
	// of the DG is high then the rat approaches the DG marker
	CoreDGOut= (mPFC_DG * core_weight_dg2dg);

	float core_threshold = 0.25;
	if (CoreLGOut < core_threshold) CoreLGOut = 0;
	if (CoreDGOut < core_threshold) CoreDGOut = 0;

	// plasticity
	core_DA = VTA;
	core_plasticity = core_DA - VTA_zero_val;
	weightChange(core_weight_lg2lg, learning_rate_core * core_plasticity * mPFC_LG);
	weightChange(core_weight_dg2dg, learning_rate_core * core_plasticity * mPFC_DG);

	// we assume that the Core performs lateral inhibtion to shut down exploration
	if ((CoreLGOut > 0.05)&&(CoreDGOut > 0.05)) {
		mPFC2CoreExploreLeft = 0; // (float)random()/(float)RAND_MAX;
		mPFC2CoreExploreRight = 0; //(float)random()/(float)RAND_MAX;
		//printf("dir! mPFC_LG = %f\n",visual_direction_LG);
	}

	logging();

	step++;  

}	




void Limbic_system::logging() {
	fprintf(flog,"%ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		step, //0
		reward, //1
		placefieldLG, //2
		placefieldDG, //3
		on_contact_direction_LG, //4
		on_contact_direction_DG, //5
		visual_direction_LG, //6
		visual_direction_DG, //7
		core_weight_lg2lg, //8
		mPFC_LG, //9
		mPFC_DG, //10
		core_weight_dg2dg, //11
		CoreLGOut, //12
		CoreDGOut, //13
		VTA,//14
		core_plasticity,//15
		shell_plasticity,//16
		lShell_weight_pflg,//17
		lShell_weight_pfdg,//18
		dlVP,//19
		EP,//20
		LHb,//21
		RMTg,//22
		pfLg2OFC,//23
		pfDg2OFC, //24
		DRN, //25
		visual_reward_LG, // 26
		visual_reward_DG //27
		);
	fflush(flog);
}

