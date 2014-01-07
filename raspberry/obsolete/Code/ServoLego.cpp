#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino.h>
#include <ServoLego.h>

// Interrupt variables have to be defined as 'volatile' so that the CPU always
// fetches them from RAM and doesn't get an incorrect value from cache
volatile unsigned int timer2_counter = 0;
volatile unsigned int timer2_trigger_low = 500;
int level = 0;
int PWM_PIN1;
int PWM_PIN2;
// Timer2 overflow interrupt vector handler
// This is called every time timer 2 overflows (around every 50ms)
// This routine needs to be as small and short as possible so that we don't
// impact the rest of the Arduino by executing lots of code in the background
ISR(TIMER2_OVF_vect) {
  timer2_counter++;
  if(timer2_counter == timer2_trigger_low) {
    level = 0;
  } else if(timer2_counter >= PWM_TICK_PERIOD) {
    timer2_counter = 0;
    level = 1;
  }
  // Reset the timer2 counter so that it can count and overflow once again
  TCNT2 = 0;
}

ServoLego::ServoLego(){
	TIMSK2 = 1<<TOIE2;  // Timer 2 overflow interrupt enable
	TCNT2 = 0;
}

void ServoLego::attach(int pin1, int pin2){
	
	PWM_PIN1 = pin1;
	PWM_PIN2 = pin2;
	pinMode(PWM_PIN1, OUTPUT);
	pinMode(PWM_PIN2, OUTPUT);
}

void ServoLego::left(){

	digitalWrite(PWM_PIN1, level);
    digitalWrite(PWM_PIN2, LOW);
}

void ServoLego::right(){

	digitalWrite(PWM_PIN2, level);
    digitalWrite(PWM_PIN1, LOW);
}

void ServoLego::none(){

	digitalWrite(PWM_PIN1, LOW);
    digitalWrite(PWM_PIN2, LOW);
}