/*
  url_builder.cpp

  Autor: Lucas Rodrigues Severo

  Dezembro de 2021
*/

#include "Arduino.h"
#include "urlBuilder.h"

UrlBuilder::UrlBuilder(String host) {
  _host = host;
}

String UrlBuilder::construirUrlVerificarPlacaExiste(String boardCode) {
  return _host + "/modulos/" + boardCode + "/existe";
}

String UrlBuilder::construirUrlRegistrarPlaca() {
  return _host + "/modulos/";
}

String UrlBuilder::construirUrlRegistrarTemperatura() {
  return _host + "/temperaturas/";
}

String UrlBuilder::construirUrlRegistrarUmidade() {
  return _host + "/umidades/";
}

String UrlBuilder::construirUrlRegistrarUmidadeSolo() {
  return _host + "/umidades-solo/";
}
