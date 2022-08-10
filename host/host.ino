#include <USB.h>
#include <USBHIDKeyboard.h>
#include <WiFi.h>
#include <esp_now.h>

#define KEY_ENTER 0xB0 // Keyboard Return (ENTER)
#define KEY_DOWN 0xD9 // Keyboard Down Arrow
#define KEY_UP 0xDA // Keyboard Up Arrow

USBHIDKeyboard Keyboard;

typedef struct struct_message {
  int action;  
} struct_message;


struct_message myMessage;

void dataReceiveCallback(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myMessage, incomingData, sizeof(myMessage));
  
  Serial.print("Bytes received: ");
  Serial.println(len);
  
  Serial.print("Action: ");
  Serial.println(myMessage.action);  

  if (myMessage.action == 1) {
    Keyboard.press(KEY_DOWN);
    Keyboard.release(KEY_DOWN);
  }
  else if (myMessage.action == 2) {
    Keyboard.press(KEY_UP);
    Keyboard.release(KEY_UP);
  }
  else if (myMessage.action == 3) {
    Keyboard.press(KEY_ENTER);
    Keyboard.release(KEY_ENTER);
  } 
}
 
void setup(){
  Serial.begin(115200);

  Keyboard.begin();
  USB.begin();
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(dataReceiveCallback);
}
 
void loop(){
}
