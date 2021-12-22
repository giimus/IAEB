#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>


const char* ssid     = "REP ZIKA";
const char* password = "repzika13";
double b, c, e, d, f;

Adafruit_MPU6050 mpu; 
WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)


void setup() {

  Wire.begin(5, 4); //Pinos do sensor
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); //Habilita o modo estaçao
  WiFi.begin(ssid, password); //Conecta na rede
  while (WiFi.status() != WL_CONNECTED) {
        delay(150);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    
  delay(500);//Espera um tempo para se conectar no WiFi

  while (!Serial){
    delay(10);
    }

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

}

void loop() {
  
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);
client.setInsecure();

    if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
    {
        String toSend = "GET /forms/d/e/1FAIpQLSfvR0-ScJrAhnyermKsnVMbqoSQj3EVXXLMg1W3sCty5kFicg/formResponse?ifq&entry.1190874150=";
        b = a.acceleration.x;
        e = a.acceleration.z;
        c = a.acceleration.y;
        d = (b*b)+(e*e)+(c*c);
        f = sqrt(d);
        toSend += b;
        toSend += "&entry.1683875726=";
        toSend += e; 
        toSend += "&entry.1299302089=";
        toSend += c;
        toSend += "&entry.1169950638=";
        toSend += f; 
        toSend += "&submit=Submit HTTP/1.1"; //Completamos o metodo GET para nosso formulario.
        client.println(toSend); //Enviamos o GET ao servidor-
        client.println("Host: docs.google.com"); //-
        client.println();//-
        client.stop();//Encerramos a conexao com o servidor
        Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
    }
    else
    {
        Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
    }
    delay(500);


}
