#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>


#ifndef SSSID

#define SSSID "Thermostat"
#define PWD "omergthermostat"
#endif 

const char *ssid = SSSID;
const char *password = PWD;

ESP8266WebServer server(80);

void set1On()
{
  
  digitalWrite(1, HIGH); //tx
  
  digitalWrite(0, HIGH);
  digitalWrite(2, LOW);

  String s = "\r\n\r\n <!DOCTYPE HTML>\r\n<html>";
  s += "1 is on</html>\r\n\r\n";

  server.send(200, "text/html", s);
  Serial.println("1 on");
}

void set1Off()
{
  
  digitalWrite(1, LOW);
  digitalWrite(2, HIGH);
  digitalWrite(0, LOW);

  String s = "\r\n\r\n <!DOCTYPE HTML>\r\n<html>";
  s += "1 is off</html>\r\n\r\n";

  server.send(200, "text/html", s);
  Serial.println("1 off");
}

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Selam");
  delay(5000);

  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  
  digitalWrite(1, LOW);
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);

 
  WiFi.softAP(ssid, password);

  IPAddress myIp = WiFi.softAPIP();
  Serial.print("IP Address: ");
  Serial.print(myIp);
  server.on("/", handleRoot);
  server.on("/on", set1On);
  server.on("/off", set1Off);
  
  server.begin();

  Serial.println("Server started");



}

void loop() {
  
  server.handleClient();
  

}
