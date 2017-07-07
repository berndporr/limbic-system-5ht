#include"limbic_system.h"

#include <stdio.h>


Limbic_system::Limbic_system()
{
	//The direction triggers
	CoreLGOut=0.0;
	CoreDGOut=0.0;

	step = 0;
};

void Limbic_system::setLearningRate(float)
{
}

void Limbic_system::doStep(float reward,
		float placefield1,
		float placefield2,
		float on_contact_direction_LG,
		float on_contact_direction_DG,
		float visual_direction_LG,
		float visual_direction_DG) {


	printf("rewards=%f, placefield1=%f, placefield2=%f, con_reflex_LG=%f, con_reflex_DG=%f, vis_LG=%f, vis_DG=%f\n",reward,
		placefield1,
		placefield2,
		on_contact_direction_LG,
		on_contact_direction_DG,
		visual_direction_LG,
		visual_direction_DG);


	// the activity in the LH is literally that of the reward
	float LH = reward;



	// we have two core units
	// if the LG is high then the rat approaches the LG marker
	CoreLGOut= 0;
	// of the DG is high then the rat approaches the DG marker
	CoreDGOut= 0;

	/**
	// projection from the NAcc to the VTA which proj back to the shell
	double vp_sh=(1+CB_DISINH)/(1+nacc*NACC_VP);
	double netInhib_sh=(d2_auto_sh*DFACT)+(vp_sh*VP_VTA+nacc*NACC_VTA);

	
	if (netInhib_sh<0) netInhib_sh=0;
	double vta_sh=(lh+BASELINE_VTA)/(1+netInhib_sh);
        double derivVTA_sh=vta_sh-lastVTA_sh;
        lastVTA_sh=vta_sh;
        double thirdFactor_sh=0;
	if (derivVTA_sh>0.2){
		thirdFactor_sh=derivVTA_sh*LTP_GAIN;
		d2_auto_sh=0;
		d2_receptor_sh=0;
	} else {
		da_reup_sh=(double)vta_sh-0.5;
	}
	if (da_reup_sh<0) {
		da_reup_sh=0;
	}
	// ignore the level of the vta during bursting
	d2_auto_sh=d2_auto_sh+(da_reup_sh-d2_auto_sh)*((double)TAU_D2);
	d2_receptor_sh=d2_receptor_sh+(da_reup_sh-d2_receptor_sh)*((double)TAU_D2);
	//default baseline
	float ltd_sh=d2_receptor_sh;
	if (ltd_sh>0) {
	  ltd_sh=ltd_sh*LTD_GAIN;
	}

**/


	step++;  
			
}	
                                      
		                                            



Limbic_system::~Limbic_system() 
{
}
