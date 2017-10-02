#ifndef __SIGGEN_H
#define __SIGGEN_H

int initializeSigGen(ViStatus viStatus, ViSession* instr);
int generateSin(ViStatus viStatus, ViSession* instr, float freq, float phase, float voltage, float dcOffset);
int setUnit(ViStatus viStatus, ViSession* instr, int option);
int sinSweep(ViStatus viStatus, ViSession* instr, float startFreq, float endFreq, float sweepTime);

#endif