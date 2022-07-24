#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Message.h"

uint8_t broadcast_address[] = {0x44, 0x17, 0x93, 0x8F, 0x4E, 0x38};

esp_now_peer_info_t peer_info;

void (*datasent_callback)(bool);

void datasent_callback_local(const uint8_t *mac_addr, esp_now_send_status_t status) {
  datasent_callback(status == ESP_NOW_SEND_SUCCESS);
}

void send_message(struct_message message) {
  esp_now_send(broadcast_address, (uint8_t *) &message, sizeof(message));
}

void setup_communication(void (*datasent_cb)(bool)) {
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peer_info.peer_addr, broadcast_address, 6);
  
  peer_info.channel = 0;  
  peer_info.encrypt = false;

  if (esp_now_add_peer(&peer_info) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  datasent_callback = datasent_cb;

  esp_now_register_send_cb(datasent_callback_local);
}
