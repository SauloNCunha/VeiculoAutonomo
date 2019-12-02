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

void reto(){  
     pos = 90; /*por ser um servo motor de 180º o ponto central e 90º*/
     servo.write(pos); /* Comando para angulo específico*/  
}

void esquerda(){
    
     if (pos < 123){
		/*a cada interação do loop principal de acordo com a distância
		do canteiro central adiciona 6º para que ele corrija seu 
		trajeto para que o veículo fique entre 5 e 6 cm do canteiro central*/
        pos = pos+6;
						
        servo.write(pos); 
        delay(45);
     }
}

void direita(){
   
     if (pos > 60){
		 /*a cada interação do loop principal de acordo com a distância
		do canteiro central adiciona 6º para que ele corrija seu 
		trajeto para que o veículo fique entre 5 e 6 cm do canteiro central*/
        pos = pos-6;
        servo.write(pos); 
        delay(45);
     }
}
