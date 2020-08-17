#include <WiFi.h>
#include <esp_sleep.h>

#define NTP_CYCLE 3600  // one hour
#define NTP_TIMEOUT 30  // 30 seconds
#define TIME_TO_SLEEP 3  // five minutes
#define NTP_SRV "time-a-b.nist.gov"
#define NOW time(NULL)

RTC_DATA_ATTR unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  Serial.printf("Now: %lu\nLast Update: %lu\n", NOW, lastUpdate);
  if (lastUpdate > NOW) lastUpdate = 0;
  if ((lastUpdate == 0) || ((NOW - lastUpdate) > NTP_CYCLE)) { 

    WiFi.begin("larryb","clownfish");
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Unable to connect to WiFi");
      return;
    }

    struct tm now;
    configTime(0, 0, NTP_SRV); //UTC time
    if (!getLocalTime(&now, NTP_TIMEOUT * 1000ULL)) {
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
