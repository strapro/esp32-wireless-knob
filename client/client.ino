#include "Message.h"
#include "Communication.h"
#include "Rotary.h"
#include "Sleep.h"

struct_message my_message;

long int current_rotation = 0,
         times_pressed = 0;

void on_clockwise(long int rotation) {
  current_rotation = rotation;
  
  my_message.action = 1;
  
  send_message(my_message);
  delay_sleep();
}

void on_counter_clockwise(long int rotation) {
  current_rotation = rotation;
  
  my_message.action = 2;
  
  send_message(my_message);
  delay_sleep();
}

void on_button(long int pressed) {
  times_pressed = pressed;
  
  my_message.action = 3;
  
  send_message(my_message);
  delay_sleep();
}

void on_data_sent(bool status) {
  Serial.print("Rotation: ");
  Serial.println(current_rotation);

  Serial.print("Times pressed: ");
  Serial.println(times_pressed);

  Serial.print("Action sent: ");
  Serial.println(my_message.action);

  Serial.print("Sent status: ");
  Serial.println(status);

  Serial.println("----------------------------------");
}


void setup(){
  Serial.begin(115200);
  
  setup_rotary(&on_clockwise,
               &on_counter_clockwise,
               &on_button);
               
  setup_communication(&on_data_sent);

  setup_sleep();
}

void loop(){
  check_sleep();
}
  
