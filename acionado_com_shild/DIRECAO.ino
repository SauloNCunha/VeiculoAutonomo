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

void esquerda(){
    int angulo = esq;
    for (angulo; angulo < 360; angulo += 1) { /* Comando que muda a posição do servo de 0 para 180°*/
      servo.write(angulo); /* Comando para angulo específico*/
      delay(15); 
      frente();
    }
  
   /*delay(2000);*/ /* Tempo de espera para próximo movimento*/
    for (angulo = 360; angulo >= 1; angulo -= 5) { /* Comando que muda a posição do servo de 180 para 0°*/
      servo.write(angulo); /* Comando para angulo específico*/
      delay(5);
      frente();
    }
  
    /*delay(2000);*/
  
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
