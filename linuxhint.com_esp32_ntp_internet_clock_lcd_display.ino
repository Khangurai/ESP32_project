//https://linuxhint.com/esp32-ntp-internet-clock-lcd-display/

#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>          
#include <TimeLib.h>                  
int lcd_Columns = 16;  /*define LCD size*/
int lcd_Rows = 2;
LiquidCrystal_I2C lcd(0x27, lcd_Columns, lcd_Rows);    /*0x27 I2C Address for LCD*/
const char *ssid     = "DECO B105";    /*Replace with your network SSID*/
const char *password = "sein8995";  /*Replace with network password*/
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", 23400, 60000);
char Time[ ] = "00:00:00(MST)";
char Date[ ] = "00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
void setup(){
  Serial.begin(115200);
  lcd.begin(); /*Initialize LCD display*/
  lcd.backlight();  /*ON LCD Backlight*/
  //lcd.setCursor(0, 0);  /*Set cursor*/
  //lcd.print("Time");  /*print time on LCD*/
  //lcd.setCursor(0, 1);  /*Set LCD cursor*/
  //lcd.print(Date);  /*Print date*/
  WiFi.begin(ssid, password);  /*begin WiFi*/
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  timeClient.begin();
  delay(1000);
  lcd.clear();  /*clear LCD display*/
}
void loop(){
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
  second_ = second(unix_epoch);
  if (last_second != second_) {
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
    Time[7] = second_ % 10 + 48;
    Time[6] = second_ / 10 + 48;
    Time[4]  = minute_ % 10 + 48;
    Time[3]  = minute_ / 10 + 48;
    Time[1]  = hour_   % 10 + 48;
    Time[0]  = hour_   / 10 + 48;
    Date[0]  = day_   / 10 + 48;
    Date[1]  = day_   % 10 + 48;
    Date[3]  = month_  / 10 + 48;
    Date[4]  = month_  % 10 + 48;
    Date[8] = (year_   / 10) % 10 + 48;
    Date[9] = year_   % 10 % 10 + 48;
    Serial.print(String(Time)+" (Myanmar Standard Time) ");  /*Prints time on serial monitor*/
    Serial.println(Date);  /*Print date on serial monitor*/
    lcd.setCursor(0, 0);  /*Set LCD cursor*/
    lcd.print(Time);  /*display time on LCD*/
    lcd.setCursor(0, 1); /*Set LCD cursor*/
    lcd.print(Date);  /*Display date on LCD*/
    last_second = second_;
  }
  delay(200);
}