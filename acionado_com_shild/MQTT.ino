
void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(STATUS);
             MQTT.subscribe(TEMPO);
             MQTT.subscribe(DISTANCIA);  
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}


void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg = " ";
    String topicoo;
    topicoo = "";

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++){
       char c = (char)payload[i];
       msg += c;     
    }

    int tamanho = strlen(topic);
    
    for(int j = 0; j < tamanho; j++){
      topicoo = topicoo+topic[j];
    }
    
    Serial.print("Topico: ");
    Serial.println(topicoo);

    if (topicoo == "TEMPO"){
       tempoS = msg;
       recebeTempo(tempoS);
       Serial.print("tempo");
       Serial.println(tempoS);
    }
    
    if (topicoo == "STATUS"){
       statsS = msg;
       recebeStatus(statsS);
       Serial.print("Status");
       Serial.println(statsS);
    }

    if (topicoo == "DISTANCIA"){
       distaS = msg;
       recebeDistancia(distaS);
       Serial.print("Distancia Semafaro: ");
       Serial.println(distaS);
    }
}
