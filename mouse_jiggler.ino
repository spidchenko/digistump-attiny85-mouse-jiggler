#include "DigiMouse.h"

#undef abs

#define INTERNAL_LED_PIN 1                 // LED on Model B
#define MAX_JIGGLE_DELAY_SEC 20
#define MAX_RANDOM_COORD_OFFSET 5
#define JIGGLE_STEPS 10
#define FLOATING_ANALOG_PIN 2     // Floating analog pin used to gather environmental noise

// Linear Congruential Generator parameters:
#define C 1
#define A 0x10DCD
#define M 0x100000000

static int32_t lcg_state = 0;


void setup()
{
  lcg_state = get_floating_entropy(); // Random Seed for LCG
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  pinMode(FLOATING_ANALOG_PIN, INPUT);
  DigiMouse.begin();
}


void loop()
{
  led_on();
  jiggle();
  led_off();
  DigiMouse.delay(get_random_delay_ms());
}


/*
 * Nudge the mouse by a small random offset (-5..+5 on each axis), moving 
 * smoothly over JIGGLE_STEPS steps across ~1 second rather than jumping instantly.
 */
void jiggle()
{
  int8_t deltaX = get_random_coord();
  int8_t deltaY = get_random_coord();

  // Gradually move to new (dX,dY) in 5 steps (1 sec total):
  int8_t prevX = 0, prevY = 0;
  for (int i = 1; i <= JIGGLE_STEPS; i++) {
    int8_t targetX = stepTarget(deltaX, i, JIGGLE_STEPS);
    int8_t targetY = stepTarget(deltaY, i, JIGGLE_STEPS);

    DigiMouse.move(targetX - prevX, targetY - prevY, 0);
    DigiMouse.update();
    DigiMouse.delay(1000 / JIGGLE_STEPS);

    prevX = targetX;
    prevY = targetY;    
  }
}  


/*
 * Compute the rounded absolute position along one axis after
 * `step` out of `totalSteps` steps of a linear move from 0 to `delta`.
 */
int8_t stepTarget(int8_t delta, int step, int totalSteps) {
  return (int8_t)round(delta * (step / (float)totalSteps));
}


/*
 * Get initial seed from hardware noise
 * Generates a seed by accumulating the least significant bits
 * of noise from an unconnected analog pin
 */
uint16_t get_floating_entropy()
{
  uint16_t seed = 0;
  for(int i = 0; i < 16; i++){
    uint8_t lsb = analogRead(FLOATING_ANALOG_PIN) & 0x01;
    seed = (seed << 1) | lsb;
    delayMicroseconds(250);
  }
  return seed;
}


/*
 * Advances the LCG (linear congruential generator) by one step, updating the internal state and returning the new pseudo-random value.
 */
int32_t lcg_next()
{
    lcg_state = (lcg_state * A + C) % M;
    return lcg_state;
}


/*
 * Return random coordinate offset in the range -MAX_RANDOM_COORD_OFFSET..+MAX_RANDOM_COORD_OFFSET
 * MAX_RANDOM_COORD_OFFSET must stay <= 127 to avoid int8_t overflow
 */
int8_t get_random_coord()
{
  return (int8_t)(lcg_next() % (MAX_RANDOM_COORD_OFFSET + 1));
}


/*
 * Return random time interval 1..MAX_JIGGLE_DELAY_SEC in miliseconds
 *
 * MAX_JIGGLE_DELAY_SEC must stay <= 65 to avoid uint16_t overflow on return
 */
uint16_t get_random_delay_ms()
{  
  return ((abs(lcg_next()) % MAX_JIGGLE_DELAY_SEC) + 1) * 1000;
}


/*
 * Custom Absolute value function instead of defined macros
 */
uint32_t abs(int32_t x)
{
  return x < 0 ? -x : x;
}


void led_on()
{
  digitalWrite(INTERNAL_LED_PIN, HIGH);
}


void led_off()
{
  digitalWrite(INTERNAL_LED_PIN, LOW);
}
