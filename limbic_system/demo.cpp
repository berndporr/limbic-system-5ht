#include"shell.h"
#include<stdio.h>

int trace=0;
int demoNumber=0;



void testShell() {
	// set up our limbic system
	Shell* shell=new Shell();
	shell->setLearningRate(0.00001);
	
        for(int step=0;step<120000;step++) {
		float us=0;
		float cs=0;
		
		if ((step>20000)&&(step<100000)) {
			// just the typical timing
			// first pairing and then just the CS
			int m=step%1000;
			if ((m>400)&&(m<600)) {
				cs=1;
			}
			if ((step%1000==590)&&(step<50000)) {
				us=1;
			}
		}

		float cs1=cs;
		float cs2=0;
		shell->doStep(us,cs1,cs2,0,0,0,0,0,0,0);
	}
	delete shell;
	
}	

void testVTA() {
	// tests over a shorter time range
	Shell* shell=new Shell();
	shell->setLearningRate(0.00001);
	
        for(int step=0;step<40000;step++) {
		float us=0;
		float cs=0;
		
		if ((step>1000)&&(step<10000)) {
			// just the typical timing
			// first pairing and then just the CS
			int m=step%1000;
			if ((m>400)&&(m<600)) {
				cs=1;
			}
			if ((step%1000==590)&&(step<5000)) {
				us=1;
			}
		}
		float cs1=cs;
		float cs2=0;
		if ((step>1000)&&(step>20000)&&(step<30000)) {
			// just the typical timing
			// first pairing and then just the CS
			int m=step%1000;
			if ((m>400)&&(m<600)) {
				cs=1;
			}
			if ((step%1000==590)&&(step<25000)) {
				us=0;
			}
		}
		cs1=0;
		cs2=1;
		shell->doStep(us,cs1,cs2,0,0,0,0,0,0,0);
	}
	delete shell;
	
}	

int main(int argc, char *argv[]) {
        int c;
        int demoNumber=-1;

        while (-1 != (c = getopt(argc, argv, "td:"))) {
                switch (c) {
                case 'd':
                        demoNumber = strtoul(optarg,NULL,0);
                        break;

                case 't':
                        // switch to trace instead of bandpass
                        trace = 1;
                        fprintf(stderr,"Filter responses are traces\n");
                        break;

                default:
                        printf("bad option\n");
                        exit(1);
                }
        }
	if (demoNumber<0) {
		fprintf(stderr,"I need a demo number: %s -d demo#\n",argv[0]);
		exit(1);
	}

	switch (demoNumber) {
	case 0:
		testShell();
		break;
	case 1:
		testVTA();
		break;

	default:
		fprintf(stderr,"Invalid demo number.\n");
		exit(1);
		
	} // switch
}
