#include <ESP_Mail_Client.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>

const char* ssid     = "REP ZIKA";
const char* password = "repzika13";

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "Viniciustesteesp32@gmail.com"
#define AUTHOR_PASSWORD "TesteESP32@"
#define RECIPIENT_EMAIL "viniciusvale73@gmail.com"

double b, c, e, d, f;

Adafruit_MPU6050 mpu; 
WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)

SMTPSession smtp;

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


        b = 0;
        e = 0;
        c = 0;
        d = 0;
        f = 0;
        b = a.acceleration.x;
        e = a.acceleration.z;
        c = a.acceleration.y;
        d = (b*b)+(e*e)+(c*c);
        f = sqrt(d);

         if (true){ 
            
            smtp.debug(1);
            
            /* Declare the session config data */
            ESP_Mail_Session session;
                        
            /* Set the session config */
            session.server.host_name = SMTP_HOST;
            session.server.port = SMTP_PORT;
            session.login.email = AUTHOR_EMAIL;
            session.login.password = AUTHOR_PASSWORD;
            session.login.user_domain = "";

            /* Declare the message class */
            SMTP_Message message;
          
            /* Set the message headers */
            message.sender.name = "ESP";
            message.sender.email = AUTHOR_EMAIL;
            message.subject = "[ATENÇÃO!] POSSÍVEL QUEDA";
            message.addRecipient("Vinicius", RECIPIENT_EMAIL);
          
            String textMsg = "[ATENÇÃO!] POSSÍVEL QUEDA DETECTADA";
            message.text.content = textMsg.c_str();
            message.text.charSet = "us-ascii";
            message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
            
          if (!MailClient.sendMail(&smtp, &message))
          {

           Serial.println("Error sending Email, " + smtp.errorReason());
                  
         }
      }
   delay(5000);
   return;

}
