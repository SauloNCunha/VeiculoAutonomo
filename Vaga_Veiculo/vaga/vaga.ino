#include <Ultrasonic.h> 
#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
const int TRIGGER1 = 16; /*D0 TRIGGER DO ULTRASSONICO ESQUERDO*/
const int ECHO1    = 5;  /*D1 DECHO DO ULTRASSONICO ESQUERDO*/
const int TRIGGER2 = 14; /*D5 TRIGGER DO ULTRASSONICO ESQUERDO*/
const int ECHO2   = 15; /*D8 ECHO DO ULTRASSONICO ESQUERDO*/

//WiFi
const char* SSID = "NotebookSaulo";/*"carro"; "MostraProfissoes";              /* SSID / nome da rede WiFi que deseja se conectar*/
const char* PASSWORD = "Saulo1004";/*"unifor2019";  "profissoes";   /* Senha da rede WiFi que deseja se conectar*/
WiFiClient wifiClient; 

//MQTT Server
const char* BROKER_MQTT ="192.168.137.1";/*"192.168.1.148";"broker.hivemq.com";//"test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar*/
int BROKER_PORT = 1883;

float disVaga =0 ;
 
#define ID_MQTT  "SEMAFARO"     //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define VAGA1 "VAGA1"         //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define STAVAGA1 "STAVAGA1"
#define VAGA2 "VAGA2"
#define STAVAGA2 "STAVAGA2"

PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient

    

void setup() {
    Serial.begin(115200);
    conectaWiFi();
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);  

    pinMode(TRIGGER1, OUTPUT); /*DECLARA ULTRASSONICO COMO vaga 1*/
    pinMode(ECHO1, INPUT);     /*DECLARA ULTRASSONICO COMO vaga 1*/
  
    pinMode(TRIGGER2, OUTPUT); /*DECLARA ULTRASSONICO COMO vaga 2*/
    pinMode(ECHO2, INPUT);     /*DECLARA ULTRASSONICO COMO vaga 2*/
  
    /*VERIFICA A PORTA USB CONECTADA*/
    while (! Serial) {
      delay(1);
    }
}

void loop() {

    mantemConexoes();
  
  MQTT.loop();  
  // put your main code here, to run repeatedly:

  vaga1();

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

void vaga1(){
     long duration, distance;
     digitalWrite(TRIGGERdir, LOW);  
     delayMicroseconds(2); 
  
     digitalWrite(TRIGGERdir, HIGH);
     delayMicroseconds(10); 
  
     digitalWrite(TRIGGERdir, LOW);
     duration = pulseIn(ECHOdir, HIGH);
     disVaga = (duration/2) / 29.1;
  
     if (disVaga >=20) && (disVaga <=200){
      MQTT.publish("STAVAGA1","1"); 
   }else{
    MQTT.publish("STAVAGA1","0"); 
   }

     String dis = String(disVaga);

     MQTT.publish("VAGA",(char*)dis.c_str());
     Serial.println("Vaga");
     Serial.println(String(disVaga));
     delay(1000);  
}

void vaga2(){
    long duration, distance;
    digitalWrite(TRIGGERdir, LOW);  
    delayMicroseconds(2); 
  
    digitalWrite(TRIGGERdir, HIGH);
    delayMicroseconds(10); 
  
    digitalWrite(TRIGGERdir, LOW);
    duration = pulseIn(ECHOdir, HIGH);
    disVaga = (duration/2) / 29.1;
  
    String dis = String(disVaga);

    MQTT.publish("VAGA",(char*)dis.c_str());
    Serial.println("Vaga");
    Serial.println(String(disVaga));
    delay(1000);
  
    if (disVaga >=20) && (disVaga <=200){
     MQTT.publish("STAVAGA2","1");  
  }else{
    MQTT.publish("STAVAGA2","0"); 
  }  
}
