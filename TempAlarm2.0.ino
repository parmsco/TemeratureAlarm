#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <IFTTTMaker.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define Max_Temp 77 //Set maximum temperature value [F]
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 14
#define BMP_CS 27
int TempF = 1;
int TempC = 1;
String Temp = "00";

Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

char ssid[] = "csu-guest";       // your network SSID (name)
//char password[] = NULL;  // your network key
#define KEY "bXMZmESe9ijMpzjVz7ThRo"  // Get it from this page https://ifttt.com/services/maker/settings
#define EVENT_NAME "TemperatureAlarm" // Name of your event name, set when you are creating the applet



WiFiClientSecure client;
IFTTTMaker ifttt(KEY, client);
void setup() {
  Serial.begin(115200);

  if (!bme.begin()) {
Serial.println("Could not find a valid BMP280 sensor, check wiring!");
while (1);
}

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  TempC = bme.readTemperature();
  TempF = 1.8 * TempC + 23; //seems to read high
Serial.print("Temperature = ");
Serial.print(TempF);
Serial.println(" *F");

  if (TempF >= Max_Temp) {
    Temp=TempF;
  if(ifttt.triggerEvent(EVENT_NAME, Temp)){
    Serial.println("Successfully sent");
    Serial.print(Temp);
  } else
  {
    Serial.println("Failed!");
  }
  }
  delay(3000);
}



