// LED RGB 1
const int Blue1 = 32;
const int Red1  = 33;

// LED RGB 2
const int Blue2 = 25;
const int Red2  = 26;

// LED RGB 3
const int Blue3 = 27;
const int Red3  = 14;

void setup() {
  // Configura todos como saída
  pinMode(Red1, OUTPUT);
  pinMode(Blue1, OUTPUT);

  pinMode(Red2, OUTPUT);
  pinMode(Blue2, OUTPUT);

  pinMode(Red3, OUTPUT);
  pinMode(Blue3, OUTPUT);
}

void loop() {
  // 🔵 Liga todos os azuis
  digitalWrite(Blue1, HIGH);
  digitalWrite(Blue2, HIGH);
  digitalWrite(Blue3, HIGH);

  digitalWrite(Red1, LOW);
  digitalWrite(Red2, LOW);
  digitalWrite(Red3, LOW);

  delay(1000);

  // 🔴 Liga todos os vermelhos
  digitalWrite(Blue1, LOW);
  digitalWrite(Blue2, LOW);
  digitalWrite(Blue3, LOW);

  digitalWrite(Red1, HIGH);
  digitalWrite(Red2, HIGH);
  digitalWrite(Red3, HIGH);

  delay(1000);

  // 🟣 Liga todos vermelho + azul (roxo)
  digitalWrite(Blue1, HIGH);
  digitalWrite(Blue2, HIGH);
  digitalWrite(Blue3, HIGH);

  digitalWrite(Red1, HIGH);
  digitalWrite(Red2, HIGH);
  digitalWrite(Red3, HIGH);

}
