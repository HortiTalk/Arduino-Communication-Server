#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>

// este parâmetro altera conforme a validade do certificado
const char* fingerprint = "C3:75:80:CA:88:E2:40:62:94:8F:E9:E9:BD:18:81:B8:33:39:AA:33";


const char* ssid = "NOME DA REDE";
const char* password = "SENHA DA REDE";
const char* host = "https://hortitalk-api.herokuapp.com"; // URL para o servidor
const String boardId = WiFi.macAddress();
boolean placaExiste = false;


int temperatura = 20;
int umidade = 21;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);

    HTTPClient https;

    // Verificar se placa existe
    if(https.begin(*client, construirUrlVerificarPlacaExiste())) {
      DynamicJsonDocument jsonTemperatura(1024);
      
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.GET();
  
      Serial.println(httpCode);
      if (httpCode > 0) {
        String payload = https.getString();
        Serial.println(payload);
        placaExiste = (payload == "true");
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }

    // Registrar Placa
    if (!placaExiste) {
      if(https.begin(*client, construirUrlRegistrarPlaca())) {
        DynamicJsonDocument jsonTemperatura(1024);
        jsonTemperatura["board_id"] = boardId;
        
        String json;
        serializeJson(jsonTemperatura, json);
        
        https.addHeader("Content-Type", "application/json");
        int httpCode = https.POST(json);
    
        Serial.println(httpCode);
        if (httpCode > 0) {
          String payload = https.getString();
          Serial.println(payload);
        } else {
          Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      }
    }
    
    // Registrar Temperatura
    if(https.begin(*client, construirUrlRegistrarTemperatura())) {
      DynamicJsonDocument jsonTemperatura(1024);
      jsonTemperatura["temperatura"] = temperatura++;
      
      String json;
      serializeJson(jsonTemperatura, json);
      
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(json);
  
      Serial.println(httpCode);
      if (httpCode > 0) {
        String payload = https.getString();
        Serial.println(payload);
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }

    // Registrar Umidade
    if(https.begin(*client, construirUrlRegistrarUmidade())) {
      DynamicJsonDocument jsonUmidade(1024);
      jsonUmidade["umidade"] = umidade++;
      
      String json;
      serializeJson(jsonUmidade, json);
      
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(json);
  
      Serial.println(httpCode);
      if (httpCode > 0) {
        String payload = https.getString();
        Serial.println(payload);
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
  delay(5000);
}

String construirUrlVerificarPlacaExiste() {
  return String(host) + "/board/" + boardId + "/exists";
}

String construirUrlRegistrarPlaca() {
  return String(host) + "/board/";
}

String construirUrlRegistrarTemperatura() {
  return String(host) + "/temperatura/board/" + boardId;
}

String construirUrlRegistrarUmidade() {
  return String(host) + "/umidade/board/" + boardId;
}
