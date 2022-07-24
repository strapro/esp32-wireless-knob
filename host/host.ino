#include <WiFi.h>
#include <esp_now.h>

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
}
 
void setup(){
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(dataReceiveCallback);
}
 
void loop(){

}
