/*
 * HeightSensor.h
 *
 *  Created on: Nov 26, 2021
 *      Author: Felipe Mohr
 */

#ifndef SRC_FLOWSENSOR_H_
#define SRC_FLOWSENSOR_H_

#include "stm32f1xx_hal.h"
#include "fir_coeffs_flow.h"


struct FIRFilterFlow
{
  float   buf[FILTER_NUM_COEFFS];
  uint8_t bufIndex;

  float out;
};

void FIRFilterFlow_Init(FIRFilterFlow *fir);
float FIRFilterFlow_Update(FIRFilterFlow *fir, float inp);

class FlowSensor
{
private:

  FIRFilterFlow lpf;

  float flow_raw;
  float flow_filtered;


public:

  FlowSensor();

  void readSensor(int numPulses);

  float getFlowRaw();
  float getFlowFiltered();

};


#endif /* SRC_FLOWSENSOR_H_ */
