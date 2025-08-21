#include <WiFi.h>
#include "time.h"

// --- CONFIGURAÇÃO --- //
bool usarWiFi = false; // true = conecta Wi-Fi / false = usa horário interno

const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3 * 3600; // UTC-3 (Brasília)
const int   daylightOffset_sec = 0;

// Horários que vão acender o LED (HH:MM)
String horarios[] = {"09:00", "15:00", "21:00"};

// Pinos LEDs RGB (Blue, Red)
const int Blue1 = 32;
const int Red1  = 33;
bool isActive1 = false;

const int Blue2 = 25;
const int Red2  = 26;
bool isActive2 = false;

const int Blue3 = 27;
const int Red3  = 14;
bool isActive3 = false;

// Variáveis de hora interna (simulada)
int horaSim = 8;
int minutoSim = 59;

void setup() {
  Serial.begin(115200);

  pinMode(Red1, OUTPUT); pinMode(Blue1, OUTPUT);
  pinMode(Red2, OUTPUT); pinMode(Blue2, OUTPUT);
  pinMode(Red3, OUTPUT); pinMode(Blue3, OUTPUT);

  if(usarWiFi){
    Serial.println("Tentando conectar ao Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  } else {
    Serial.println("Usando horário interno simulado.");
  }
}

void loop() {
  String agora;

  if(usarWiFi){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Falha ao obter horário");
      return;
    }
    char buffer[6];
    sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    agora = String(buffer);
  } else {
    // Incrementa 1 minuto a cada 2 segundos para simulação
    minutoSim++;
    if(minutoSim >= 60){
      minutoSim = 0;
      horaSim++;
      if(horaSim >= 24) horaSim = 0;
    }
    char buffer[6];
    sprintf(buffer, "%02d:%02d", horaSim, minutoSim);
    agora = String(buffer);

    delay(2000); // 1 minuto simulado = 2 segundos
  }

  Serial.println("Hora atual: " + agora);

  // Apaga todos os LEDs antes de checar
  if (!isActive1){
    digitalWrite(Red1, HIGH); digitalWrite(Blue1, HIGH);
  }
  if (!isActive2){
    digitalWrite(Red2, HIGH); digitalWrite(Blue2, HIGH);
  }
  if (!isActive3){
    digitalWrite(Red3, HIGH); digitalWrite(Blue3, HIGH);
  }
  
  

  // Checa se bate com algum horário
  for(int i=0; i<3; i++){
    if(agora == horarios[0]) {
      digitalWrite(Red1, LOW); digitalWrite(Blue1, HIGH);
      isActive1 = true;
    }
    if(agora == horarios[1]) {
      digitalWrite(Red2, LOW); digitalWrite(Blue2, HIGH);
      isActive2 = true;
    }
    if(agora == horarios[2]) {
      digitalWrite(Red3, LOW); digitalWrite(Blue3, HIGH);
      isActive3 = true;
    }
  }

  if(usarWiFi){
    delay(60000);
  }
}
