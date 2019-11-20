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
  
  
     pos = 90;
     servo.write(pos); /* Comando para angulo específico*/
  
}

void esquerda(){
    
     if (pos < 123){
        pos = pos+6;
        servo.write(pos); 
        delay(45);
     }
}

void direita(){
   
     if (pos > 60){
        pos = pos-5;
        servo.write(pos); 
        delay(40);
     }
     /*
     for(pos; pos > 50; pos--){ /* Comando que muda a posição do servo de 180 para 0°*/
     /*    servo.write(pos); /* Comando para angulo específico*/
    /*     delay(18); 
       
             distancia();
              Serial.print("Direita "); 
        Serial.println(esq);
        // delay(20); 
         if (esq > 4.5){
      
           break;
           
           }
         }

         */
}
