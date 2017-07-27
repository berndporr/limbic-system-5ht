#include"limbic_system.h"

#include <stdio.h>

#include <filter.h>

Limbic_system::Limbic_system()
{
	//The direction triggers
	CoreLGOut=0.0;
	CoreDGOut=0.0;

	on_contact_direction_LG_filter = new SecondOrderLowpassFilter(0.1);
	on_contact_direction_DG_filter = new SecondOrderLowpassFilter(0.1);

	step = 0;
};

void Limbic_system::weightChange(float &w, float delta) {
	w += delta;
	if (w>1) w = 1;
	if (w<0) w = 0;
}


void Limbic_system::doStep(float reward,
		float placefieldLG,
		float placefieldDG,
		float on_contact_direction_LG,
		float on_contact_direction_DG,
		float visual_direction_LG,
		float visual_direction_DG) {


	// the activity in the LH is literally that of the reward
	float LH = reward;

	float VTA = (LH + VTA_baseline_activity) / (1+RMTg * 10) ;

	// we have two core units
	// if the LG is high then the rat approaches the LG marker
	CoreLGOut= visual_direction_LG * core_weight_lg2lg + visual_direction_DG * core_weight_dg2lg + on_contact_direction_LG_filter->filter(on_contact_direction_LG);
	// of the DG is high then the rat approaches the DG marker
	CoreDGOut= visual_direction_LG * core_weight_lg2dg + visual_direction_DG * core_weight_dg2dg + on_contact_direction_DG_filter->filter(on_contact_direction_DG);

	float core_DA = VTA;
	float core_plasticity = core_DA - VTA_baseline_activity;
	weightChange(core_weight_lg2lg, learning_rate_core * core_plasticity * visual_direction_LG * CoreLGOut);
	weightChange(core_weight_lg2dg, learning_rate_core * core_plasticity * visual_direction_LG * CoreDGOut);
	weightChange(core_weight_dg2lg, learning_rate_core * core_plasticity * visual_direction_DG * CoreLGOut);
	weightChange(core_weight_dg2dg, learning_rate_core * core_plasticity * visual_direction_DG * CoreDGOut);

#ifdef CORE_OUTPUT
	printf("%f %f %f %f %f %f %f %f %f %f %f %f %f\n",reward,
	       placefieldLG,
	       placefieldDG,
	       on_contact_direction_LG,
	       on_contact_direction_DG,
	       visual_direction_LG,
	       visual_direction_DG,
	       core_weight_lg2lg,
	       core_weight_lg2dg,
	       core_weight_dg2lg,
	       core_weight_dg2dg,
	       CoreLGOut,
	       CoreDGOut
		);
#endif


	// the place field feeds into the Nacc shell for the time being. TODO: Cortex
	float lShell = placefieldLG * lShell_weight_pflg + placefieldDG * lShell_weight_pfdg;

	// Let's do heterosynaptic plasticity
	float shell_DA = VTA;
	float shell_plasticity = shell_DA - VTA_baseline_activity;
	if (shell_plasticity < 0) shell_plasticity = 0;
	weightChange(lShell_weight_pflg, learning_rate_lshell * shell_plasticity * placefieldLG);
	weightChange(lShell_weight_pfdg, learning_rate_lshell * shell_plasticity * placefieldDG);

	// the shell inhibits the
	float dlVP = 1/(1+lShell * shunting_inhibition_factor);

	// another inhibition
	float EP = 1/(1+dlVP * shunting_inhibition_factor);

	float LHb = EP;

	printf("%f %f %f\n",VTA,core_weight_lg2lg,lShell_weight_pflg);

	RMTg = LHb;

	//

	step++;  
			
}	
                                      
		                                            



Limbic_system::~Limbic_system() 
{
}
