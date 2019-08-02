#include <Servo.h>

int angatual = 0; // angulo atual do servo
int angulo = 0; // angulo a ser atualizado
bool direita = false;

#define velDiant 5 //portas definidas pela shild para os motores (padrao)
#define dirDiant 0 //portas definidas pela shild para os motores (padrao) 
#define velTras  4 //portas definidas pela shild para os motores (padrao)
#define dirTras  2 //portas definidas pela shild para os motores (padrao)

#define TRIGGERdir 12
#define ECHOdir    13

//#define TRIGGERdiresq 16
//#define ECHOdiresq    17

//#define TRIGGERdirfre 5
//#define ECHOdirfre    4

float dir = 0;
//float esq = 0;
//float fre = 0;

Servo servo;

int sensorValue = 0;
int cmValue = 0;


int minRange = 512;
int maxRange = 512;

int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;
int pos = 0;

void setup(){
  Serial.begin(9600);
   
  // initial settings for motors off and direction forward
  servo.attach(16); // servo está na porta 15
  servo.write(pos); // aciona a porta do servo, zerando
  pinMode(velDiant, OUTPUT); //declara saida dos pinos
  pinMode(velTras, OUTPUT);
  pinMode(dirDiant, OUTPUT);
  pinMode(dirTras, OUTPUT);
  digitalWrite(velDiant, LOW); //declara motores desligados
  digitalWrite(velTras, LOW);
  digitalWrite(dirDiant, LOW);
  digitalWrite(dirTras,LOW);
  

  
  pinMode(TRIGGERdir, OUTPUT);
  pinMode(ECHOdir, INPUT);
}

void distancia() {
  
  long duration, distance;
  digitalWrite(TRIGGERdir, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGERdir, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGERdir, LOW);
  duration = pulseIn(ECHOdir, HIGH);
  dir = (duration/2) / 29.1;
  Serial.println(duration); 

  //-------------------------------

  /*long duration, distance;
  digitalWrite(TRIGGERdir, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGERdir, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGERdir, LOW);
  duration = pulseIn(ECHOdir, HIGH);
  esq = (duration/2) / 29.1;*/

  //----------------------------------

  /*long duration, distance;
  digitalWrite(TRIGGERdir, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGERdir, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGERdir, LOW);
  duration = pulseIn(ECHOdir, HIGH);
  fre = (duration/2) / 29.1;*/
  
  
  Serial.print("Centimeter:");
  Serial.println(dir);
  delay(1000);
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

void re(){
  digitalWrite(dirDiant,LOW);
  digitalWrite(dirTras,LOW);
  analogWrite(velDiant,maxSpeed);
  analogWrite(velTras,maxSpeed);
}

void direcao(){
  angatual = angulo;
  
  // Adicionar afino da distancia com o angulo e passar para a variavel angulo
  // afino irá retornar valor para angAtual, no qual irá atualizar a variavel angulo a partir da distancia do sensor ultrassonico
  // j irá incrementar do atual até o necessário para robo fazer a curva certa.
  // dentro da funcao angulacao() deve retornar, caso esquerda variavel direita = false, caso contrario direita = true.
  
  angulo = dir; // apos fazer funcao angulacao() tricar dir pelo retorno da funcao.
  int j = angatual;
  
  if (direita){
    
    for (j ; j =< angulo ; j += 1) { // Comando que muda a posição do servo de 0 para 180°
    servo.write(angulo); // Comando para angulo específico
    delay(15); // delay pode diminuir de acordo com a velocidade do robo
    }
  }else{
  
    
    for (angulo = 360; angulo >= 1; angulo -= 5) { // Comando que muda a posição do servo de 180 para 0°
    servo.write(angulo); // Comando para angulo específico
    delay(5);
    }
  }
}

void loop(){
  distancia();

  
  
  if(dir <= 10 ) {
    pontomorto();
  }
  else{
  direcao();
  frente();
  
  //re();
  }
  
  Serial.print("distancia: "); 
  Serial.println(dir);
}
