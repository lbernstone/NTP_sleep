/*
 Deep sleep with periodic NTP update
 This example stores a variable in the RTC memory
 to keep track of the last time NTP was checked
 Larry Bernstone <lbernstone@gmail.com>
*/
 
#include <WiFi.h>
#include <esp_sleep.h>

#define NTP_CYCLE 3600  // one hour
#define NTP_TIMEOUT 30  // seconds
#define TIME_TO_SLEEP 30  // seconds
#define NTP_SRV "time-a-b.nist.gov"

RTC_DATA_ATTR unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  unsigned long now = time(NULL);
  Serial.printf("Now: %lu\nLast Update: %lu\n", now, lastUpdate);
  if (lastUpdate > now) lastUpdate = 0;

  if ((lastUpdate == 0) || ((now - lastUpdate) > NTP_CYCLE)) { 
    WiFi.begin("ssid","passwd");
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Unable to connect to WiFi");
      return;
    }

    struct tm tm_now;
    configTime(0, 0, NTP_SRV); //UTC time
    if (!getLocalTime(&tm_now, NTP_TIMEOUT * 1000ULL)) {
      Serial.println("Unable to sync with NTP server");
      return;
    }
    lastUpdate = time(NULL);
    Serial.println("NTP time updated");
  }
}

void loop() {  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000ULL);
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
}
