
int recebeTempo(String tempo){
      
      if(tempo == "1"){
        return 1;
      }
      else if(tempo == "2"){
        return 2;
      }
      else if(tempo == "3"){
        return 3;
      }
      else if(tempo == "4"){
        return 4;
      }
      else if(tempo == "5"){
        return 5;
      }
      else if(tempo == "6"){
        return 6;
      }
      else if(tempo == "7"){
        return 7;
      }
      else if(tempo == "8"){
        return 8;
      }
      else if(tempo == "9"){
        return 9;
      }
      else if(tempo == "10"){
        return 10;
      }
      else if(tempo == "11"){
        return 11;
      }
      else if(tempo == "12"){
        return 12;
      }
      else if(tempo == "13"){
        return 13;
      }
      else if(tempo == "14"){
        return 14;
      }
      else if(tempo == "15"){
        return 15;
      }
       Serial.print("tempo");
       Serial.println(tempoS);
}

int recebeStatus(String statu){
    if (statu == "1"){//vermelho
           return 1;
        
     }
     else if (statu == "2"){//Verde
            return 2;
     }
     else if (statu == "3"){//amarelo
            return 3; 
     }       
     
} 

float  recebeDistancia(String distanciac){
   disSem = distanciac.toFloat();
   return disSem;
}
