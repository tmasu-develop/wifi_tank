#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <FS.h>
//#include <I2CLiquidCrystal.h>

#define LED_PIN     16
#define USE_SERIAL Serial

// モーター制御PIN
#define APHASE  12
#define AENABLE 13
#define BPHASE  14
#define BENABLE 15

// AP設定
char ssid[16];
const char* password = "1234567890";

ESP8266WebServer server = ESP8266WebServer(80);
IPAddress ip( 192, 168, 10, 1 );
IPAddress subnet( 255, 255, 255, 0 );
WebSocketsServer webSocket = WebSocketsServer(81);

//SPIFFSから読み込むWebページ
const char* html_file_name = "/wifitank.html";

//I2CLiquidCrystal lcd((uint8_t)0x3e, (uint8_t)30);

        uint8_t l_up;
        uint8_t r_up;
        uint8_t l_down;
        uint8_t r_down;

// WebSocketイベントハンドラ
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

      // send message to client
      webSocket.sendTXT(num, "Connected");
      break;
    }
    case WStype_TEXT:
      USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
      if(payload[0] == '#') { 

        uint32_t button = (uint32_t) strtol((const char *) &payload[1], NULL, 2);

        l_up = (button & 8) != 0;
        r_up = (button & 4) != 0;
        l_down = (button & 2) != 0;
        r_down = (button & 1) != 0;

        if((l_up) && (l_down)){
          l_down = 0;
        }

        if((r_up) && (r_down)){
          r_down = 0;
        }
        
        USE_SERIAL.printf("[%u]:[%u]:[%u]:[%u]\n", l_up, r_up, l_down, r_down);
        
/*
                // we get RGB data

                // decode rgb data
                uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);

                analogWrite(LED_RED,    ((rgb >> 16) & 0xFF));
                analogWrite(LED_GREEN,  ((rgb >> 8) & 0xFF));
                analogWrite(LED_BLUE,   ((rgb >> 0) & 0xFF));
*/
       }
       break;
    }
}

// SPIFFSからファイル読み込み
bool handleFileRead(String path){
  USE_SERIAL.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  String contentType = "text/html";
  if(SPIFFS.exists(path)){
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

// mode0
// モーターA初期化
void motor_a_init(){
  pinMode(APHASE,OUTPUT);
  pinMode(AENABLE,OUTPUT);
}

// mode0
// モーターB初期化
void motor_b_init(){
  pinMode(BPHASE,OUTPUT);
  pinMode(BENABLE,OUTPUT);
}

// mode0
// モーターA停止
void motor_a_stop(){
  digitalWrite(AENABLE,0);
  digitalWrite(APHASE,LOW);
}

// mode0
// モーターB停止
void motor_b_stop(){
  digitalWrite(BENABLE,0);
  digitalWrite(BPHASE,LOW);
}

// mode0
// モーターA前進
void motor_a_forward(){
 digitalWrite(AENABLE,HIGH);
 digitalWrite(APHASE,LOW);
}

// mode0
// モーターB前進
void motor_b_forward(){
 digitalWrite(BENABLE,HIGH);
 digitalWrite(BPHASE,LOW);
}

// mode0
// モーターA後退
void motor_a_reverse(){
 digitalWrite(AENABLE,LOW);
 digitalWrite(APHASE,HIGH);
}

// mode0
// モーターB後退
void motor_b_reverse(){
 digitalWrite(BENABLE,LOW);
 digitalWrite(BPHASE,HIGH);
}

void setup() {
  // put your setup code here, to run once:
  // LED initialize
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // モーター初期化
  motor_a_init();
  motor_b_init();

  // start Wi-fi AP
  sprintf(ssid, "esp8266-%06x", ESP.getChipId());
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);  
  WiFi.softAP(ssid, password);
  
  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for(uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  if(MDNS.begin("esp8266")) {
    USE_SERIAL.println("MDNS responder started");
  }

  SPIFFS.begin();
  server.on("/", [](){
    if(!handleFileRead(html_file_name)) server.send(404, "text/plain", "FileNotFound");
  });
  server.begin();

  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);

  USE_SERIAL.println(ip);

/*
  String ipstring = address(ip);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ipstring);
  lcd.setCursor(0, 1);
  lcd.print(ipstring.substring(8));
*/
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  server.handleClient();

if((l_up == 0) && (l_down == 0)){
  motor_a_stop();
}
if((r_up == 0) && (r_down == 0)){
  motor_b_stop();
}

if(l_up){
  motor_a_forward();
}
if(r_up){
  motor_b_forward();
}
if(l_down){
  motor_a_reverse();
}
if(r_down){
  motor_b_reverse();
}


}

/*
String address(IPAddress ip){
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  return ipStr;
}
*/



