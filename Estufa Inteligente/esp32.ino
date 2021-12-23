#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Adafruit_Sensor.h>  
#include<SimpleDHT.h>
#include <WiFi.h> 
#include <WiFiClientSecure.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTPIN 14     
#define DHTTYPE DHT22


SimpleDHT22 dht;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int PinLamp = 0;
int PinHeat = 5;

int inLDR = 26;

float temperature, humidity;
int luminosity;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin("SUA REDE", "SUA SENHA");
    
  delay(2000);
 
  pinMode(PinLamp, OUTPUT);
  pinMode(PinHeat, OUTPUT);

  pinMode(inLDR, INPUT); 
}

void readDHTSensor() {
  float t, h;

  delay(2000);
  
  int status = dht.read2(DHTPIN, &t, &h, NULL);
 
  if (status == SimpleDHTErrSuccess) {
  
    temperature = t;
    humidity = h;
  }
}

void readLDRSensor() {
  int ldr_read = 0;
  
  for(int i=0; i<10; i++){
    delay(500);
    ldr_read += analogRead(inLDR);
  }

  luminosity = ldr_read / 10;
}

void ControlTemp(){

  float Reference_Temp = 35;
  
  if(temperature < Reference_Temp){
    digitalWrite(PinHeat, LOW);
  }
  else{
    digitalWrite(PinHeat, HIGH);
  }
}

void ControlLumi(){

  int Reference_Lumi = 400;
  
  if(luminosity < Reference_Lumi){
    digitalWrite(PinLamp, LOW);
  }
  else{
    digitalWrite(PinLamp, HIGH);
  }
}

void ControlData(){
  
  WiFiClientSecure client;
  String tempFix = "GET /forms/d/e/1FAIpQLSdm6M_0mTVx_LKHLB1J3u_hjaag_hBtMfDHQlTIKe0EoatfsQ/formResponse?ifq&entry.XXXXXX=";
  String HumiFix = "GET /forms/d/e/1FAIpQLSdm6M_0mTVx_LKHLB1J3u_hjaag_hBtMfDHQlTIKe0EoatfsQ/formResponse?ifq&entry.XXXXXX=";
  String LumiFix = "GET /forms/d/e/1FAIpQLSdm6M_0mTVx_LKHLB1J3u_hjaag_hBtMfDHQlTIKe0EoatfsQ/formResponse?ifq&entry.XXXXXX=";

  if (client.connect("docs.google.com", 443) == 1){
    String toSend = tempFix;
    toSend += temperature;
    toSend += "&submit=Submit HTTP/1.1";
        
    toSend += HumiFix;
    toSend += humidity;
    toSend += "&submit=Submit HTTP/1.1";
        
    toSend += LumiFix;
    toSend += luminosity;
    toSend += "&submit=Submit HTTP/1.1";
    client.println(toSend);
    client.println("Host: docs.google.com");//-
    client.println();//-
        
    client.stop();//Encerramos a conexao com o servidor
    Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
  }
  else {
    Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
  }
}
    


void printData(){
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");
  Serial.print("Luminosity: ");
  Serial.print(luminosity);

}

void loop() {
 
  readDHTSensor();
  readLDRSensor();

  ControlTemp();
  ControlLumi();

  ControlData();

  printData();
}
