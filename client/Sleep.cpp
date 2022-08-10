#include <Arduino.h>
#include <esp_sleep.h>

unsigned long lastActionMilli = 0;
unsigned long sleepDelayMilli = 5000;

void setup_sleep() {
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
}

void delay_sleep() {
  lastActionMilli = millis();
}

void check_sleep() {
  if ((millis() - lastActionMilli) > sleepDelayMilli) {
    if (gpio_get_level(GPIO_NUM_4)) {
      esp_deep_sleep_enable_gpio_wakeup(BIT(GPIO_NUM_4), ESP_GPIO_WAKEUP_GPIO_LOW);  
    } else {     
      esp_deep_sleep_enable_gpio_wakeup(BIT(GPIO_NUM_4), ESP_GPIO_WAKEUP_GPIO_HIGH);  
    }
    
    esp_deep_sleep_enable_gpio_wakeup(BIT(GPIO_NUM_5), ESP_GPIO_WAKEUP_GPIO_LOW);

    Serial.println("Going to sleep");
    esp_deep_sleep_start();
  }
}
