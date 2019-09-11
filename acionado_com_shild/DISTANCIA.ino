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
  
    

    /*long duration, distance;
    digitalWrite(TRIGGERdir, LOW);  
    delayMicroseconds(2); 
  
    digitalWrite(TRIGGERdir, HIGH);
    delayMicroseconds(10); 
  
    digitalWrite(TRIGGERdir, LOW);
    duration = pulseIn(ECHOdir, HIGH);
    fre = (duration/2) / 29.1;*/
}


float distanciaLaser(){
  /*-------------------------------*/
    /*Codigo do Laser*/
   /*
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

    return fre;*/
    /*------------------------------- */
}
