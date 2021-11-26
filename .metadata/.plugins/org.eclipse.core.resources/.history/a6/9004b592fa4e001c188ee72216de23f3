/*
 * HeightSensor.h
 *
 *  Created on: Nov 26, 2021
 *      Author: Felipe Mohr
 */

#ifndef SRC_HEIGHTSENSOR_H_
#define SRC_HEIGHTSENSOR_H_

#include "stm32f1xx_hal.h"
#include "fir_coeffs.h"

#define CONE_HEIGHT 70.0f

struct FIRFilter
{
  float   buf[FILTER_NUM_COEFFS];
  uint8_t bufIndex;

  float out;
};

void FIRFilter_Init(FIRFilter *fir);
float FIRFilter_Update(FIRFilter *fir, float inp);

class HeightSensor
{
private:

  TIM_HandleTypeDef htim;
  GPIO_TypeDef* trig_port;
  GPIO_TypeDef* echo_port;
  uint16_t trig_pin;
  int16_t echo_pin;

  FIRFilter lpf;

  float dist_raw;
  float dist_filtered;
  float water_height;


public:
  HeightSensor(GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN,
   	   	   	   GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN,
			   TIM_HandleTypeDef HTMI) ;
  ~HeightSensor();

  void readSensor();

  float getDistRaw();
  float getDistFiltered();
  float getWaterHeight();

};


#endif /* SRC_HEIGHTSENSOR_H_ */
