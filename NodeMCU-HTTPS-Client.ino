#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid     = "SERT_Soundcar";         //อย่าลืมแก้เป็นชื่อ SSID  ของตัวเอง
const char* password = "23242700"; //อย่าลืมแก้เป็นชื่อ password ของตัวเอง

#define SERVER_PORT 80

WiFiServer server(SERVER_PORT);     //เปิดใช้งาน TCP Port 80
WiFiClient client1;              //ประกาศใช้  client 
WiFiClientSecure client;
 
unsigned long previousMillis = 0;       //กำหนดตัวแปรเก็บค่า เวลาสุดท้ายที่ทำงาน    
const long interval = 10000;            //กำหนดค่าตัวแปร ให้ทำงานทุกๆ 10 วินาที

const char* host1 = "benalman.com";
const char* host = "www.okcoin.com";
const char* fingerprint = "89 3C E7 F0 60 42 8F 47 C1 FC 70 97 90 11 65 E5 0E 76 74 AE";
const int httpsPort = 443;

void setup() {

    Serial.begin(115200);
    Serial.println();
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED)
{
            delay(500);
            Serial.print(".");
    }
    Serial.println(""); 
    Serial.println("WiFi connected"); 
    Serial.println("IP address: ");   
    Serial.println(WiFi.localIP());
}

void loop() {
  while(client.available())      
    {
          String line = client.readStringUntil('\n');
          Serial.println(line);
           StaticJsonBuffer<500> jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(line);

            if (!root.success()) {  
          return;
  }
  int okcoin = root["ticker"]["last"];

  Serial.println("okcoin");
  Serial.println(okcoin);
    }
    while(client1.available())    
    {
          String line1 = client1.readStringUntil('\n');
           StaticJsonBuffer<500> jsonBuffer1;
            JsonObject& root1 = jsonBuffer1.parseObject(line1);

            if (!root1.success()) {
          return;
  }
  int buy = root1["contents"]["quote"]["ask"];
  int sell = root1["contents"]["quote"]["bid"];

  Serial.println("buy");
  Serial.println(buy);
  Serial.println("sell");
  Serial.println(sell);
    }
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval)
  {
        previousMillis = currentMillis;
        Client_Request();
        Client_Request1();
  }   
    
}
 
void Client_Request()
{
  if (!client1.connect(host1, SERVER_PORT)) {
    Serial.println("connection failed");
    return;
  }

  client1.println("GET /code/projects/php-simple-proxy/ba-simple-proxy.php?url=https%3A%2F%2Fcoins.co.th%2Fapi%2Fv1%2Fquote HTTP/1.1\r\nHost: benalman.com");
  client1.println("Connection: close");
  client1.println();
  delay(100);
}

void Client_Request1()
{
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = "/api/v1/future_ticker.do?symbol=btc_usd&contract_type=quarter";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0\r\n" +
               "Connection: close\r\n\r\n");
  delay(100);
}

