float  recebeDistancia(String distanciac){
   disSem = distanciac.toFloat();
   return disSem;
}

float recebeDistanciaVaga(String distanciac){
      disVaga = distanciac.toFloat();
      return disVaga;
}

/* Arquivo MQTT adicionar no subscribe os Topicos que vem do semafaro
 *  adicioanr na função recebe pacote verificando o nome do topico para ler a mensagem vinda junto do topico para mandar para a função correspondente 
 *  dentro do arquivo VAGA
 *  
 *  OBS MQTT so envia e recebe STRING 
 */
