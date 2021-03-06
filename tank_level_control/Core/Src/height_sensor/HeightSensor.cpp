/*
 * HeightSensor.cpp
 *
 *  Created on: Nov 26, 2021
 *      Author: Felipe Mohr
 */

#include "HeightSensor.h"


void FIRFilter_Init(FIRFilter *fir)
{
  for (uint8_t n=0; n<FILTER_NUM_COEFFS; n++)
    fir->buf[n] = 0.0f;
  fir->bufIndex = 0;
  fir->out = 0.0f;
}


float FIRFilter_Update(FIRFilter *fir, float inp)
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


HeightSensor::HeightSensor(GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN,
	  	   	   	   	   	   GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN,
						   TIM_HandleTypeDef HTIM)
							: trig_port(TRIG_PORT), trig_pin(TRIG_PIN),
							  echo_port(ECHO_PORT), echo_pin(ECHO_PIN),
							  htim(HTIM){
	FIRFilter_Init(&lpf);

	HAL_TIM_Base_Start(&htim);
	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);
}

HeightSensor::HeightSensor()
{
	FIRFilter_Init(&lpf);
}

void HeightSensor::initialize(GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN,
	   	   	   	  	  	  	  GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN,
							  TIM_HandleTypeDef HTIM)
{
	trig_port = TRIG_PORT;
	trig_pin = TRIG_PIN;
	echo_port = ECHO_PORT;
	echo_pin = ECHO_PIN;
	htim = HTIM;

	HAL_TIM_Base_Start(&htim);
	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);
}

float HeightSensor::getDistRaw()
{
	return dist_raw;
}

float HeightSensor::getDistFiltered()
{
	return dist_filtered;
}

float HeightSensor::getWaterHeight()
{
	return water_height;
}


void HeightSensor::readSensor()
{
	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	__HAL_TIM_SET_COUNTER(&htim, 0);
	while (__HAL_TIM_GET_COUNTER (&htim) < 10);  // wait for 10 us
	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	uint32_t millis = HAL_GetTick(); // used this to avoid infinite while loop  (for timeout)
	// wait for the echo pin to go high
	while (!(HAL_GPIO_ReadPin (echo_port, echo_pin)) && millis + 10 >  HAL_GetTick());
	uint32_t time1 = __HAL_TIM_GET_COUNTER (&htim);

	millis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	// wait for the echo pin to go low
	while ((HAL_GPIO_ReadPin (echo_port, echo_pin)) && millis + 50 > HAL_GetTick());
	uint32_t time2 = __HAL_TIM_GET_COUNTER (&htim);

	uint16_t distance_mm = (time2 - time1)* 0.34/2;

	dist_raw = (distance_mm/10.0) < CONE_HEIGHT ? (distance_mm/10.0) : CONE_HEIGHT;
	dist_filtered = FIRFilter_Update(&lpf, dist_raw);
	water_height = CONE_HEIGHT - dist_filtered;
}

