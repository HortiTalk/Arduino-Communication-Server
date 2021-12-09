/*
  serverRequest.cpp

  Autor: Lucas Rodrigues Severo

  Dezembro de 2021
*/

#include "Arduino.h"
#include "serverRequest.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

HTTPClient http;

ServerRequest::ServerRequest(std::unique_ptr<BearSSL::WiFiClientSecure>& client) {
  _client = &client;
}

void ServerRequest::registrarPlaca(String url, String boardCode, int cultivo) {
  registrarInformacoesServidorModulo(url, BOARD_CODE, boardCode, CULTIVO, 1);
}

void ServerRequest::registrarTemperatura(String url, int temperatura, String boardCode) {
  registrarInformacoesServidorModulo(url, TEMPERATURA, temperatura, MODULO, boardCode);
}

void ServerRequest::registrarUmidade(String url, int umidade, String boardCode) {
  registrarInformacoesServidorModulo(url, UMIDADE, umidade, MODULO, boardCode);
}

void ServerRequest::registrarUmidadeSolo(String url, int umidade, String boardCode) {
  registrarInformacoesServidorModulo(url, UMIDADE_SOLO, umidade, MODULO, boardCode);
}

boolean ServerRequest::verificarSePlacaJaRegistrada(String url) {
  boolean placaExiste = false;
  if(http.begin(**_client, url)) {
    DynamicJsonDocument jsonTemperatura(1024);
    
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();

    Serial.println(httpCode);
    if (httpCode > 0) {
      String payload = http.getString();
      placaExiste = (payload == "true");
    } else {
      Serial.println("Problema ao verificar se placa já registrada");
      Serial.printf("[HTTPS] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return placaExiste;
}

// valor Inteiro
void ServerRequest::registrarInformacoesServidorModulo(String url, String propriedade, String valor, String propriedadeRelacionamento, int valorRelacionamento) {
  if(http.begin(**_client, url)) {
    DynamicJsonDocument documentJson(1024);
    documentJson[propriedade] = valor;
    documentJson[propriedadeRelacionamento] = valorRelacionamento;
        
    String json;
    serializeJson(documentJson, json);
        
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json);
    
    Serial.println(httpCode);
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Problema ao registrar " + propriedade);
      Serial.printf("[HTTPS] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}


// valor String
void ServerRequest::registrarInformacoesServidorModulo(String url, String propriedade, int valor, String propriedadeRelacionamento, String valorRelacionamento) {
  if(http.begin(**_client, url)) {
    DynamicJsonDocument documentJson(1024);
    documentJson[propriedade] = valor;
    documentJson[propriedadeRelacionamento] = valorRelacionamento;
        
    String json;
    serializeJson(documentJson, json);
        
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json);
    
    Serial.println(httpCode);
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Problema ao registrar " + propriedade);
      Serial.printf("[HTTPS] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}
