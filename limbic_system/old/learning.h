#ifndef LEARNING_H
#define LEARNING_H

#include "world.h"
#include "filter.h"
#include "parameteriterator.h"

#include <QtGlobal>
#include <QVarLengthArray>


class Learner
{
public:
	virtual void setInput(int index, qreal value)=0;
	virtual void setReflex(int stimulus)=0;

	virtual int step()=0;
	virtual void setReward(bool reward)=0;

	virtual void reset()=0;

	virtual bool waitReward() const { return false; }
	virtual bool waitSettle() const { return false; }

	World& world() { return world_; }

protected:
	Learner(World &world) : world_(world) {}
	World &world_;
};

class SerotoninPrediction2 : public Learner
{
public:
	SerotoninPrediction2(World &world, ParameterIterator &parameters);
	void init(bool reset=false);

	void setInput(int index, qreal value);
	void setReflex(int stimulus);

	int step();
	void setReward(bool reward);

	void reset() { init(true); }

	bool waitReward() const;
	bool waitSettle() const;

private:
	// references to parameters which can be changed by the parameter iterator
	const qreal &EP_bl, &LH_bl, &LHb_bl, &VTA_da_bl, &VTA_gaba_bl, &threshold;

	// appended underscore signifies previous value for derivative calculations
	qreal r_input;
	qreal b_input;

	qreal r_reflex;
	qreal b_reflex;

	SecondOrderLowpassFilter r_f_US;
	SecondOrderLowpassFilter b_f_US;

	qreal rewardSet;
	FIRFilter f_LH;
	qreal LH_;

	FIRFilter r_f_lOFC, b_f_lOFC;
	qreal r_lOFC_, b_lOFC_;

	SecondOrderLowpassFilter f_DRN;

	qreal EP;
	qreal LHb, LHb_;

	DelayFilter<15> r_t_mOFC;
	qreal r_mOFC_;

	DelayFilter<15> b_t_mOFC;
	qreal b_mOFC_;

	qreal VTA_gaba, VTA_da, VTA_da_;

	FIRFilter r_f_mPFC;
	qreal r_mPFC_;

	FIRFilter b_f_mPFC;
	qreal b_mPFC_;

	qreal r_core_ep, b_core_ep;
	qreal r_core_ep_, b_core_ep_;
	qreal r_core_out, b_core_out;
	qreal core_diff;

	qreal rr_w_core, rb_w_core;
	qreal bb_w_core, br_w_core;

	qreal r_w_shell, r_w_lOFC;
	qreal b_w_shell, b_w_lOFC;

	unsigned rewardCount;
};

#endif // LEARNING_H
