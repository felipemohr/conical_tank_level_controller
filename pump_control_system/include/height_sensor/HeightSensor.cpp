#include "height_sensor/HeightSensor.h"

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


HeightSensor::HeightSensor(PinName trig, PinName echo) 
  : HCSR04(trig, echo)
{
  FIRFilter_Init(&lpf);

}

HeightSensor::~HeightSensor()
{
}


void HeightSensor::readSensor()
{
  start();
  dist_raw = min(get_dist_cm(), CONE_HEIGHT);
  dist_filtered = FIRFilter_Update(&lpf, dist_raw);
  water_height = CONE_HEIGHT - dist_filtered;
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
