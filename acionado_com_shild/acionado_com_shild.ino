#include "Adafruit_VL53L0X.h"     /*Biblioteca do laser */
#include <Wire.h>                 /*Biblioteca para declaração das portas SDA e SCL*/
#include <Servo.h>                /*Biblioteca do Servo Motor*/  
#include <Ultrasonic.h>           /*Biblioteca do do Ultrasonico*/
#include <ESP8266WiFi.h>          /*Biblioteca WIFI para ESP */
#include <PubSubClient.h>         /*Biblioteca do MQTT */

Servo servo; //SERVO DA DIREÇÃO

#define velDiant 5 //portas definidas pela shild para os motores (padrao)
#define dirDiant 0 //portas definidas pela shild para os motores (padrao) 
#define velTras  4 //portas definidas pela shild para os motores (padrao)
#define dirTras  2 //portas definidas pela shild para os motores (padrao)

//WiFi
const char* SSID = "NotebookSaulo";                // SSID / nome da rede WiFi que deseja se conectar
const char* PASSWORD = "Saulo1004";   // Senha da rede WiFi que deseja se conectar
WiFiClient wifiClient; 
  
//MQTT Server
const char* BROKER_MQTT = "192.168.137.1";//"test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT

#define ID_MQTT  "CARRO"      //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define STATUS "STATUS"       //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define TEMPO "TEMPO"         //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define DITANCIAC "DITANCIAC" //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.

PubSubClient MQTT(wifiClient); // Instancia o Cliente MQTT passando o objeto espClient

String tempoS = " ";
String statsS = " ";
String distaS = " ";



const int TRIGGERdir = 14; //TRIGGER DO ULTRASSONICO ESQUERDO
const int ECHOdir    = 10; //ECHO DO ULTRASSONICO ESQUERDO

float esq = 0; //CONTROLA DISTANCIA DA ESQUERDA
float fre = 0; //CONTROLA DISTANCIA DA FRENTE
float disSem = 0;

int minRange = 512;
int maxRange = 512;
int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;
int pos = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X(); //DECLARAÇÃO DO LASER

//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT
void recebePacote(char* topic, byte* payload, unsigned int length);

void setup(){
  Wire.begin(13,12); //DECLARA SCL E DEPOIS A SDA DO LASER
  Serial.begin(115200);
  //_____________________________________________
  conectaWiFi();
   MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
   MQTT.setCallback(recebePacote); 
  //_____________________________________________
   
  // initial settings for motors off and direction forward
  servo.attach(16); // servo está na porta 16
  servo.write(pos); // aciona a porta do servo, zerando
 
  pinMode(velDiant, OUTPUT); //declara saida dos pinos
  pinMode(velTras, OUTPUT);
  pinMode(dirDiant, OUTPUT);
  pinMode(dirTras, OUTPUT);
  digitalWrite(velDiant, LOW); //declara motores desligados
  digitalWrite(velTras, LOW);
  digitalWrite(dirDiant, LOW);
  digitalWrite(dirTras,LOW);
 

  
  pinMode(TRIGGERdir, OUTPUT); //DECLARA ULTRASSONICO COMO SAIDA
  pinMode(ECHOdir, INPUT);     //DECLARA ULTRASSONICO COMO ENTRADA

  // VERIFICA A PORTA USB CONECTADA
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // LIGA O LASER 
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
  
//-------------------------------
  //Codigo do Laser
   
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
//-------------------------------

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

void esquerda(){
  int angulo = dir;
  for (angulo; angulo < 360; angulo += 1) { // Comando que muda a posição do servo de 0 para 180°
  servo.write(angulo); // Comando para angulo específico
  delay(15); 
  }
  
  delay(2000); // Tempo de espera para próximo movimento
  for (angulo = 360; angulo >= 1; angulo -= 5) { // Comando que muda a posição do servo de 180 para 0°
  servo.write(angulo); // Comando para angulo específico
  delay(5);
  }
  
delay(2000);
  
  /*
  // Lê o valor do Potenciometro
  int angle = dir; 
  // Mapeia o valor de 0 a 180 graus
  angle=map(angle, 0, 1023, 0, 180);
  // Repassa o angulo ao ServoWrite
  servo.write(angle); 
  // Delay de 15ms para o Servo alcançar a posição
  delay(15);
  */
/*}
*/
void loop(){
   mantemConexoes();
  
  MQTT.loop();  
  
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
  
  //re();*/
  
  }
}

void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
        
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);  
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP()); 
  Serial.println("----------------------");  
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
             MQTT.subscribe(STATUS);
             
              MQTT.subscribe(TEMPO);
             
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}



void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg;
    String topicoo;
    topicoo = "";
    int tamanho = strlen(topic);
    Serial.print("Tamanho: ");
    Serial.println(tamanho);
    
    for(int j = 0; j < tamanho; j++){
      topicoo = topicoo+topic[j];
    }
    
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    Serial.print("Topico: ");
    Serial.println(topicoo);

    if (topicoo == "TEMPO"){
       tempoS = msg;
       recebeTempo(tempoS);
       Serial.print("tempo");
       Serial.println(tempoS);
    }
    
    if (topicoo == "STATUS"){
       statsS = msg;
       recebeStatus(statsS);
       Serial.print("Status");
       Serial.println(statsS);
    }

    if (topicoo == "DITANCIAC"){
       distaS = msg;
       recebeDistancia(distaS);
       Serial.print("Distancia Semafaro");
       Serial.println(distaS);
    }

    
}

int recebeTempo(String tempo){
      
      if(tempo == "1"){
        return 1;
      }
      else if(tempo == "2"){
        return 2;
      }
      else if(tempo == "3"){
        return 3;
      }
      else if(tempo == "4"){
        return 4;
      }
      else if(tempo == "5"){
        return 5;
      }
      else if(tempo == "6"){
        return 6;
      }
      else if(tempo == "7"){
        return 7;
      }
      else if(tempo == "8"){
        return 8;
      }
      else if(tempo == "9"){
        return 9;
      }
      else if(tempo == "10"){
        return 10;
      }
      else if(tempo == "11"){
        return 11;
      }
      else if(tempo == "12"){
        return 12;
      }
      else if(tempo == "13"){
        return 13;
      }
      else if(tempo == "14"){
        return 14;
      }
      else if(tempo == "15"){
        return 15;
      }
}



int recebeStatus(String statu){
    if (statu == "1"){//vermelho
           return 1;
        
     }
     else if (statu == "2"){//Verde
            return 2;
     }
     else if (statu == "3"){//amarelo
            return 3; 
     }       
     
} 

float  recebeDistancia(String distanciac){
   disSem = distanciac.toFloat();
   return disSem;
}

boolean logicasemafaro(int statu, int tempo, float disSem){
  
}
