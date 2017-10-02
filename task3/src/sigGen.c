#include <stdio.h>
#include <stdlib.h>  
#include <visa.h>


int initializeSigGen(ViStatus viStatus, ViSession* instr)
{
	ViSession defaultRM;
	ViFindList fList;
	ViUInt32 numInst;
	ViChar desc[VI_FIND_BUFLEN];
	ViChar resultBuffer[256];
	ViUInt32 resultCount;

	viStatus=viOpenDefaultRM(&defaultRM);

	if(viStatus != VI_SUCCESS) return EXIT_FAILURE;

	viStatus=viFindRsrc(defaultRM, "USB[0-9]::0x0400?*INSTR", &fList, &numInst, desc);

	viStatus=viOpen(defaultRM, desc, VI_NULL, VI_NULL, instr);
	printf("Opened: %s\n", desc);
	fflush(stdout);
	sleep(2);

	if(viStatus != VI_SUCCESS) return EXIT_FAILURE;
	sleep(2);
	viStatus=viWrite(*instr, "*IDN?", 6, VI_NULL);
	if(viStatus != VI_SUCCESS) return EXIT_FAILURE;
	sleep(2);
	viStatus=viRead(*instr, resultBuffer, 256, &resultCount);
	if(viStatus != VI_SUCCESS) return EXIT_FAILURE;
	sleep(2);
	printf("Found instrument: %s", resultBuffer);
	fflush(stdout);
	return VI_SUCCESS;
}

int generateSin(ViStatus viStatus, ViSession* instr, float freq, float phase, float voltage, float dcOffset)
{
	char commandString[128];
	sprintf(commandString, "APPL:SIN %f, %f, %f", freq, voltage, dcOffset);
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "PHAS %f", phase);
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "OUTP ON");
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
  	sleep(2);
  	printf("Sine wave generated\n");
	fflush(stdout);
  	return EXIT_SUCCESS;
}

int setUnit(ViStatus viStatus, ViSession* instr, int option) //input 0 for VPP, 1 for RMS, or 2 for DBM
{
	if(option==0)
	{
		viStatus=viWrite(*instr, "VOLT:UNIT VPP", 14, VI_NULL);
		sleep(2);
	}
	if(option==1)
	{
		viStatus=viWrite(*instr, "VOLT:UNIT RMS", 14, VI_NULL);
		sleep(2);
	}
	if(option==2)
	{
		viStatus=viWrite(*instr, "VOLT:UNIT DBM", 14, VI_NULL);
		sleep(2);
	}
	if(option<0 && option>2)
	{
		printf("Invalid unit entry\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int sinSweep(ViStatus viStatus, ViSession* instr, float startFreq, float endFreq, float sweepTime)
{
	if(startFreq>endFreq)
	{
		printf("Sine sweep failed\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	printf("Initializing sweep\n");
	fflush(stdout);

	char commandString[128];

	sprintf(commandString, "FUNC SIN");
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "SWE:STAT ON");
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "SWE:STAT LIN");
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "FREQ:STAR %f", startFreq);
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "FREQ:STOP %f", endFreq);
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "SWE:TIME %f", sweepTime);
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "TRIG:SOUR IMM", startFreq);
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
	sleep(2);
	sprintf(commandString, "OUTP ON");
	viStatus=viWrite(*instr, commandString, sizeof commandString, VI_NULL);
  	sleep(2);

	printf("Sweep initialized\n");
	fflush(stdout);

	return EXIT_SUCCESS;
}
