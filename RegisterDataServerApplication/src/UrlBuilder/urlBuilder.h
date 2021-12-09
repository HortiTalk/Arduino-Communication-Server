/*
  url_builder.h

  Autor: Lucas Rodrigues Severo

  Dezembro de 2021
*/

#ifndef url_builder
#define url_builder

#include "Arduino.h"

class UrlBuilder {
  private:
    String _host;
    
  public:
    UrlBuilder(String host);
    String construirUrlVerificarPlacaExiste(String boardCode);
    String construirUrlRegistrarPlaca();
    String construirUrlRegistrarTemperatura();
    String construirUrlRegistrarUmidade();
    String construirUrlRegistrarUmidadeSolo();
};

#endif
