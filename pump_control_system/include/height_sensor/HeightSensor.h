#include "mbed.h"
#include "hcsr04.h"
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

class HeightSensor : public HCSR04
{
private:
  FIRFilter lpf;

  float dist_raw;
  float dist_filtered;
  float water_height;

public:
  HeightSensor(PinName trig, PinName echo);
  ~HeightSensor();

  void readSensor();
  
  float getDistRaw();
  float getDistFiltered();
  float getWaterHeight();

};

