const byte DIR17 = 50; //x
const byte PUL17 = 51;
const byte DIR23 = 52; //y
const byte PUL23 = 53;

void homeX();
void homeY();
void singleStep(int stepPin);

// Pines de los interruptores de fin de carrera
const int homeSwitchXPin = 9;
const int homeSwitchYPin = 10;

// Variables para la velocidad del motor
const int motorSpeed = 200;  // Ajusta este valor para cambiar la velocidad

void setup() {
  Serial.begin(9600);

  // Configurar los pines de los motores como salidas
  pinMode(DIR17, OUTPUT);
  pinMode(PUL17, OUTPUT);
  pinMode(DIR23, OUTPUT);
  pinMode(PUL23, OUTPUT);

  // Configurar los interruptores de fin de carrera como entradas
  pinMode(homeSwitchXPin, INPUT_PULLUP);
  pinMode(homeSwitchYPin, INPUT_PULLUP);

  // Realizar el homing de los ejes
  homeX();
  homeY();

  // Ahora los motores están en el punto home (0,0)
  Serial.println("Homing completado. Motores en la posición de inicio.");
}

void loop() {
  // Tu código principal aquí
}

void homeX() {
  Serial.println("Homing eje X...");
  while (digitalRead(homeSwitchXPin)) {
    digitalWrite(DIR17, LOW);  // Mover el motor en la dirección negativa
    singleStep(PUL17);
    delayMicroseconds(motorSpeed);
  }
    while (!digitalRead(homeSwitchXPin)) {
    digitalWrite(DIR17, HIGH);  // Mover el motor en la dirección negativa
    singleStep(PUL17);
    delayMicroseconds(motorSpeed);
  }
  Serial.println("Eje X homing completado.");
}

void homeY() {
  Serial.println("Homing eje Y...");
  while (digitalRead(homeSwitchYPin)) {
    digitalWrite(DIR23, LOW);  // Mover el motor en la dirección negativa
    singleStep(PUL23);
    delayMicroseconds(motorSpeed);
  }
  while (!digitalRead(homeSwitchYPin)) {
    digitalWrite(DIR23, HIGH);  // Mover el motor en la dirección negativa
    singleStep(PUL23);
    delayMicroseconds(motorSpeed);
  }
  Serial.println("Eje Y homing completado.");
}

void singleStep(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(10);
}