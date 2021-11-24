#include "mbed.h"
#include "hcsr04.h"
#include "fir_coeffs.h"
#include "TextLCD.h"

#include <stdint.h>

#define SAMPLING_FREQUENCY 100.0f
#define LCD_UPDATE_TIME 1.0f
#define CONE_HEIGHT 70.0f

struct FIRFilter
{
  float   buf[FILTER_NUM_COEFFS];
  uint8_t bufIndex;

  float out;
};

void FIRFilter_Init(FIRFilter *fir);
float FIRFilter_Update(FIRFilter *fir, float inp);

Serial pc(PB_6, PB_7);

HCSR04 usensor(PB_10, PB_11);
TextLCD lcd(PA_8, PA_9, PA_10, PA_11, PA_12, PA_15);

Ticker sensor_ticker;
void readUltrassonic();

Ticker lcd_ticker;
void updateLCD();

unsigned int millis = 0;

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


FIRFilter lpf;
float dist_raw;
float dist_filtered;
float water_height;

bool update_lcd = true;

int main()
{
  FIRFilter_Init(&lpf);

  sensor_ticker.attach(&readUltrassonic, 1/SAMPLING_FREQUENCY);
  lcd_ticker.attach(&updateLCD, LCD_UPDATE_TIME);

  while(1)
  {
    if (update_lcd)
    {
      lcd.cls();
      lcd.printf("H: %.2f cm", water_height);
      update_lcd = false;
    }
    pc.printf("%.2f\n", water_height);
    wait(1/SAMPLING_FREQUENCY);
  }

}


void readUltrassonic()
{
  usensor.start();
  dist_raw = min(usensor.get_dist_cm(), CONE_HEIGHT);
  dist_filtered = FIRFilter_Update(&lpf, dist_raw);
  water_height = CONE_HEIGHT - dist_filtered;
}


void updateLCD()
{
  update_lcd = true;
}