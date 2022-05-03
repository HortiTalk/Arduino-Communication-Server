/*
  serverRequest.h

  Autor: Hortitalk

  Dezembro de 2021
*/

#ifndef server_request
#define server_request

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Propriedades para passar para registro e busca no servidor
const String TEMPERATURA = "temperatura";
const String UMIDADE = "umidade";
const String UMIDADE_SOLO = "umidade";
const String CULTIVO = "cultivo";
const String BOARD_CODE = "codigo";
const String MODULO = "modulo";
const String VIVEIRO = "viveiro";

class ServerRequest {
  private:
    std::unique_ptr<BearSSL::WiFiClientSecure>* _client;
    void registrarInformacoesServidorModulo(String url, String propriedade, String valor, String propriedadeRelacionamento, int valorRelacionamento);
    void registrarInformacoesServidorModulo(String url, String propriedade, int valor, String propriedadeRelacionamento, String valorRelacionamento);
    
  public:
    ServerRequest(std::unique_ptr<BearSSL::WiFiClientSecure>& client);
    boolean verificarSePlacaJaRegistrada(String url);
    void registrarPlaca(String url, String boardCode, int viveiro);
    void registrarTemperatura(String url, int temperatura, String boardCode);
    void registrarUmidade(String url, int umidade, String boardCode);
    void registrarUmidadeSolo(String url, int umidade, String boardCode);
};

#endif
