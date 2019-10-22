#include <Ultrasonic.h>

const int dianteiro = 7;
const int traseiro = 6;

const int infraVermelho = 8;

#define pino_triggerD 4
#define pino_echoD 5

#define pino_triggerE 2
#define pino_echoE 3

Ultrasonic ultrasonicD(pino_triggerD, pino_echoD);
Ultrasonic ultrasonicE(pino_triggerE, pino_echoE);

void setup() {

 Serial.begin(9600);
 Serial.println("Lendo dados do sensor...");
 pinMode(dianteiro, OUTPUT);
 pinMode(traseiro, OUTPUT);

}

void loop() {
    //Le as informacoes do sensor, em cm e pol
    float cmMsecD,cmMsecE;
    long microsec1 = ultrasonicD.timing();
    long microsec2 = ultrasonicE.timing();
    cmMsecD = ultrasonicD.convert(microsec1, Ultrasonic::CM);
    cmMsecE = ultrasonicE.convert(microsec2, Ultrasonic::CM);
    //Exibe informacoes no serial monitor

    
    int C = digitalRead(infraVermelho);

    if( (C == 0) || (cmMsecD <= 5) || (cmMsecE <=5)  ){
          digitalWrite(dianteiro, LOW);
          digitalWrite(traseiro, LOW);
    }
    else{
         digitalWrite(dianteiro, HIGH);
         digitalWrite(traseiro, HIGH);
    }
    
    delay(1000);
    Serial.print("Distancia Direita em cm: ");
    Serial.println(cmMsecD);
    Serial.print("Distancia Esquerda em cm: ");
    Serial.println(cmMsecE);
    Serial.print("Infra Vermelho ");
    Serial.println(C);
    
    
}
