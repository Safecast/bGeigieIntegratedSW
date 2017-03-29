
#include <Arduino.h>

#include "RotaryEncoder.h"

unsigned char step_count = 0;
unsigned char position = 0;
unsigned char max_pos = 0;

int pin_a, pin_b;

unsigned int state_a;
unsigned int state_b;

void interrupt_a()
{
  int a = digitalRead(pin_a);

  if (state_b == HIGH)
  {
    if (state_a == HIGH && a == LOW)
      position++;
    else
      position--;
  }
  else
  {
    if (state_a == HIGH && a == LOW)
      position--;
    else
      position++;
  }

  // wrap around
  if (position >= max_pos)
    position = 0;
  else if (position < 0)
    position = max_pos - 1;

  // save state
  state_a = a;

}

void interrupt_b()
{
  int b = digitalRead(pin_b);

  if (state_a == LOW)
  {
    if (state_b == HIGH && b == LOW)
      position++;
    else
      position--;
  }
  else
  {
    if (state_b == HIGH && b == LOW)
      position--;
    else
      position++;
  }

  // wrap around
  if (position >= max_pos)
    position = 0;
  else if (position < 0)
    position = max_pos - 1;

  // save state
  state_b = b;
}

void rotary_encoder_setup(int _pin_a, int _pin_b, int _max_pos)
{
  // assign pin values
  pin_a = _pin_a;
  pin_b = _pin_b;

  // the number of positions of the encoder
  position = 0;
  max_pos = _max_pos * 4;

  // set as inputs
  pinMode(pin_a, INPUT);
  pinMode(pin_b, INPUT);

  // read initial state
  state_a = digitalRead(pin_a);
  state_b = digitalRead(pin_b);

  // declare the interrupts
  attachInterrupt(pin_a, interrupt_a, CHANGE);
  attachInterrupt(pin_b, interrupt_b, CHANGE);
}

int rotary_encoder_position()
{
  return (position >> 2);
}
