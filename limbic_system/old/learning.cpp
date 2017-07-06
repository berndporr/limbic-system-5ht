#include <QtDebug>

#include "learning.h"

#define SET_PARAMETER(name) name(parameters.getParameterByName(#name))

SerotoninPrediction2::SerotoninPrediction2(World &world, ParameterIterator &parameters) :
  Learner(world),
	SET_PARAMETER(EP_bl),
	SET_PARAMETER(LH_bl),
	SET_PARAMETER(LHb_bl),
	SET_PARAMETER(VTA_da_bl),
	SET_PARAMETER(VTA_gaba_bl),
  SET_PARAMETER(threshold),
  r_f_US(0.002),
  b_f_US(0.002),
  f_LH("lh_impulse.mat", LH_bl),
  r_f_lOFC("lh_impulse.mat"),
  b_f_lOFC("lh_impulse.mat"),
	f_DRN(0.02),
  r_f_mPFC("lh_impulse.mat"),
  b_f_mPFC("lh_impulse.mat")
{
	init();

#define REGISTER(name) world.registerProbeVariable(#name, &name)

	REGISTER(r_input);
	REGISTER(b_input);
	REGISTER(r_reflex);
	REGISTER(b_reflex);
	REGISTER(r_f_US);
	REGISTER(b_f_US);
	REGISTER(rewardSet);
	REGISTER(f_LH);
	REGISTER(r_f_lOFC);
	REGISTER(b_f_lOFC);
	REGISTER(f_DRN);
	REGISTER(EP);
	REGISTER(LHb);
	REGISTER(r_t_mOFC);
	REGISTER(b_t_mOFC);
	REGISTER(VTA_gaba);
	REGISTER(VTA_da);
	REGISTER(r_f_mPFC);
	REGISTER(b_f_mPFC);
	REGISTER(r_core_ep);
	REGISTER(b_core_ep);
	REGISTER(r_core_out);
	REGISTER(b_core_out);
	REGISTER(core_diff);
	REGISTER(rr_w_core);
	REGISTER(rb_w_core);
	REGISTER(bb_w_core);
	REGISTER(br_w_core);
	REGISTER(r_w_shell);
	REGISTER(b_w_shell);
	REGISTER(r_w_lOFC);
	REGISTER(b_w_lOFC);
}

// saner to have initialisation and reset in the same function
void SerotoninPrediction2::init( bool reset )
{
  r_input = 0;
  b_input = 0;
	r_reflex = 0;
	b_reflex = 0;
  rewardSet = false;
	LH_ = LH_bl;
	r_lOFC_ = 0;
	b_lOFC_ = 0;
  EP = EP_bl;
  LHb = LHb_bl;
  LHb_ = LHb_bl;
  r_mOFC_ = 0;
  b_mOFC_ = 0;
  VTA_gaba = 0;
  VTA_da = VTA_da_bl;
  VTA_da_ = VTA_da_bl;
	r_mPFC_ = 0;
	b_mPFC_ = 0;
	r_core_ep = 0;
	b_core_ep = 0;
	r_core_out = 0;
	b_core_out = 0;
	core_diff = 0;
	rr_w_core = 0;
	rb_w_core = 0;
	bb_w_core = 0;
	br_w_core = 0;
  r_w_shell = 0;
  b_w_shell = 0;
  r_w_lOFC = 0;
  b_w_lOFC = 0;
  rewardCount = 0;

	if( reset )
	{
		r_f_US.reset();
		b_f_US.reset();
		f_LH.reset();
		r_f_lOFC.reset();
		b_f_lOFC.reset();
		f_DRN.reset();
		r_t_mOFC.reset();
		b_t_mOFC.reset();
		r_f_mPFC.reset();
		b_f_mPFC.reset();
	}
}

void SerotoninPrediction2::setInput(int index, qreal value)
{
	if(index == 0)
		r_input = value;
	else if(index == 1)
		b_input = value;
}

void SerotoninPrediction2::setReflex(int stimulus)
{
		if(stimulus == 0)
			r_reflex = 1;
		else if(stimulus == 1)
			b_reflex = 1;
}

void SerotoninPrediction2::setReward(bool reward)
{
	if( reward && rewardSet==0 )
		++rewardCount;

	rewardSet = reward;
}


bool SerotoninPrediction2::waitReward() const
{
	return VTA_da>(VTA_da_bl+0.0001) || VTA_da<(VTA_da_bl-0.0001);
}


bool SerotoninPrediction2::waitSettle() const
{
	return (VTA_gaba - VTA_gaba_bl) > 0.01 || r_f_US > 0.01 || b_f_US > 0.01;
}


int SerotoninPrediction2::step()
{
	/** Variable prefixes
		*
		* r_ - red stimulus
		* b_ - blue stimulus
		* f_ - filter (basic filter class contains a qreal typecast, so many normal variable operations will use the output of the filter)
		* t_ - delay filter, works the same as above
		* w_ - learning weight
		* d_ - derivative
		* pd_ - positive part of a derivative
		* nd_ - rectified negative part of a derivative
		*
		*
		* Variable postfixes
		*
		* _ - previous value for derivative calculation
		* _pa - persistent activity
		**/

	/** First the inputs **/

	f_LH(LH_bl + 13*rewardSet);

	r_f_US(r_reflex);
	b_f_US(b_reflex);

	r_f_lOFC(r_input > threshold);
	b_f_lOFC(b_input > threshold);

	qreal r_nd_lOFC = r_lOFC_ - r_f_lOFC;
	if( r_nd_lOFC < 0 ) r_nd_lOFC = 0;

	qreal b_nd_lOFC = b_lOFC_ - b_f_lOFC;
	if( b_f_lOFC < 0 ) b_f_lOFC = 0;

	// floating...
	f_DRN(r_f_lOFC*r_w_lOFC + b_f_lOFC*b_w_lOFC);


	/** Now the mOFC->m_shell->vmVP->VTA pathway **/

	// mOFC activity is simply a delay of lOFC_pa
	r_t_mOFC(r_f_lOFC);
	b_t_mOFC(b_f_lOFC);

	// derivative of mOFC
	qreal r_d_mOFC = r_t_mOFC - r_mOFC_;
	qreal b_d_mOFC = b_t_mOFC - b_mOFC_;

	// positive and inverted negative components of derivative of mOFC
	// values will be small, so not much point plotting...
	qreal r_pd_mOFC = (r_d_mOFC > 0) ? (r_d_mOFC)  : (0);
	qreal r_nd_mOFC = (r_d_mOFC < 0) ? (-r_d_mOFC) : (0);

	qreal b_pd_mOFC = (b_d_mOFC > 0) ? (b_d_mOFC)  : (0);
	qreal b_nd_mOFC = (b_d_mOFC < 0) ? (-b_d_mOFC) : (0);

	VTA_gaba += 0.25 * (r_t_mOFC*r_w_shell + b_t_mOFC*b_w_shell + (f_LH-LH_bl)/13) - 0.2;
	if( VTA_gaba < VTA_gaba_bl ) VTA_gaba = VTA_gaba_bl;
	if( VTA_gaba > 70 ) VTA_gaba = 70;

	qreal pd_LH = f_LH - LH_;
	if( pd_LH < 0 ) pd_LH = 0;

	VTA_da = ( VTA_da_bl*(1+LHb_bl) + (r_pd_mOFC*200*r_w_shell + b_pd_mOFC*200*b_w_shell + pd_LH*30) / (1 + VTA_gaba - VTA_gaba_bl) ) / (1 + LHb_	);


	/** mPFC->core->EP->LHb->VTA pathway **/

	r_f_mPFC(r_input);
	b_f_mPFC(b_input);

	qreal r_nd_mPFC = r_mPFC_ - r_f_mPFC;
	if(r_nd_mPFC < 0 ) r_nd_mPFC = 0;

	qreal b_nd_mPFC = b_mPFC_ - b_f_mPFC;
	if(b_nd_mPFC < 0 ) b_nd_mPFC = 0;

//	r_core = rr_w_core*r_f_mPFC + rb_w_core*b_f_mPFC - f_DRN/(1+r_f_mPFC);
//	b_core = br_w_core*r_f_mPFC + bb_w_core*b_f_mPFC - f_DRN/(1+b_f_mPFC);

//	r_core = rr_w_core*r_f_mPFC + rb_w_core*b_f_mPFC + r_f_US;
//	b_core = br_w_core*r_f_mPFC + bb_w_core*b_f_mPFC + b_f_US;

//	r_core = rr_w_core*r_f_mPFC + rb_w_core*b_f_mPFC - f_DRN*b_f_mPFC;
//	b_core = br_w_core*r_f_mPFC + bb_w_core*b_f_mPFC - f_DRN*r_f_mPFC;

	r_core_ep = rr_w_core*r_f_mPFC + rb_w_core*b_f_mPFC;
	b_core_ep = br_w_core*r_f_mPFC + bb_w_core*b_f_mPFC;

	// justifiable?
	r_core_out = r_core_ep + 0.5*r_f_mPFC;
	b_core_out = b_core_ep + 0.5*b_f_mPFC;

	core_diff = r_core_out - b_core_out;

	// the output!
	int approach = -1;

	if( r_core_out >= threshold || b_core_out >= threshold )
		approach = (r_core_out > b_core_out) ? 0 : 1;

	qreal r_nd_core = r_core_ep_ - r_core_ep;
	if( r_nd_core < 0 ) r_nd_core = 0;

	qreal b_nd_core = b_core_ep_ - b_core_ep;
	if( b_nd_core < 0 ) b_nd_core = 0;

	EP = EP_bl + 160 * (r_nd_core + b_nd_core);

	// precisely zero point in LH and EP baselines then
	LHb = ( LHb_bl*(1+VTA_da_bl) + ((EP-EP_bl) / ( 1 + 99*(f_LH-LH_bl) )) ) / (1 + VTA_da_);


	/** Update weights **/

	qreal burst = VTA_da - VTA_da_bl;

	// core
  rr_w_core += burst * r_nd_mPFC * r_core_out;
	if( rr_w_core > 1 ) rr_w_core = 1;
	else if( rr_w_core < 0 ) rr_w_core = 0;

  rb_w_core += burst * b_nd_mPFC * r_core_out;
	if( rb_w_core > 1 ) rb_w_core = 1;
	else if( rb_w_core < 0 ) rb_w_core = 0;

  br_w_core += burst * r_nd_mPFC * b_core_out;
	if( br_w_core > 1 ) br_w_core = 1;
	else if( br_w_core < 0 ) br_w_core = 0;

  bb_w_core += burst * b_nd_mPFC * b_core_out;
	if( bb_w_core > 1 ) bb_w_core = 1;
	else if( bb_w_core < 0 ) bb_w_core = 0;

	// lOFC
	r_w_lOFC += burst * 0.5 * r_nd_lOFC;
	if( r_w_lOFC > 1 ) r_w_lOFC = 1;
	else if( r_w_lOFC < 0 ) r_w_lOFC = 0;

	b_w_lOFC += burst * 0.5 * b_nd_lOFC;
	if( b_w_lOFC > 1 ) b_w_lOFC = 1;
	else if( b_w_lOFC < 0 ) b_w_lOFC = 0;

	// m-shell
	r_w_shell += burst * 0.1 * r_nd_mOFC;
	if( r_w_shell > 1 ) r_w_shell = 1;
	else if( r_w_shell < 0 ) r_w_shell = 0;

	b_w_shell += burst * 0.1 * b_nd_mOFC;
	if( b_w_shell > 1 ) b_w_shell = 1;
	else if( b_w_shell < 0 ) b_w_shell = 0;


	/** Finish up **/

	// Need this to plot the variable values
	world_.updateProbes();

	// update labels
	world_.setLabel(1, QString("Reward Count: %1").arg(rewardCount));

	world_.setLabel(5, QString("r_w_shell: %1").arg(r_w_shell, 0, 'f'));
	world_.setLabel(6, QString("b_w_shell: %1").arg(b_w_shell, 0, 'f'));

	world_.setLabel(10, QString("r_w_lOFC: %1").arg(r_w_lOFC, 0, 'f'));
	world_.setLabel(11, QString("b_w_lOFC: %1").arg(b_w_lOFC, 0, 'f'));

	world_.setLabel(15, QString("rr_w_core: %1").arg(rr_w_core, 0, 'f'));
	world_.setLabel(16, QString("rb_w_core: %1").arg(rb_w_core, 0, 'f'));
	world_.setLabel(17, QString("br_w_core: %1").arg(br_w_core, 0, 'f'));
	world_.setLabel(18, QString("bb_w_core: %1").arg(bb_w_core, 0, 'f'));

	// set previous values for derivative calculations on next step
	r_lOFC_ = r_f_lOFC;
	b_lOFC_ = b_f_lOFC;

	r_mOFC_ = r_t_mOFC;
	b_mOFC_ = b_t_mOFC;

	r_mPFC_ = r_f_mPFC;
	b_mPFC_ = b_f_mPFC;

	r_core_ep_ = r_core_ep;
	b_core_ep_ = b_core_ep;

	LH_ = f_LH;
	LHb_ = LHb;
	VTA_da_ = VTA_da;

	// clear reflexes
	r_reflex = 0;
	b_reflex = 0;

	return approach;
}
