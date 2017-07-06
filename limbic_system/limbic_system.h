#ifndef _LIMBIC_SYSTEM
#define _LIMBIC_SYSTEM

#include <unistd.h>



class Limbic_system {
public:
	Limbic_system();
	~Limbic_system();

	void doStep(float reward,
			float placefield1,
			float placefield2,
			float on_contact_direction_LG,
			float on_contact_direction_DG,
			float visual_direction_LG,
			float visual_direction_DG
		    );

	float getLGOutput() {return CoreLGOut;};
	float getDGOutput() {return CoreDGOut;};
	void setLearningRate(float mu);

	float CoreLGOut;
	float CoreDGOut;
	long int step;
};

#endif
