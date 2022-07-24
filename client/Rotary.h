#ifndef ROTARY_DEFINED
#define ROTARY_DEFINED

void setup_rotary(void (*clockwise_cb)(long int), 
                  void (*counter_clockwise_cb)(long int), 
                  void (*button_cb)(long int));

#endif
