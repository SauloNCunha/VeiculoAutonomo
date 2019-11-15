#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include "Adafruit_VL53L0X.h"     /*Biblioteca do laser */
#include <Wire.h>                 /*Biblioteca para declaração das portas SDA e SCL*/

#define vermelho 2  /*D4*/
#define amarelo 0   /*D3*/
#define verde 4    /*D2*/

//WiFi
const char* SSID = "NotebookSaulo";/*"carro"; "MostraProfissoes";              /* SSID / nome da rede WiFi que deseja se conectar*/
const char* PASSWORD = "Saulo1004";/*"unifor2019";  "profissoes";   /* Senha da rede WiFi que deseja se conectar*/
WiFiClient wifiClient; 
  
//MQTT Server
const char* BROKER_MQTT ="192.168.137.1";/*"192.168.1.148";"broker.hivemq.com";//"test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar*/
int BROKER_PORT = 1883;
                   
 
#define ID_MQTT  "SEMAFARO"     //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define STATUS "STATUS"         //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define TEMPO "TEMPO"           //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define DISTANCIA "DISTANCIA"   //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.

PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient

//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT
//void recebePacote(char* topic, byte* payload, unsigned int length);
void semafaro();
void enviaStatus(int token,int tempo);
void distanciaLaser();

double fre = 0;
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); /*DECLARAÇÃO DO LASER*/


void setup() {
           /*D6,D7*/ 
  Wire.begin(13,12); /*/DECLARA SCL E DEPOIS A SDA DO LASER*/
  pinMode(vermelho, OUTPUT);         
  pinMode(amarelo, OUTPUT);
  pinMode(verde, OUTPUT);


  digitalWrite(vermelho,LOW);
  digitalWrite(amarelo,LOW);
  digitalWrite(verde,LOW);

  Serial.begin(115200);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  //MQTT.setCallback(recebePacote); 

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
  semafaro();
  MQTT.loop();  
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
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void semafaro(){
    int cont = 0;
    for(cont = 1; cont<=15 ;cont++){
      digitalWrite(vermelho, HIGH);
      digitalWrite(verde, LOW);
      digitalWrite(amarelo, LOW);
      delay(1000);
      enviaStatus(1,cont);
    }
    
    for(cont = 1; cont<=15 ;cont++){
     digitalWrite(vermelho, LOW);
     digitalWrite(verde, HIGH);
     digitalWrite(amarelo, LOW);
     delay(1000);
     enviaStatus(2,cont);
    }
    for(cont = 1; cont<=5 ;cont++){ 
     digitalWrite(vermelho, LOW);
     digitalWrite(verde, LOW);
     digitalWrite(amarelo, HIGH);
     delay(1000);
     enviaStatus(3,cont);
    }
  
}



void enviaStatus(int token,int tempo){

    
    String stempo = String(tempo);
     //MQTT.publish(STATUS,stoken);
     //MQTT.publish(TEMPO,stempo);
    switch (tempo) {
        case 1:
          MQTT.publish(TEMPO,"1");
        break; 
        case 2:
          MQTT.publish(TEMPO,"2");
        break;
        case 3:
          MQTT.publish(TEMPO,"3");
        break;
        case 4:
          MQTT.publish(TEMPO,"4");
        break;
        case 5:
          MQTT.publish(TEMPO,"5");
        break;
        case 6:
          MQTT.publish(TEMPO,"6");
        break;
        case 7:
          MQTT.publish(TEMPO,"7");
        break;
        case 8:
          MQTT.publish(TEMPO,"8");
        break;
        case 9:
          MQTT.publish(TEMPO,"9");
        break;
        case 10:
          MQTT.publish(TEMPO,"10");
        break;
        case 11:
          MQTT.publish(TEMPO,"11");
        break;
        case 12:
          MQTT.publish(TEMPO,"12");
        break;
        case 13:
          MQTT.publish(TEMPO,"13");
        break;
        case 14:
          MQTT.publish(TEMPO,"14");
        break;
        case 15:
          MQTT.publish(TEMPO,"15");
        break;
        default:
            MQTT.publish(TEMPO,"1");
        break;
    }

        
     if (token == 1){//vermelho
        Serial.println("Status: Vermelho Tempo: " + stempo);
        MQTT.publish(STATUS,"1");
     }
     else if (token == 2){//Verde
          Serial.println("Status: Verde Tempo: " + stempo);
          MQTT.publish(STATUS,"2");
     }
     else if (token == 3){//amarelo
          Serial.println("Status: Amarelo Tempo: " + stempo);
          MQTT.publish(STATUS,"3");
     } 

    distanciaLaser(); 
     
} 

void distanciaLaser(){
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
  
    /*------------------------------- */

    String tempDis;
    tempDis = (fre, DEC);
    String dir = String(tempDis);
    /*converte Float para String*/
    char buffer[10];
    dtostrf(fre,2, 2, buffer);
    /*-------------------------*/
    Serial.println("Dinstancia: " + dir);
    MQTT.publish(DISTANCIA,buffer);
}
