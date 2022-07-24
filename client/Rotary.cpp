#include <Arduino.h>

#define ROTARY_CLK 2
#define ROTARY_DT 4
#define ROTARY_SW 6

uint8_t state=0;

long int rot_value=0, 
         sw_value=0;

void (*clockwise_callback)(long int), 
     (*counter_clockwise_callback)(long int), 
     (*button_callback)(long int);

portMUX_TYPE gpio_mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR interrupt_rotation() {
  portENTER_CRITICAL_ISR(&gpio_mux);
  
  uint8_t s = state & 3;

  if (digitalRead(ROTARY_CLK)) s |= 8;
  if (digitalRead(ROTARY_DT)) s |= 4;

  switch (s) {
    case 1: case 14:
      rot_value++;        
      clockwise_callback(rot_value);
      break;
    case 2: case 13:
      rot_value--;       
      counter_clockwise_callback(rot_value);
      break;
    default:
      break;
  }
  
  state = (s >> 2);
  
  portEXIT_CRITICAL_ISR(&gpio_mux);
}


void IRAM_ATTR interrupt_switch() {
  portENTER_CRITICAL_ISR(&gpio_mux);

  sw_value++;    
  button_callback(sw_value);
  
  portEXIT_CRITICAL_ISR(&gpio_mux);
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
 
