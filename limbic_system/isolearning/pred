void Isolearning::prediction(int) {
	/**
	 * Feed the inputs into the filter bank
	 **/

	// channel 0 is no filter bank, it's just one filter
	getFilter(0,0)->filter(inputs[0]);

	// feed the input-channels into the Filter filters
	for(int j=1;j<nChannels;j++) {
		// look throught the filter bank
		for(int i=0;i<nFilters;i++) 
		  {
			// get an input
			float dd=inputs[j];
			if (onlyChange) 
			  {
				// filter out the DC
				dd=inputs[j]-lastInputs[j];
			  }
			// feed it into the filter
			getFilter(j,i)->filter(dd);
		  }
	}


	/**
	 * calculate the new activity: weighted sum over the filters
	 **/
	// get the reflex
	/**calculate the new activity: weighted sum over the filters**/
  
	actualActivity=getFilter(0,0)->getActualOutput();	// get the reflex
  
	for(int j=1;j<nChannels;j++)                          // loop through the predictive inputs
	  {
	    for(int i=0;i<nFilters;i++) 
	      {
		actualActivity=actualActivity+getWeight(j,i)*getFilter(j,i)->getActualOutput();
	      }
	  }

       	/**if (actualActivity > 1.0) 
	  {
	    actualActivity = 10.0;
	    }**/
	//fprintf(stderr,"weight: %f\n",weight[1][9]);
	// calculate the derivative of the output
	delta=actualActivity-lastActivity;
	lastActivity=actualActivity;

	/**
	 * change the weights
	 **/
	// run through different predictive input channels
	for(int j=1;j<nChannels;j++) {
		// run through the filter bank
		for(int i=0;i<nFilters;i++) 
		  {
			// ISO learning with 3rd factor 
		    float correl=thirdFactor*delta*getFilter(j,i)->getActualOutput()
		      -decay*getWeight(j,i)*fabs(getFilter(j,i)->getActualOutput())
		      *fabs(delta);
	 
		    //  float correl=thirdFactor*delta*getFilter(j,i)->getActualOutput()-decay*getWeight(j,i)*fabs(getFilter(j,i)->getActualOutput())*fabs(delta);
	 
			// calculate the integral by a linear approximation
			float integral=correl-(correl-lastCorrel[j][i])/2.0;
			// set the weight;
			setWeight(j,i,getWeight(j,i)+0.0001*integral);
			//	if (weight[j][i]!=0)
			 fprintf(stderr,"!!!weight: %f\n",weight[j][i]);
			// save the last correlation result. Needed for the
			// integral.
			lastCorrel[j][i]=correl;
		}
	}
}
