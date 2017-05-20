float d;
do {
	x=(int)((((float)random()+(float)random())/
		 (float)RAND_MAX)*maxx)/2;
	y=(int)((((float)random()+(float)random())/
		 (float)RAND_MAX)*maxy)/2;
	d=sqrt((x-lastXfood)*(x-lastXfood)+(y-lastYfood)*(y-lastYfood));
} while ((!drawFood(step,x,y,FOOD_DIAMETER,index))||(d<100));
