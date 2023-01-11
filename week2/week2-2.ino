#include <WiFi.h>
#include "secret.h"
#include "time.h"
#include "esp_sntp.h"


// contants for SNTP client
const char* ntpServer = "nl.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

char* getLocalTime()
{
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/apireference/system/systime_t now;
  char *time_string = (char*)malloc(64 * sizeof(char));
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(time_string, (64 * sizeof(char)), "%c", &timeinfo);
  //example on how to print the returned time_string
  //printf("\r\nThe current date/time is: %s\r\n", time_string);
  return time_string;
}


void WiFi_connect(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      printf(".");
  }
  printf(" CONNECTED\r\n");
}

void setup(){
  printf("Starting");
  WiFi_connect();
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
  setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
  tzset();
}

void loop(){
}
