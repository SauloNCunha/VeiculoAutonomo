    
#include "Adafruit_VL53L0X.h"     /*Biblioteca do laser */
#include <Wire.h>                 /*Biblioteca para declaração das portas SDA e SCL*/
#include <Servo.h>                /*Biblioteca do Servo Motor*/  
#include <Ultrasonic.h>           /*Biblioteca do do Ultrasonico*/
#include <ESP8266WiFi.h>          /*Biblioteca WIFI para ESP */
#include <PubSubClient.h>         /*Biblioteca do MQTT */

Servo servo; /*SERVO DA DIREÇÃO*/

#define velDiant 5 /*portas definidas pela shild para os motores (padrao)*/
#define dirDiant 0 /*portas definidas pela shild para os motores (padrao)*/ 
#define velTras  4 /*portas definidas pela shild para os motores (padrao)*/
#define dirTras  2 /*portas definidas pela shild para os motores (padrao)*/

/*WiFi*/
const char* SSID = "NotebookSaulo";                /* SSID / nome da rede WiFi que deseja se conectar*/
const char* PASSWORD = "Saulo1004";   /* Senha da rede WiFi que deseja se conectar*/
WiFiClient wifiClient; 
  
/*MQTT Server*/
const char* BROKER_MQTT = "192.168.137.1";/*"test.mosquitto.org"; *//*URL do broker MQTT que se deseja utilizar*/
int BROKER_PORT = 1883;                      /* Porta do Broker MQTT*/

#define ID_MQTT  "CARRO"      /*Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. */
#define STATUS "STATUS"       /*Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.*/
#define TEMPO "TEMPO"         /*Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.*/
#define DITANCIAC "DITANCIAC" /*Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.*/

PubSubClient MQTT(wifiClient); /* Instancia o Cliente MQTT passando o objeto espClient*/

String tempoS = " ";
String statsS = " ";
String distaS = " ";

const int TRIGGERdir = 14; /*TRIGGER DO ULTRASSONICO ESQUERDO*/
const int ECHOdir    = 10; /*ECHO DO ULTRASSONICO ESQUERDO*/

float esq = 0; /*CONTROLA DISTANCIA DA ESQUERDA*/
float fre = 0; /*CONTROLA DISTANCIA DA FRENTE*/
float disSem = 0;
float esc =0;

int minRange = 512;
int maxRange = 512;
int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;
int pos = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X(); /*DECLARAÇÃO DO LASER*/

/*Declaração das Funções*/
void mantemConexoes();  /*/Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas*/
void conectaWiFi();     /*/Faz conexão com WiFi*/
void conectaMQTT();     /*/Faz conexão com Broker MQTT*/
void recebePacote(char* topic, byte* payload, unsigned int length);
float distanciaLaser();

void setup(){
    Wire.begin(13,12); /*/DECLARA SCL E DEPOIS A SDA DO LASER*/
    Serial.begin(115200);
    
    /*-----------------------------------------*/
    conectaWiFi();
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
    MQTT.setCallback(recebePacote); 
    /*-----------------------------------------*/
   
    /*initial settings for motors off and direction forward*/
    servo.attach(16); /* servo está na porta 16*/
    servo.write(pos); /* aciona a porta do servo, zerando*/
 
    pinMode(velDiant, OUTPUT); /*declara saida dos pinos*/
    pinMode(velTras, OUTPUT);
    pinMode(dirDiant, OUTPUT);
    pinMode(dirTras, OUTPUT);
    digitalWrite(velDiant, LOW); /*declara motores desligados*/
    digitalWrite(velTras, LOW);
    digitalWrite(dirDiant, LOW);
    digitalWrite(dirTras,LOW);
  
    pinMode(TRIGGERdir, OUTPUT); /*DECLARA ULTRASSONICO COMO SAIDA*/
    pinMode(ECHOdir, INPUT);     /*DECLARA ULTRASSONICO COMO ENTRADA*/

    /* VERIFICA A PORTA USB CONECTADA*/
    while (! Serial) {
      delay(1);
    }

    Serial.println("Adafruit VL53L0X test");
    if (!lox.begin()) {
      Serial.println(F("Failed to boot VL53L0X"));
      while(1);
    }
    /* LIGA O LASER */
    Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 

}

void distancia() {
  
    long duration, distance;
    digitalWrite(TRIGGERdir, LOW);  
    delayMicroseconds(2); 
  
    digitalWrite(TRIGGERdir, HIGH);
    delayMicroseconds(10); 
  
    digitalWrite(TRIGGERdir, LOW);
    duration = pulseIn(ECHOdir, HIGH);
    esq = (duration/2) / 29.1;
    Serial.print("Distancia Utrassonico: ");
    Serial.println(esq);
  
    

    /*long duration, distance;
    digitalWrite(TRIGGERdir, LOW);  
    delayMicroseconds(2); 
  
    digitalWrite(TRIGGERdir, HIGH);
    delayMicroseconds(10); 
  
    digitalWrite(TRIGGERdir, LOW);
    duration = pulseIn(ECHOdir, HIGH);
    fre = (duration/2) / 29.1;*/
}


void pontomorto(){
  digitalWrite(velDiant, LOW);
  digitalWrite(velTras, LOW);
  digitalWrite(dirDiant, LOW);
  digitalWrite(dirTras,LOW);
}

void frente(){
  digitalWrite(velDiant, HIGH);
  digitalWrite(velTras, HIGH);
  digitalWrite(dirDiant, HIGH);
  digitalWrite(dirTras,HIGH);
}
/*
void re(){
  digitalWrite(dirDiant,LOW);
  digitalWrite(dirTras,LOW);
  analogWrite(velDiant,maxSpeed);
  analogWrite(velTras,maxSpeed);
}
*/
void esquerda(){
    int angulo = esc;
    for (angulo; angulo < 360; angulo += 1) { /* Comando que muda a posição do servo de 0 para 180°*/
      servo.write(angulo); /* Comando para angulo específico*/
      delay(15); 
    }
  
    delay(2000); /* Tempo de espera para próximo movimento*/
    for (angulo = 360; angulo >= 1; angulo -= 5) { /* Comando que muda a posição do servo de 180 para 0°*/
      servo.write(angulo); /* Comando para angulo específico*/
      delay(5);
    }
  
    delay(2000);
  
    /*
    /* Lê o valor do Potenciometro
    int angle = dir; 
    /* Mapeia o valor de 0 a 180 graus
    angle=map(angle, 0, 1023, 0, 180);
    /* Repassa o angulo ao ServoWrite
    servo.write(angle); 
    /* Delay de 15ms para o Servo alcançar a posição
    delay(15);
    */
}

void loop(){
   mantemConexoes();
  
   MQTT.loop();  

   fre = distanciaLaser();
   distancia();

  /*frente();
  delay(2000);
  
  re();
  delay(2000);
  pontomorto();
  delay(2000);*/
  /*esquerda();*/
  Serial.print("valor FRE: ");
  Serial.println(fre);
  if(fre <= 15 ) {
   pontomorto();
   Serial.print("Ponto Morto"); 
  }
  else{
  frente(); 
  
  /*re();*/
  
  }
}

boolean logicasemafaro(int statu, int tempo, float disSem){
  
}

float distanciaLaser(){
  /*-------------------------------*/
    /*Codigo do Laser*/
   
    VL53L0X_RangingMeasurementData_t measure;  
    Serial.print("Lendo a medida... ");
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus != 4) {
      Serial.print("Distancia laser (cm): "); 
      Serial.println((measure.RangeMilliMeter-40)/10);
      fre = (measure.RangeMilliMeter-40)/10;
    } else {
      Serial.println(" Fora de alcance! ");
      fre = 1;
    } 

    return fre;
    /*------------------------------- */
}
