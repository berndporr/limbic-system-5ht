#include"limbic_system.h"

#include <stdio.h>

#include <filter.h>

Limbic_system::Limbic_system()
{
	//The direction triggers
	CoreLGOut=0.0;
	CoreDGOut=0.0;

	// learning rates
	learning_rate_core = 0.05;

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
		float placefield1,
		float placefield2,
		float on_contact_direction_LG,
		float on_contact_direction_DG,
		float visual_direction_LG,
		float visual_direction_DG) {


	// the activity in the LH is literally that of the reward
	float LH = reward;

	// let's keep it simple for now: the LH drives the VTA to spike
	float VTA = LH;

	// we have two core units
	// if the LG is high then the rat approaches the LG marker
	CoreLGOut= visual_direction_LG * weight_lg2lg + visual_direction_DG * weight_dg2lg + on_contact_direction_LG_filter->filter(on_contact_direction_LG);
	// of the DG is high then the rat approaches the DG marker
	CoreDGOut= visual_direction_LG * weight_lg2dg + visual_direction_DG * weight_dg2dg + on_contact_direction_DG_filter->filter(on_contact_direction_DG);

	weightChange(weight_lg2lg, learning_rate_core * VTA * visual_direction_LG * CoreLGOut);
	weightChange(weight_lg2dg, learning_rate_core * VTA * visual_direction_LG * CoreDGOut);
	weightChange(weight_dg2lg, learning_rate_core * VTA * visual_direction_DG * CoreLGOut);
	weightChange(weight_dg2dg, learning_rate_core * VTA * visual_direction_DG * CoreDGOut);

	printf("%f %f %f %f %f %f %f %f %f %f %f %f %f\n",reward,
	       placefield1,
	       placefield2,
	       on_contact_direction_LG,
	       on_contact_direction_DG,
	       visual_direction_LG,
	       visual_direction_DG,
	       weight_lg2lg,
	       weight_lg2dg,
	       weight_dg2lg,
	       weight_dg2dg,
	       CoreLGOut,
	       CoreDGOut
		);

	step++;  
			
}	
                                      
		                                            



Limbic_system::~Limbic_system() 
{
}
