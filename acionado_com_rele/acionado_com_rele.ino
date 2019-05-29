#include <Ultrasonic.h>

const int dianteiro = 9;
const int traseiro = 8;

#define pino_trigger 5
#define pino_echo 6


Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup() {

 Serial.begin(9600);
 Serial.println("Lendo dados do sensor...");
 pinMode(dianteiro, OUTPUT);
 pinMode(traseiro, OUTPUT);
}

void loop() {
    //Le as informacoes do sensor, em cm e pol
    float cmMsec, inMsec;
    long microsec = ultrasonic.timing();
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    //inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
    //Exibe informacoes no serial monitor

    if(cmMsec <= 10){
          digitalWrite(dianteiro, LOW);
          digitalWrite(traseiro, LOW);
    }
    else{
         digitalWrite(dianteiro, HIGH);
         digitalWrite(traseiro, HIGH);
    }
    
    //delay(5000);
    Serial.print("Distancia em cm: ");
    Serial.println(cmMsec);
}
