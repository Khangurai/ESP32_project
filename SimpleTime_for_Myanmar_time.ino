#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the I2C address of the LCD display

const char* ssid       = "DECO B105";
const char* password   = "sein8995";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 23400;
const int   daylightOffset_sec = 0;

const char* time_zone = "MST+6:30,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S");Serial.println(" Myanmar Standard Time");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(timeinfo.tm_mday);
  lcd.print("/");
  lcd.print(timeinfo.tm_mon + 1);
  lcd.print("/");
  lcd.print(timeinfo.tm_year + 1900);
  lcd.setCursor(0, 1);
  lcd.print(timeinfo.tm_hour);
  lcd.print(":");
  lcd.print(timeinfo.tm_min);
  lcd.print(":");
  lcd.print(timeinfo.tm_sec);
  //lcd.print("MMT");
  }


// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup()
{
  Serial.begin(115200);

  lcd.begin();
  lcd.backlight();
  //lcd.print("Hello, world!");

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );

  /**
   * NTP server address could be aquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: configTime() function call if made AFTER DHCP-client run
   * will OVERRIDE aquired NTP server address
   */
  sntp_servermode_dhcp(1);    // (optional)

  /**
   * This will set configured ntp servers and constant TimeZone/daylightOffset
   * should be OK if your time zone does not need to adjust daylightOffset twice a year,
   * in such a case time adjustment won't be handled automagicaly.
   */
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  /**
   * A more convenient approach to handle TimeZones with daylightOffset 
   * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
   * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
   */
  //configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

}

void loop()
{
  delay(1000);
  printLocalTime();     // it will take some time to sync time :)

  
  
}
/*gmtOffset_sec myanamar

Myanmar is in the Myanmar Standard Time (MST) timezone, which has a GMT offset of 6 hours and 30 minutes. This means that Myanmar is 6 hours and 30 minutes ahead of Coordinated Universal Time (UTC).

This can be represented in seconds as 6.5 * 3600 = 23400.

This offset is used to calculate the local time in Myanmar from UTC. For example, if the UTC time is 12:00 PM, then the local time in Myanmar would be 6:30 PM.
This offset is also used to set the timezone on devices in Myanmar. For example, to set the timezone on an ESP32 to Myanmar, you would use the following code:
C++

configTzTime("MST+6:30,M3.5.0,M10.5.0/3", "pool.ntp.org", "time.nist.gov");

This code would set the timezone on the ESP32 to Myanmar and sync the time with the NTP server.
I hope this information is helpful.FROM GOOGLE BARD SOURCE*/

