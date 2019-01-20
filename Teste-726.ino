#include <ESP8266WiFi.h>
#include <SPI.h>
#include <VS1053.h>
#define XDCS  D0
#define XCS   D1
#define DREQ  D2
#define RST   D8
#define SCLK  D5 // habilitar para decodificar aac
#define MOSI  D7
#define MISO  D6
#define VOLUME_KNOB   A0
#define VOLUME        90


VS1053 player(XCS, XDCS, DREQ, RST );
uint8_t mp3buff[32];
int loopcounter = 0;
int currentvolume = VOLUME;
#ifndef STASSID
#define STASSID "NaoGrite"
#define STAPSK  "porcaria" // pode-se usar WPS !!!!!!!!!!!
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;
/* http://r2.ciclano.io:10032/stream */
const char* host = "r2.ciclano.io";
const uint16_t port = 10032;
const char* clinome = "Esp8266";
void setup() {
  Serial.begin(115200);
  Serial.println("Setup...");
  Serial.println("ok...");
  Serial.print("Conectado em: ");
  Serial.println(ssid);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.hostname(clinome);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("ok");
  Serial.println("WiFi conectado...");
  Serial.print("Endereço IP : ");
  Serial.println(WiFi.localIP());

  pinMode(SCLK, INPUT); // habilitar para decodificar STREAMER aac
  digitalWrite(SCLK, HIGH); // habilitar para decodificar STREAMER aac
  pinMode ( DREQ, INPUT ) ;
  pinMode ( XCS, OUTPUT ) ;
  digitalWrite ( XCS, HIGH ) ;
  pinMode ( XDCS, OUTPUT ) ;
  digitalWrite ( XDCS, HIGH ) ;



  SPI.begin();
  player.begin();
  player.setVolume(VOLUME, VOLUME); // Volume canal esquerdo e Volume canal direito (estéreo)
  //player.sineTest(44, 999); // beepa a placa vs1053 para testar - sintaxe(tonalidade do apito,tempo do apito)
}

void loop() {
  Serial.print("conectado em: ");
  Serial.print(host);
  Serial.print(':');
  Serial.print(port);
  Serial.println();
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Erro de Conexão... Conexão falhou...");
    delay(5000);
    return;
  }
  if (client.connected()) {

    Serial.println("Recebendo dados do servidor...");

    client . print ( String ( "GET /stream " ) + " HTTP/1.1 \r\n " +
                     "Host: " + host + " \r\n " +
                     "Port: " + port + " \r\n " +
                     "Connection: close \r\n\r\n " );

    String line = client.readStringUntil('\r');
    Serial . print ("Recebido Retorno :");
    Serial . println(client.availableForWrite());
    int LenBuffer = client.write(client.availableForWrite()); // enviar para vs1053 (LenBuffer,myBuffer)
    /// ??????
    
    //Serial.print(player.playData(myBuffer,LenBuffer));
    Serial.println(player.spiread());

    /// ??????
    while (client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
    }


    Serial.println();
    Serial.print("Código do erro VS1053:");
    Serial.println(player.begin());
    delay(3000);
  }
}
