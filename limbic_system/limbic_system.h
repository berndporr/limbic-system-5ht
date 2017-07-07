#ifndef _LIMBIC_SYSTEM
#define _LIMBIC_SYSTEM

#include <unistd.h>

#include <filter.h>


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


	float weight_lg2lg = 0;
	float weight_lg2dg = 0;
	float weight_dg2dg = 0;
	float weight_dg2lg = 0;

	float learning_rate_core = 0;

private:
	inline void weightChange(float &w, float delta) {
		w += delta;
		if (w>1) w = 1;
		if (w<0) w = 0;
	}

	SecondOrderLowpassFilter* on_contact_direction_LG_filter;
	SecondOrderLowpassFilter* on_contact_direction_DG_filter;


};

#endif
