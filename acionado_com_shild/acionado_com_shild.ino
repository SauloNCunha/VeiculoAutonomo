
//#include <Servo.h>
//#include <SharpIR.h>

const int analogInPin = A0;

#define velDiant 5 //portas definidas pela shild para os motores (padrao)
#define dirDiant 0 //portas definidas pela shild para os motores (padrao) 
#define velTras  4 //portas definidas pela shild para os motores (padrao)
#define dirTras  2 //portas definidas pela shild para os motores (padrao)

#define model 1080 //DETERMINA O MODELO DO SENSOR (1080 PARA O MODELO 2Y0A21Y OU 20150 PARA O MODELO 2Y0A02Y)

//SharpIR SharpIR(analogInPin, model); //PASSA OS PARÂMETROS PARA A FUNÇÃO (PINO ANALÓGICO, MODELO DO SENSOR)

//Servo servo;

int sensorValue = 0;
int cmValue = 0;
int sensorPin = A0;

int minRange = 512;
int maxRange = 512;

int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;
int pos = 0;

void setup(){
  Serial.begin(9600);
   
  /*// initial settings for motors off and direction forward
  servo.attach(13); // servo está na porta 13
  servo.write(pos); // aciona a porta do servo, zerando
  pinMode(velDiant, OUTPUT); //declara saida dos pinos
  pinMode(velTras, OUTPUT);
  pinMode(dirDiant, OUTPUT);
  pinMode(dirTras, OUTPUT);

  digitalWrite(velDiant, LOW); //declara motores desligados
  digitalWrite(velTras, LOW);
  digitalWrite(dirDiant, LOW);
  digitalWrite(dirTras,LOW);
  */
  pinMode(sensorPin, INPUT);
}

void frente(){
  digitalWrite(velDiant, HIGH);
  digitalWrite(velTras, HIGH);
  digitalWrite(dirDiant, HIGH);
  digitalWrite(dirTras,HIGH);
}

void re(){
  digitalWrite(dirDiant,LOW);
  digitalWrite(dirTras,LOW);
  analogWrite(velDiant,maxSpeed);
  analogWrite(velTras,maxSpeed);
}

void loop(){
  /*
  frente();
  delay(1000);
  
  re();
  delay(1000);
  */

  sensorValue = analogRead(sensorPin);
  cmValue = 27.728 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.2045);
  Serial.print("Valor lido = ");
  Serial.print(cmValue);
  Serial.println(" cm");
  delay(250);
}
