#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>

// este parâmetro altera conforme a validade do certificado
const char* fingerprint = "E1:87:83:58:FE:6B:0E:DF:61:F2:12:AE:D2:F2:C3:7F:A2:70:6C:5C";

const char* ssid = "NOME DA REDE";
const char* password = "SENHA DA REDE";
const char* host = "https://hortitalk-server.herokuapp.com"; // URL para o servidor
const String boardId = WiFi.macAddress();

// Propriedades para passar para registro e busca no servidor
const String TEMPERATURA = "temperatura";
const String UMIDADE = "umidade";
const String UMIDADE_SOLO = "umidade";
const String CULTIVO = "cultivo";
const String BOARD_ID = "codigo";
const String MODULO = "modulo";

boolean placaExiste = false;

// Configurações para requisições com SSL
std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
HTTPClient https;

int temperatura = 20;
int umidade = 21;
int umidade_solo = 30;

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
    client->setFingerprint(fingerprint);

    // Verificar se placa existe
    verificarSePlacaJaRegistrada();

    // Registrar Placa, caso não esteja cadastrada
    if (!placaExiste) {
      registrarInformacoesServidorModulo(construirUrlRegistrarPlaca(), BOARD_ID, boardId, CULTIVO, 1);
    }
    
    // Registrar Temperatura
    registrarInformacoesServidorModulo(construirUrlRegistrarTemperatura(), TEMPERATURA, temperatura++, MODULO, boardId);

    // Registrar Umidade
    registrarInformacoesServidorModulo(construirUrlRegistrarUmidade(), UMIDADE, umidade++, MODULO, boardId);

    // Registrar Umidade Solo
    registrarInformacoesServidorModulo(construirUrlRegistrarUmidadeSolo(), UMIDADE_SOLO, umidade++, MODULO, boardId);
  }
  delay(5000);
}

void verificarSePlacaJaRegistrada() {
  if(https.begin(*client, construirUrlVerificarPlacaExiste())) {
    DynamicJsonDocument jsonTemperatura(1024);
    
    https.addHeader("Content-Type", "application/json");
    int httpCode = https.GET();
    
    if (httpCode > 0) {
      String payload = https.getString();
      placaExiste = (payload == "true");
    } else {
      Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  }
}

void registrarInformacoesServidorModulo(String url, String propriedade, String valor, String propriedadeRelacionamento, int valorRelacionamento) {
  if(https.begin(*client, url)) {
    DynamicJsonDocument documentJson(1024);
    documentJson[propriedade] = valor;
    documentJson[propriedadeRelacionamento] = valorRelacionamento;
        
    String json;
    serializeJson(documentJson, json);
        
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

void registrarInformacoesServidorModulo(String url, String propriedade, int valor, String propriedadeRelacionamento, String valorRelacionamento) {
  if(https.begin(*client, url)) {
    DynamicJsonDocument documentJson(1024);
    documentJson[propriedade] = valor;
    documentJson[propriedadeRelacionamento] = valorRelacionamento;
        
    String json;
    serializeJson(documentJson, json);
        
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

String construirUrlVerificarPlacaExiste() {
  return String(host) + "/modulos/" + boardId + "/existe";
}

String construirUrlRegistrarPlaca() {
  return String(host) + "/modulos/";
}

String construirUrlRegistrarTemperatura() {
  return String(host) + "/temperaturas/";
}

String construirUrlRegistrarUmidade() {
  return String(host) + "/umidades/";
}

String construirUrlRegistrarUmidadeSolo() {
  return String(host) + "/umidades-solo/";
}
