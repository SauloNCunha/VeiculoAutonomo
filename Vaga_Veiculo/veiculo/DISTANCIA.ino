void distancia() {
  
    long duration, distance;
    digitalWrite(TRIGGERdir, LOW);  
    delayMicroseconds(2); 
  
    digitalWrite(TRIGGERdir, HIGH);
    delayMicroseconds(10); 
  
    digitalWrite(TRIGGERdir, LOW);
    duration = pulseIn(ECHOdir, HIGH);
    dir = (duration/2) / 29.1;
    Serial.print("Frente: ");
    Serial.println(dir);
}


void distanciaLaser(){
  /*-------------------------------*/
    /*Codigo do Laser*/
   
    VL53L0X_RangingMeasurementData_t measure;  
    /*Serial.print("Lendo a medida... ");*/
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus != 4) {
     /* Serial.print("Distancia laser (cm): "); */
     /* Serial.println((measure.RangeMilliMeter-40)/10);*/
      esq = (measure.RangeMilliMeter-40)/10;
    } else {
     /* Serial.println(" Fora de alcance! ");*/
      esq = 1;
    } 
    /*------------------------------- */
    Serial.print("Esquerda: ");
    Serial.println(esq);
}
