/*
 * HeightSensor.cpp
 *
 *  Created on: Nov 26, 2021
 *      Author: Felipe Mohr
 */

#include "FlowSensor.h"


void FIRFilterFlow_Init(FIRFilterFlow *fir)
{
  for (uint8_t n=0; n<FILTER_NUM_COEFFS; n++)
    fir->buf[n] = 0.0f;
  fir->bufIndex = 0;
  fir->out = 0.0f;
}


float FIRFilterFlow_Update(FIRFilterFlow *fir, float inp)
{
  fir->buf[fir->bufIndex] = inp;

  fir->bufIndex++;
  if(fir->bufIndex == FILTER_NUM_COEFFS)
    fir->bufIndex = 0;

  fir->out = 0.0f;

  uint8_t sumIndex = fir->bufIndex;

  for (uint8_t n=0; n<FILTER_NUM_COEFFS; n++)
  {
    if (sumIndex > 0) sumIndex--;
    else sumIndex = FILTER_NUM_COEFFS - 1;
    fir->out += FIR_COEFFS[n] * fir->buf[sumIndex];
  }

  return fir->out;

}


FlowSensor::FlowSensor(){
	FIRFilterFlow_Init(&lpf);
}


float FlowSensor::getFlowRaw()
{
	return flow_raw;
}

float FlowSensor::getFlowFiltered()
{
	return flow_filtered;
}


void FlowSensor::readSensor(int numPulses)
{
	flow_raw = numPulses/0.55;
	flow_filtered = FIRFilterFlow_Update(&lpf, flow_raw);
}

