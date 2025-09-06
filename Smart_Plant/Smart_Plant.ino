#include <WiFi.h>
#include "time.h"

const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // fuso horário (Brasil -3h)
const int   daylightOffset_sec = 0;

int sensor_cap = 34;
int seco = 3700;
int molhado = 1400;

int boia_agua = 35;
int estado_boia = 0;

int Rele = 32;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(boia_agua, INPUT_PULLUP);

  pinMode(Rele, OUTPUT);
  digitalWrite(Rele, HIGH);

}

void loop() {
  //RTC/WIFI
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter hora");
  }
  else{
    Serial.print("Hora: ");
    Serial.println(&timeinfo, "%H:%M:%S");
  }

  //SENSOR DE UMIDADE CAP 
  int umidade_solo = analogRead(sensor_cap);
  umidade_solo = map(umidade_solo, seco, molhado, 0, 100);
  if (umidade_solo > 100) umidade_solo = 100;
  else if(umidade_solo < 0) umidade_solo = 0;
  Serial.println("Umidade do Solo: " + String(umidade_solo) + "%");

  //BOIA D AGUA
  estado_boia = digitalRead(boia_agua);
  if (estado_boia == HIGH){
    Serial.println("Reservatório com água!");
    }
  else{
    Serial.println("Reservatório Vazio");
  }

  //RELE/BOMBA
  digitalWrite(Rele, LOW);
  Serial.println("Relé LIGADO");
  delay(1000);
  
  digitalWrite(Rele, HIGH);
  Serial.println("Relé DESLIGADO");
  delay(1000);


}
