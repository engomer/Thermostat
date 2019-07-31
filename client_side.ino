#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

//scl-> d1 sda->d2

//WIFI SETUP
const char* host = "192.168.4.1";
const char* ssid = "Thermostat";
const char* password = "omergthermostat";

//LCD SETUP
LiquidCrystal_I2C lcd(0x27, 16, 2);

//DHT SETUP
#define DHTPIN 12 //d6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//POT SETUP

int potPin = 17;


void print2Screen(String a, float temp, float pot);
void print2Screen(String a);

void print2Screen(float temp, int pot)
{
  lcd.clear();
  lcd.home();
  lcd.print("  Oda : ");
  lcd.print(temp,1);
  lcd.setCursor(0,1);
  lcd.print(" Istenen : ");
  lcd.print(pot);
}

void print2Screen(String a)
{
  lcd.clear();
  lcd.home();
  lcd.print(a);
}


void setup() {
  pinMode(A0, INPUT);
  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,1);
  lcd.print("Merhaba");
  delay(2000);
  
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to base");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  print2Screen("Wifi Baglandi");

  
}

void loop() {
  WiFiClient client;
 
  if(!client.connect(host,80))
  {
    Serial.print(".");
    delay(500);
    return;
  }
  if(client.connected())
  {
    Serial.println("Bağlandı");
  }
   float temp;
  temp = dht.readTemperature();
  Serial.println(temp);

  int potVal;
  potVal = analogRead(A0);
  int potVall = map(potVal,0,1023,18,32);

  Serial.println(potVall);

  
  print2Screen((int)temp, potVall);

  if(potVall > (int)temp)
  {
   String url = "/on";
   client.print("GET " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("active");
  
  client.stop();
  delay(500);
  }
  if(potVall <= (int)temp)
  {
    String url = "/off";
    client.print("GET " + url +" HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("deactive");
  client.stop();
  delay(500);
  }
 

}
