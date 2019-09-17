#include "Adafruit_VL53L0X.h"     /*Biblioteca do laser */
#include <Wire.h>                 /*Biblioteca para declaração das portas SDA e SCL*/
#include <Servo.h>                /*Biblioteca do Servo Motor*/  
#include <Ultrasonic.h>           /*Biblioteca do do Ultrasonico*/
#include <ESP8266WiFi.h>          /*Biblioteca WIFI para ESP */
#include <PubSubClient.h>         /*Biblioteca do MQTT */

//WiFi
const char* SSID ="Adriana"; /*"MostraProfissoes";       */         /* SSID / nome da rede WiFi que deseja se conectar*/
const char* PASSWORD = "123oliv456";/*"profissoes"; */  /* Senha da rede WiFi que deseja se conectar*/
WiFiClient wifiClient; 
  
//MQTT Server
const char* BROKER_MQTT ="broker.hivemq.com";/*"192.168.137.1";//"test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar*/
int BROKER_PORT = 1883;               /*       // Porta do Broker MQTT*/

#define ID_MQTT  "CARRO"      /*Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. */
#define STATUS "STATUS"       /*Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.*/
#define TEMPO "TEMPO"         /*Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.*/
#define DISTANCIA "DISTANCIA" /*Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.*/

PubSubClient MQTT(wifiClient); /* Instancia o Cliente MQTT passando o objeto espClient*/

Servo servo; /*SERVO DA DIREÇÃO*/

#define velDiant 5 /*portas definidas pela shild para os motores (padrao)*/
#define dirDiant 0 /*portas definidas pela shild para os motores (padrao)*/ 
#define velTras  4 /*portas definidas pela shild para os motores (padrao)*/
#define dirTras  2 /*portas definidas pela shild para os motores (padrao)*/

const int TRIGGERdir = 14; /*TRIGGER DO ULTRASSONICO ESQUERDO*/
const int ECHOdir    = 10; /*ECHO DO ULTRASSONICO ESQUERDO*/

String tempoS = " ";
String statsS = " ";
String distaS = " ";
float esq = 0; /*CONTROLA DISTANCIA DA ESQUERDA*/
float fre = 0; /*CONTROLA DISTANCIA DA FRENTE*/
float disSem = 20;
int tempoSem = 15;
int statusSem = 2;
float esc =0;

int minRange = 512;
int maxRange = 512;
int minSpeed = 50;
int maxSpeed = 100;/*1020;*/
int noSpeed = 0;
int pos = 90; /*SERVO SÓ GIRA 180*/


Adafruit_VL53L0X lox = Adafruit_VL53L0X(); /*DECLARAÇÃO DO LASER*/


//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT
void recebePacote(char* topic, byte* payload, unsigned int length);
float distanciaLaser();

int recebeTempo(String tempo);
int recebeStatus(String statu);
float  recebeDistancia(String distanciac);

void pontomorto();
void frente();
void re();
void esquerda();
void direita();
void distancia() ;


void setup() {
  Wire.begin(13,12); /*/DECLARA SCL E DEPOIS A SDA DO LASER*/
  Serial.begin(115200);        

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  MQTT.setCallback(recebePacote); 

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

void loop() {
  mantemConexoes();
  MQTT.loop();

  

   /**
   distancia();

  frente();
  /*delay(2000);
  
  re();
  delay(2000);
  pontomorto();
  delay(2000);*/
  /*esquerda();*/
   
  
     distancia();
  
   if((esq >= 4.5) &&(esq <= 5.5 )) {
     reto();
   }else if (esq >= 5.6 ){
      esquerda();    
    }else if(esq <= 4.4){
      direita();
    }

    if (fre >= 15) {
      if ((statusSem = 2) or (statusSem = 3)){
        if (disSem >60){
           frente();
        }else{
          pontomorto();
        }
        
      }else if ((statusSem = 1) && (disSem >30)){
         frente();
      }else {
        pontomorto();
      }    
     
    } else {
      pontomorto();
    }

    
  
  /*if(fre <= 15 ) {
   pontomorto();
   Serial.print("Ponto Morto"); 
  }
  else{
  frente(); 
  
  re();*/
  
  
}
