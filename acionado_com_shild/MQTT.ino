
void mantemConexoes() {
  if (!MQTT.connected()) {
    conectaMQTT();
  }

  conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
void conectaMQTT() {
  while (!MQTT.connected()) {
    /*Serial.print("Conectando ao Broker MQTT: ");*/
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      /*Serial.println("Conectado ao Broker com sucesso!");*/
      Serial.println("Status");
      MQTT.subscribe(STATUS);
      Serial.println("Tempo");
      MQTT.subscribe(TEMPO);
      Serial.println("Distancia");
      MQTT.subscribe(DISTANCIA);
    }
    else {
      /*Serial.println("Noo foi possivel se conectar ao broker.");*/
      /*Serial.println("Nova tentatica de conexao em 10s");*/
      delay(10000);
    }
  }
}


void recebePacote(char* topic, byte* payload, unsigned int length)
{
  String msg = " ";
  String topicoo;
  topicoo = "";
  int tamanho = strlen(topic);
  for (int j = 0; j < tamanho; j++) {
    topicoo = topicoo + topic[j];
  }

  //obtem a string do payload recebido
  
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
    if (topicoo == "STATUS") {
      Serial.println("Status");
      Serial.println(msg);
    }
    
    
  }

  

  

  /*Serial.print("Topico: ");*/
  /*Serial.println(topicoo);*/

  if (topicoo == "TEMPO") {
    tempoS = msg;
    recebeTempo(tempoS);
    /*Serial.print("tempo");*/
    /*Serial.println(tempoS);*/
    /*Serial.println(recebeTempo(tempoS));*/
    tempoSem = recebeTempo(tempoS);
  }

  if (topicoo == "STATUS") {
    
    statsS = msg;
    
    /*Serial.print("Status");*/
    /*Serial.println(statsS);*/
    
    statusSem = recebeStatus(statsS);
  }

  if (topicoo == "DISTANCIA") {
    distaS = msg;
    recebeDistancia(distaS);
    /*Serial.print("Distancia Semafaro: ");*/
    /*Serial.println(distaS);*/
    /*Serial.println(recebeDistancia(distaS));*/
    disSem = recebeDistancia(distaS);
  }
}
