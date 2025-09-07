#include <WiFi.h>
#include "AdafruitIO_WiFi.h"
#include <time.h>

// ===== CONFIGURAÇÃO WIFI E ADAFRUIT IO =====
const char* ssid = "Casa Nova";
const char* password = "martaisabela";

#define IO_USERNAME "lucianojrs"
#define IO_KEY      "Adafruit API"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, ssid, password);

// ===== CONFIGURAÇÃO NTP =====
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800; // GMT-3
const int daylightOffset_sec = 0;

// ===== PINOS =====
int sensor_cap = 34;
int seco = 3700;
int molhado = 1400;

int boia_agua = 33;
int Rele = 32;

// ===== VARIÁVEIS =====
int estado_boia = 0;
bool bombaligada = false;
int qtd_regada = 0;

// ===== ADAFRUIT FEEDS =====
static unsigned long lastsend = 0;  
AdafruitIO_Feed *umidadeFeed = io.feed("umidade_solo");
AdafruitIO_Feed *boiaFeed = io.feed("estado_boia");
AdafruitIO_Feed *releFeed = io.feed("rele");
AdafruitIO_Feed *qtdregadaFeed = io.feed("qtd_regada");

void setup() {
  Serial.begin(115200);

  pinMode(boia_agua, INPUT_PULLUP);
  pinMode(Rele, OUTPUT);
  digitalWrite(Rele, HIGH);

  // Conectar ao Adafruit IO
  io.connect();
  while(io.status() < AIO_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConectado ao Adafruit IO!");

  // Configurar NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  // ===== HORA ATUAL =====
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter hora");
  }

  int hora = timeinfo.tm_hour;
  int min  = timeinfo.tm_min;
  int seg  = timeinfo.tm_sec;

  Serial.printf("Hora: %02d:%02d:%02d\n", hora, min, seg);

  // ===== SENSOR DE UMIDADE =====
  int umidade_solo = analogRead(sensor_cap);
  umidade_solo = map(umidade_solo, seco, molhado, 0, 100);
  umidade_solo = constrain(umidade_solo, 0, 100);

  Serial.printf("Umidade do Solo: %d%%\n", umidade_solo);

  // ===== BOIA =====
  estado_boia = digitalRead(boia_agua);
  if(estado_boia == HIGH){
    Serial.println("Reservatório com água!");
  } else {
    Serial.println("Reservatório Vazio");
  }

  // ===== CONTROLE DE RELE/BOMBA =====
  if(estado_boia == HIGH && umidade_solo <= 60 && (hora < 17 || (hora == 17 && min <= 30))){
    digitalWrite(Rele, HIGH);
    Serial.println("Relé LIGADO");
    if(!bombaligada){
      bombaligada = true;
      qtd_regada++;
    }
  } else {
    digitalWrite(Rele, LOW);
    Serial.println("Relé DESLIGADO");
    bombaligada = false;
  }

  // ===== RESET DIÁRIO =====
  if(hora == 0 && min == 0 && seg == 0){
    qtd_regada = 0;
  }

  // ===== ENVIO PARA ADAFRUIT IO =====
  if(millis() - lastsend > 10000){
    io.run();
    umidadeFeed->save(umidade_solo);
    boiaFeed->save(estado_boia);
    releFeed->save(digitalRead(Rele));
    qtdregadaFeed->save(qtd_regada);
    lastsend = millis();
    Serial.println("Dados enviados para Adafruit IO");
  }

  delay(1000);
}
