#include <ArduinoJson.h>
#include <Arduino.h>
#include "src/UrlBuilder/urlBuilder.h"
#include "src/ServerRequest/serverRequest.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include "DHT.h"
#define DHTTYPE DHT22
#define DHTPIN D1
#define pino_sinal_analogico A0
#include <ESP8266HTTPClient.h>

// este parâmetro altera conforme a validade do certificado
const char* fingerprint = "B5:86:32:41:D3:FB:3D:2B:56:26:0E:A3:51:14:75:E8:DC:FD:4A:9B";
//  E1:87:83:58:FE:6B:0E:DF:61:F2:12:AE:D2:F2:C3:7F:A2:70:6C:5C
//  39:A9:C4:FE:B1:7E:23:9E:2F:4D:BB:AC:E8:D7:A3:4F:CE:43:0E:7B

const char* ssid = "Wifi";
const char* password = "123456789";
const String host = "https://hortitalk-server.herokuapp.com";
const String boardCode = WiFi.macAddress();

// Configurações para requisições com SSL
std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
UrlBuilder* urlBuilder = new UrlBuilder(host);
ServerRequest* serverRequest = new ServerRequest(client);

DHT dht(DHTPIN, DHTTYPE);
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
//  temperatura = dht.readTemperature();
//  umidade = dht.readHumidity();
//  umidade_solo = analogRead(pino_sinal_analogico);
  
  if (WiFi.status() == WL_CONNECTED) {
    client->setFingerprint(fingerprint);

    // Verificar se placa existe
    String urlPlacaExiste = urlBuilder->construirUrlVerificarPlacaExiste(boardCode);
    boolean placaExiste = serverRequest->verificarSePlacaJaRegistrada(urlPlacaExiste);

    // Registrar Placa, caso não esteja cadastrada
    if (!placaExiste) {
      String urlRegistrarPlaca = urlBuilder->construirUrlRegistrarPlaca();
      serverRequest->registrarPlaca(urlRegistrarPlaca, boardCode, 1);
    }
    
    // Registrar Temperatura
    String urlRegistrarTemperatura = urlBuilder->construirUrlRegistrarTemperatura();
    serverRequest->registrarTemperatura(urlRegistrarTemperatura, temperatura, boardCode);

    // Registrar Umidade
    String urlRegistrarUmidade = urlBuilder->construirUrlRegistrarUmidade();
    serverRequest->registrarUmidade(urlRegistrarUmidade, umidade, boardCode);

    // Registrar Umidade Solo
    String urlRegistrarUmidadeSolo = urlBuilder->construirUrlRegistrarUmidadeSolo();
    serverRequest->registrarUmidadeSolo(urlRegistrarUmidadeSolo, umidade_solo, boardCode);
  }
  
  delay(5000);
}
