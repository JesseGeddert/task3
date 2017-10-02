#include <stdio.h>
#include <stdlib.h>  
#include <visa.h>
#include <sigGen.h>

ViStatus viStatus;
//ViSession defaultRM;
ViSession* viDG1022;
//ViFindList fList;
//ViUInt32 numInst;
//ViChar desc[VI_FIND_BUFLEN];
//ViChar resultBuffer[256];
//ViUInt32 resultCount;

int initialization(void)
{
  viStatus=initializeSigGen(viStatus, &viDG1022);

  if(viStatus!=VI_SUCCESS)
  {
    printf("Instrument not found\n");
    return EXIT_FAILURE;
  }

  return 0;
}

int 
setup(void)
{
  setUnit(viStatus, &viDG1022, 0);
  if(viStatus!=VI_SUCCESS)  return EXIT_FAILURE;

  viStatus=sinSweep(viStatus, &viDG1022, 100, 20000, 2);
  if(viStatus!=VI_SUCCESS)  return EXIT_FAILURE;

  return 0;
}

int
loop(void)
{

  return 0;
}

int
cleanup(void)
{
  return 0;
}

  
int  
main(void)  
{  

  int r = 0;
  r = initialization();
  if(!r) r = setup();
  if(!r) r = loop();
  r = cleanup();

  return(0);  
}  