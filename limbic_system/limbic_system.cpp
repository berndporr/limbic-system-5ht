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


// changes a synaptic weight by the amount "delta"
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
// - two place field signals (placefieldLG/DG) which go from 0 to 1 when in the place field
// - two signals when the agent touches the landmark: on_contact_direction_LG/DG
// - visual inputs when the agent sees a landmark which goes from 0 to 1 the closer the agent gets
//
// It needs to set the outputs:
// - CoreLGOut and CoreDGOut which when set to non-zero generates a navigation behaviour towards
//   the landmarks
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

	mPFC_LG = visual_direction_LG_mPFC_filter->filter(visual_direction_LG);
	mPFC_DG = visual_direction_LG_mPFC_filter->filter(visual_direction_DG);

	// the activity in the LH is literally that of the reward
	LH = reward;

	VTA = (LH + VTA_baseline_activity) / (1+RMTg * shunting_inhibition_factor);

	BLA = placefieldLG + placefieldDG;

	DRN = BLA * 10;

	//printf("%f\n",DRN);

	// we have two core units
	// if the LG is high then the rat approaches the LG marker
	CoreLGOut= (mPFC_LG * core_weight_lg2lg + mPFC_DG * core_weight_dg2lg + visual_reward_LG)/(1+DRN) + on_contact_direction_LG_filter->filter(on_contact_direction_LG);
	// of the DG is high then the rat approaches the DG marker
	CoreDGOut= (mPFC_LG * core_weight_lg2dg + mPFC_DG * core_weight_dg2dg + visual_reward_DG)/(1+DRN) + on_contact_direction_DG_filter->filter(on_contact_direction_DG);
	CoreExploreOut = 1 / (CoreLGOut+CoreDGOut+DRN+1);

	core_DA = VTA;
	core_plasticity = core_DA - VTA_baseline_activity/2;
	weightChange(core_weight_lg2lg, learning_rate_core * core_plasticity * visual_direction_LG * CoreLGOut);
	weightChange(core_weight_lg2dg, learning_rate_core * core_plasticity * visual_direction_LG * CoreDGOut);
	weightChange(core_weight_dg2lg, learning_rate_core * core_plasticity * visual_direction_DG * CoreLGOut);
	weightChange(core_weight_dg2dg, learning_rate_core * core_plasticity * visual_direction_DG * CoreDGOut);


	// the place field feeds into the Nacc shell for the time being.
	lShell = placefieldLG * lShell_weight_pflg + placefieldDG * lShell_weight_pfdg;

	// Let's do heterosynaptic plasticity
	shell_DA = VTA;
	shell_plasticity = shell_DA - VTA_baseline_activity/2;
	if (shell_plasticity < 0) shell_plasticity = 0;

	weightChange(lShell_weight_pflg, learning_rate_lshell * shell_plasticity * placefieldLG);
	weightChange(lShell_weight_pfdg, learning_rate_lshell * shell_plasticity * placefieldDG);

	// the shell inhibits the
	dlVP = 1/(1+lShell * shunting_inhibition_factor);

	// another inhibition
	EP = 1/(1+dlVP * shunting_inhibition_factor);

	LHb = EP;

	RMTg = LHb;

	logging();

	step++;  

}	




void Limbic_system::logging() {
	fprintf(flog,"%ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			step, //0
			reward, //1
			placefieldLG, //2
			placefieldDG, //3
			on_contact_direction_LG, //4
			on_contact_direction_DG, //5
			visual_direction_LG, //6
			visual_direction_DG, //7
			core_weight_lg2lg, //8
			core_weight_lg2dg, //9
			core_weight_dg2lg, //10
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
			RMTg//22
	);
}

