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
const int Blue1 = 32; const int Red1  = 33; bool isActive1 = false; bool click1 = false;
const int Blue2 = 25; const int Red2  = 26; bool isActive2 = false; bool click2 = false;
const int Blue3 = 27; const int Red3  = 14; bool isActive3 = false; bool click3 = false;

const int btn1 = 4; const int btn2 = 5; const int btn3 = 18;

const int buzzer = 19;

// Variáveis de hora interna (simulada)
int horaSim = 8;
int minutoSim = 59;

void setup() {
  Serial.begin(115200);

  // pinagem dos leds
  pinMode(Red1, OUTPUT); pinMode(Blue1, OUTPUT);
  pinMode(Red2, OUTPUT); pinMode(Blue2, OUTPUT);
  pinMode(Red3, OUTPUT); pinMode(Blue3, OUTPUT);

  // pinagem dos botões
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);

  // pinagem Buzzer
  pinMode(buzzer, OUTPUT);

  // configurar wi-fi
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

  // checar se os botões estão pressionados
  bool b1 = digitalRead(btn1) == LOW;
  bool b2 = digitalRead(btn2) == LOW;
  bool b3 = digitalRead(btn3) == LOW;

  // Calcular tempo
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

    delay(100); // 1 minuto simulado = 1 segundo
  }

  // printar horario
  Serial.println("Hora atual: " + agora);

  // Apaga todos os LEDs
  if (!isActive1){
    if (click1) {
      digitalWrite(Red1, HIGH); digitalWrite(Blue1, LOW);
    } else {
      digitalWrite(Red1, HIGH); digitalWrite(Blue1, HIGH);
    }
  }
  if (!isActive2){
    if (click2) {
      digitalWrite(Red2, HIGH); digitalWrite(Blue2, LOW);
    } else {
      digitalWrite(Red2, HIGH); digitalWrite(Blue2, HIGH);
    }
  }
  if (!isActive3){
    if (click3) {
      digitalWrite(Red3, HIGH); digitalWrite(Blue3, LOW);
    } else {
      digitalWrite(Red3, HIGH); digitalWrite(Blue3, HIGH);
    }
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

  if(isActive1 || isActive2 || isActive3){
    // Algum LED vermelho aceso → toca buzzer
    tone(buzzer, 1000); // 1kHz
  } else {
    noTone(buzzer); // apaga buzzer
  }

  // Botões pressionados
  if(b1 and isActive1){
      isActive1 = false;      
      click1 = true;
      digitalWrite(Red1, HIGH);    
      digitalWrite(Blue1, LOW);    
  }
  if(b2 and isActive2){
      isActive2 = false;
      click2 = true;
      digitalWrite(Red2, HIGH);
      digitalWrite(Blue2, LOW);
  }
  if(b3 and isActive3){
      isActive3 = false;
      click3 = true;
      digitalWrite(Red3, HIGH);
      digitalWrite(Blue3, LOW);
  }

  if (agora == "00:00") {
    click1 = false;
    click2 = false;
    click3 = false;

    isActive1 = false;  
    isActive2 = false;  
    isActive3 = false;  
  }


  if(usarWiFi){
    delay(60000);
  }
}
