#include"limbic_system.h"

Limbic_system::Limbic_system()
{
	char tmp[]="shell.dat";
	f=fopen(tmp,"wt");
	if (!f) {
		fprintf(stderr,"Could not open %s\n",tmp);
		exit(1);
	}
	char tmpo[]="core.dat";
	fcore=fopen(tmpo,"wt");
	if (!fcore) {
		fprintf(stderr,"Could not open %s\n",tmpo);
		exit(1);
	}
	// shell
        isolearningShell=new Isolearning(3,1);
	if (!isolearningShell) {
		fprintf(stderr,"Could not allocate memory for isolearningShell\n");
		exit(1);
	}
        isolearningShell->setLearningRate(MU_SHELL);
        isolearningShell->setReflex(0.25,Q_FOOD_FIXED);
	
	
	// core

	/**this is a new architecture in the core**/

	// this is used to trigger direction in the cerebellum.
	
	//the lightgreen place field

	isolearningCoreLG=new Isolearning(3,1);

	if (!isolearningCoreLG) {
		fprintf(stderr,"Could not allocate memory for isolearningCore\n");
		exit(1);
	}
        isolearningCoreLG->setLearningRate(MU_SHELL);
        isolearningCoreLG->setReflex(F_FOOD_FIXED,Q_FOOD_FIXED);
	
	//the Darkgreen place field
	isolearningCoreDG=new Isolearning(3,1);
	
	if (!isolearningCoreDG) {
		fprintf(stderr,"Could not allocate memory for isolearningCore\n");
		exit(1);
	}
        isolearningCoreDG->setLearningRate(MU_SHELL);
        isolearningCoreDG->setReflex(F_FOOD_FIXED,Q_FOOD_FIXED);

	/**Core architecture ends **/

	//traces

        if (TRACES) {
                isolearningShell->setPredictorsAsTraces(50);
		isolearningCoreLG->setPredictorsAsTraces(50);
		isolearningCoreDG->setPredictorsAsTraces(50);
        } else {
                isolearningShell->setPredictorsAsBandp(F_FOOD_LEARN,Q_FOOD);
		isolearningCoreLG->setPredictorsAsBandp(F_FOOD_LEARN,Q_FOOD);
		isolearningCoreDG->setPredictorsAsBandp(F_FOOD_LEARN,Q_FOOD);
	}

	
	//The direction triggers
	CoreLGOut=0.0;
	CoreDGOut=0.0;

	lastVTA=0;
	d2_auto=0; // just a guess
	d2_receptor=0;
	da_reup=0; // da with reuptake or whatever
	step=0;
#ifndef THIRD_FACTOR
	fprintf(stderr,"!!!!!!!!!!! Third factor is off !!!!!!!!!!!!!!\n");
	sleep(1);
#endif
#ifndef SHELL_LEARNING
	fprintf(stderr,"!!!!!!!!!!! The shell does NOT learn !!!!!!!!!!!!!!\n");
	sleep(1);
#endif
};

void Limbic_system::setLearningRate(float mu)
{
	fprintf(stderr,"Shell: setting learning rate to: %f\n",mu);
	fprintf(stderr,"Shell: setting learning rate in shell to: %f\n",mu*1);
#ifdef SHELL_LEARNING
	isolearningShell->setLearningRate(mu);
#else
	isolearningShell->setLearningRate(0);
#endif	
	isolearningCoreLG->setLearningRate(mu);
	isolearningCoreDG->setLearningRate(mu);
}

void Limbic_system::doStep(float us,float cs1, float cs2,float x0LG,float x0DG, float x1LG, float x1DG) {
	isolearningShell->setInput(0,us*1);
	isolearningShell->setInput(1,cs1*1);
	isolearningShell->setInput(2,cs2*1);

	//setting the inputs of the core corresponding to each place field
	isolearningCoreLG->setInput(0,x0LG);
	isolearningCoreLG->setInput(1,x1LG);
	isolearningCoreLG->setInput(2,x1DG);
	isolearningCoreDG->setInput(0,x0DG);
	isolearningCoreDG->setInput(1,x1DG);
	isolearningCoreDG->setInput(2,x1LG);
	isolearningShell->prediction(step);

	//prediction step  of the direction triggers
	isolearningCoreLG->prediction(step);
	isolearningCoreDG->prediction(step);
		
	double nacc=isolearningShell->getOutput();
	double lh=(isolearningShell->getFilter(0,0)->getActualOutput());

	double vp=1/(1+nacc*NACC_VP);
	lh=lh/(1+nacc*NACC_LH);
	double netInhib=(d2_auto*DFACT)+(vp*VP_VTA+nacc*NACC_VTA);

	
	if (netInhib<0) netInhib=0;
	double vta=(lh+BASELINE_VTA)/(1+netInhib);
        double derivVTA=vta-lastVTA;
        lastVTA=vta;
        double thirdFactor=0;
	if (derivVTA>0.2){
		thirdFactor=derivVTA*LTP_GAIN;
		d2_auto=0;
		d2_receptor=0;
	} else {
		da_reup=(double)vta-0.5;
	}
	if (da_reup<0) {
		da_reup=0;
	}
	// ignore the level of the vta during bursting
	d2_auto=d2_auto+(da_reup-d2_auto)*((double)TAU_D2);
	d2_receptor=d2_receptor+(da_reup-d2_receptor)*((double)TAU_D2);
	//default baseline
	float ltd=d2_receptor;
	if (ltd>0) {
	  ltd=ltd*LTD_GAIN;
	}



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





#ifdef THIRD_FACTOR
		isolearningShell->set3rdFactor(thirdFactor_sh);
#ifdef WITH_LTD
		isolearningShell->setDecay(ltd_sh);
#endif
		isolearningCoreLG->set3rdFactor(thirdFactor);
		isolearningCoreDG->set3rdFactor(thirdFactor);
#ifdef WITH_LTD
		isolearningCoreLG->setDecay(ltd);
		isolearningCoreDG->setDecay(ltd);
#endif
#endif	
		CoreLGOut=isolearningCoreLG->getOutput();
		CoreDGOut=isolearningCoreDG->getOutput();

		
	if (f) 
	  {
		fprintf(f,
			"%ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			step,  	                                //01
		        d2_receptor,                            //02
			us,                                     //03
			cs1,                                    //04
			cs2,                                    //05
			nacc,                                   //06
			vp,                                     //07 
			vta,                                    //08 
			derivVTA,                               //09
			thirdFactor,                            //10
			ltd,                                    //11
			lh,                                     //12
			isolearningShell->getWeight(1,0),       //13
			isolearningShell->getWeight(2,0),       //14
			isolearningShell->getFilter(0,0)->getActualOutput(),        //15
			isolearningShell->getFilter(1,0)->getActualOutput(),        //16
			isolearningShell->getFilter(2,0)->getActualOutput(),        //17
			da_reup,                                                    //18
                        d2_receptor                                                 //19
			);
	  }
	//document for the core
	if (fcore)
	  {
	    fprintf(fcore,"%ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		    step,  	                                                   //01
		    x0LG,                                                          //02
		    x0DG,                                                          //03
		    isolearningCoreLG->getFilter(0,0)->getActualOutput(),          //04
		    isolearningCoreDG->getFilter(0,0)->getActualOutput(),          //05
		    CoreLGOut,                                                     //06
		    CoreDGOut,                                                     //07
		    thirdFactor,                                                   //08
		    isolearningCoreLG->getFilter(0,0)->getActualOutput(),          //09
		    isolearningCoreLG->getFilter(1,0)->getActualOutput(),          //10
		    isolearningCoreDG->getFilter(0,0)->getActualOutput(),          //11
		    isolearningCoreDG->getFilter(1,0)->getActualOutput(),          //12	
		    isolearningCoreLG->getWeight(0,0),                             //13
		    isolearningCoreDG->getWeight(0,0),                             //14
		    isolearningCoreLG->getWeight(1,0),                             //15
		    isolearningCoreDG->getWeight(1,0),                             //16	
		    isolearningCoreLG->getWeight(2,0),                             //17
		    isolearningCoreDG->getWeight(2,0),                             //18
		    x1LG,                                                          //19
		    x1DG,                                                          //20
		    vta,                                                           //21
		    ltd,                                                           //22
		    isolearningCoreLG->getDeltaOutput(),                           //23
		    isolearningCoreDG->getDeltaOutput()                            //24
		    );
		}  
	step++;  
			
}	
                                      
		                                            



Limbic_system::~Limbic_system() 
{
	if (f) fclose(f);
	if (fcore) fclose(fcore);
	delete isolearningShell;
	delete isolearningCoreLG;
	delete isolearningCoreDG;
}
