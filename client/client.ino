#include "Communication.h"
#include "Message.h"
#include "Rotary.h"

struct_message myMessage;

void on_clockwise(long int currentRotation) {
  Serial.print("Clockwise ");
  Serial.println(currentRotation);
  
  myMessage.action = 1;
  
  send_message(myMessage);
}

void on_counter_clockwise(long int currentRotation) {
  Serial.print("Counter clockwise ");
  Serial.println(currentRotation);
  
  myMessage.action = 2;
  
  send_message(myMessage);
}

void on_button(long int timesPressed) {
  Serial.print("Times pressed ");
  Serial.println(timesPressed);
  
  myMessage.action = 3;
  
  send_message(myMessage);
}

void setup(){
  Serial.begin(115200);
  
  setup_rotary(&on_clockwise,
               &on_counter_clockwise,
               &on_button);
               
  setup_communication();
}

void loop(){

}
  
