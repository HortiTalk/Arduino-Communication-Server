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

// Propriedades para passar para registro e busca no servidor
const String TEMPERATURA = "temperatura";
const String UMIDADE = "umidade";
const String BOARD_ID = "board_id";

boolean placaExiste = false;

// Configurações para requisições com SSL
std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
HTTPClient https;

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
    client->setFingerprint(fingerprint);

    // Verificar se placa existe
    verificarSePlacaJaRegistrada();

    // Registrar Placa, caso não esteja cadastrada
    if (!placaExiste) {
      registrarInformacoesServidor(construirUrlRegistrarPlaca(), BOARD_ID, boardId);
    }
    
    // Registrar Temperatura
    registrarInformacoesServidor(construirUrlRegistrarTemperatura(), TEMPERATURA, temperatura++);

    // Registrar Umidade
    registrarInformacoesServidor(construirUrlRegistrarUmidade(), UMIDADE, umidade++);
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

void registrarInformacoesServidor(String url, String propriedade, int valor) {
  if(https.begin(*client, url)) {
    DynamicJsonDocument documentJson(1024);
    documentJson[propriedade] = valor;
        
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

void registrarInformacoesServidor(String url, String propriedade, String valor) {
  if(https.begin(*client, url)) {
    DynamicJsonDocument documentJson(1024);
    documentJson[propriedade] = valor;
        
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
