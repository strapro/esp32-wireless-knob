#include <Arduino.h>

#define ROTARY_CLK 2
#define ROTARY_DT 4
#define ROTARY_SW 6

uint8_t state=0;

long int rotValue=0, 
         swValue=0;

void (*clockwise_callback)(long int), 
     (*counter_clockwise_callback)(long int), 
     (*button_callback)(long int);

portMUX_TYPE gpioMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR interrupt_rotation() {
  portENTER_CRITICAL_ISR(&gpioMux);
  
  uint8_t s = state & 3;

  if (digitalRead(ROTARY_CLK)) s |= 8;
  if (digitalRead(ROTARY_DT)) s |= 4;

  switch (s) {
    case 1: case 14:
      rotValue++;        
      clockwise_callback(rotValue);
      break;
    case 2: case 13:
      rotValue--;       
      counter_clockwise_callback(rotValue);
      break;
    default:
      break;
  }
  
  state = (s >> 2);
  
  portEXIT_CRITICAL_ISR(&gpioMux);
}


void IRAM_ATTR interrupt_switch() {
  portENTER_CRITICAL_ISR(&gpioMux);

  swValue++;    
  button_callback(swValue);
  
  portEXIT_CRITICAL_ISR(&gpioMux);
}

void setup_rotary(void (*clockwise_cb)(long int), 
                  void (*counter_clockwise_cb)(long int), 
                  void (*button_cb)(long int)) {
  clockwise_callback = clockwise_cb;
  counter_clockwise_callback = counter_clockwise_cb;
  button_callback = button_cb;
  
  pinMode(ROTARY_CLK, INPUT_PULLUP);
  pinMode(ROTARY_DT, INPUT_PULLUP);
  pinMode(ROTARY_SW, INPUT_PULLUP);

  attachInterrupt(ROTARY_CLK, interrupt_rotation, CHANGE);
  attachInterrupt(ROTARY_DT, interrupt_rotation, CHANGE);
  attachInterrupt(ROTARY_SW, interrupt_switch, CHANGE);
}
 
