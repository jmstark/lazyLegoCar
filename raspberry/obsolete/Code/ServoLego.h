#ifndef ServoLego_h
#define ServoLego_h

#include <inttypes.h>

// TICK_SIZE is the time between interrupts. This is essentially the resolution
// of the PWM we're able to create. This is also an average value, because other
// interrupts will affect the frequency of ours.
#define PWM_TICK_SIZE 50 // uS
// PWM_PERIOD is the period of the PWM we want to have
#define PWM_PERIOD   13000 // uS (20ms)
// This calculation is used to determine the 'reset' number for the counter
#define PWM_TICK_PERIOD (int) (PWM_PERIOD / PWM_TICK_SIZE)

class ServoLego
{
public:
  ServoLego();
  void attach(int pin1, int pin2);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  void left();
  void right();
  void none();
};

#endif
