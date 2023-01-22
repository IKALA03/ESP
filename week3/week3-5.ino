#include <WiFi.h>
#include "secret.h"
#include "time.h"
#include "esp_sntp.h

const char *ntpServer = "nl.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

void WiFi_connect()
{
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        printf(".");
    }
    printf(" CONNECTED\r\n");
}

char *localTime()
{
    systime_t now;
    char *t_string = (char *)malloc(64 * sizeof(char));
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(t_string, (64 * sizeof(char)), "%c", &timeinfo);
    return t_string;
}

void setup()
{
    WiFi_connect();

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
    tzset();
}

void loop()
{
    xTaskCreate(WiFi_connect, "WiFi_connect", 2048, NULL, 1, NULL);
    xTaskCreate(localTime, "getLocalTime", 2048, NULL, 2, NULL);
}