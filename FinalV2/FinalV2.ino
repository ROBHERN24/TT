#include <Key.h>
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "TM1637Display.h"
#include <max6675.h>

// Definición de constantes
const int vel17 = 500;
const int vel23 = 1000;
const int velH = 500;
const int incremento = 15;  // Incremento o decremento en grados
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};
const byte FILAS = 5;
const byte COLUMNAS = 5;
char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', 'A'},
  {'B', 'C', 'D', 'E', 'F'},
  {'G', 'H', 'I', 'J', 'K'},
  {'L', 'M', 'N', 'O', 'P'}
};

// Definición de pines de entrada y salida
const int PinCaudal = 2;    // Sensor conectado en el pin 2
const int thermoDO = 3;
const int thermoCS = 4;
const int thermoCLK = 5;
const int ledMM = 22;
const int ledCM = 24;
byte pinFilas[FILAS] = {23, 25, 27, 29, 31};           
byte pinColumnas[COLUMNAS] = {33, 35, 37, 39, 41};
const int homeSwitchXPin = 43;
const int homeSwitchYPin = 45;
const byte DIR17 = 47;
const byte PUL17 = 49;
const byte DIR23 = 51;
const byte PUL23 = 53;

// Variables globales
int movX = 0;
int movY = 0;
int pasos;
int Cont = 2;
int anguloActual =  0;
int angulo = 45;  // Ángulo inicial del servo
int paso = 1;
unsigned long tiempoUltimaLectura = 0; // Variable para almacenar el tiempo de la última lectura
const unsigned long INTERVALO_LECTURA = 1000;
volatile int pulseCount = 0;  // Contador de pulsos
float flowRate = 0.0;         // Tasa de flujo (L/min)
unsigned long oldTime = 0;    // Para almacenar el tiempo anterior

// Objetos de hardware
Keypad teclado = Keypad(makeKeymap(teclas), pinFilas, pinColumnas, FILAS, COLUMNAS);
LiquidCrystal_I2C lcd(0x27,16,2);
TM1637Display displayTemp(6,7);
TM1637Display displayPres(8,9);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Servo servo1;  
Servo servo2;
Servo servo3;
Servo servo4;

// Prototipos de funciones
void girarMotor(byte dirPin, byte pulPin, bool direccion, int pasos, int velocidad);
void mostrarInformacion();
void mostrarLCD();
void mostrarTemperatura();
void mostrarPresion();
//void Caudal();
void pulseCounter();
void homeX();
void homeY();

void setup() {
  Serial.begin(9600);
   // Configuramos los pines de las filas como entradas con resistencias pull-up internas
  for (byte i = 0; i < FILAS; i++) {
    pinMode(pinFilas[i], INPUT_PULLUP);
  }
  // Configuramos los pines de las columnas como salidas y los ponemos en LOW inicialmente
  for (byte j = 0; j < COLUMNAS; j++) {
    pinMode(pinColumnas[j], OUTPUT);
    digitalWrite(pinColumnas[j], LOW);
  }
  servo1.attach(10);  
  servo2.attach(11);
  servo3.attach(12);
  servo4.attach(13);
  lcd.init();
  lcd.backlight();
  displayTemp.setBrightness(7,true);
  displayTemp.clear();
  displayPres.setBrightness(7,true);
  displayPres.clear();
  pinMode(DIR17, OUTPUT);
  pinMode(PUL17, OUTPUT);
  pinMode(DIR23, OUTPUT);
  pinMode(PUL23, OUTPUT);
  pinMode(ledCM, OUTPUT);
  pinMode(ledMM, OUTPUT);
  pinMode(homeSwitchXPin, INPUT_PULLUP);
  pinMode(homeSwitchYPin, INPUT_PULLUP);
  //pinMode(PinCaudal, INPUT);
  //digitalWrite(PinCaudal, HIGH);
  //attachInterrupt(digitalPinToInterrupt(PinCaudal), pulseCounter, FALLING);
  servo1.write(90);
  servo2.write(90);
  servo3.write(45);
  servo4.write(0);
  //homeX();
  //homeY();
  delay(500);
}

void loop() {
  unsigned long tiempoActual = millis(); // Obtener el tiempo actual
  if (tiempoActual - tiempoUltimaLectura >= INTERVALO_LECTURA) { // Verificar si es hora de realizar una nueva lectura de los sensores
    mostrarInformacion(); // Realizar la lectura de sensores
    tiempoUltimaLectura = tiempoActual; // Actualizar el tiempo de la última lectura
  }

  digitalWrite(ledCM, Cont % 2 == 0 ? HIGH : LOW);
  digitalWrite(ledMM, Cont % 2 == 0 ? LOW : HIGH);

  char tecla = teclado.getKey();
  switch(tecla) {
    case NO_KEY:
      // No se ha presionado ninguna tecla
      break;
    case '1':// Avance rápido a la zona 6 lowizq
      // Implementar lógica
      break;
    case '2':// Bomba apagado
      anguloActual += paso;
      if (anguloActual <= 0) {
        anguloActual = 0; // Asegurarse de que el ángulo no sea menor que 0 grados
        paso = 1; // Cambiar la dirección del movimiento
      }
      servo4.write(anguloActual);
      break;
    case '3':// Movimiento a la izquierda en eje 17
      if(digitalRead(homeSwitchXPin) == HIGH){
        Serial.println(Cont % 2 == 0 ? "izq17" : "mmizq17");
        girarMotor(DIR17, PUL17, HIGH, Cont % 2 == 0 ? 250 : 25, vel17);
      }
      break;
    case '4':// Movimiento hacia abajo en eje 23
      if(digitalRead(homeSwitchYPin) == HIGH){
        Serial.println(Cont % 2 == 0 ? "down23" : "mmdown23");
        girarMotor(DIR23, PUL23, HIGH, Cont % 2 == 0 ? 250 : 25, vel23);
      }
      break;
    case '5':// Movimiento hacia arriba en eje 23
      Serial.println(Cont % 2 == 0 ? "up23" : "mmup23");
      girarMotor(DIR23, PUL23, LOW, Cont % 2 == 0 ? 250 : 25, vel23);
      break;
    case '6':// Avance rápido a la zona 5 lowizq
      // Implementar lógica
      break;
    case '7':// Bomba encendido
      anguloActual += paso;
      if (anguloActual >= 180) {
        anguloActual = 180; // Asegurarse de que el ángulo no exceda 180 grados
        paso = -1; // Cambiar la dirección del movimiento
      }
      servo4.write(anguloActual);
      break;
    case '8':// Aumentar presión
      angulo += incremento;  // Aumentamos el ángulo
      anguloActual += paso;
      if (anguloActual >= 180) {
        anguloActual = 180; // Asegurarse de que el ángulo no exceda 180 grados
        paso = -1; // Cambiar la dirección del movimiento
      }
      servo4.write(anguloActual);
      if (angulo > 90) {  // Verificamos si el ángulo supera el límite máximo
        angulo = 90;  // Limitamos el ángulo al máximo
      }
      servo3.write(angulo);  // Movemos el servo al nuevo ángulo
      break;
    case '9':// Posición 2-1
      servo1.write(90);  // Enviamos el valor escalado al servo.
      Serial.println("pos2-1");
      break;
    case 'A':// Movimiento a la derecha en eje 17
      Serial.println(Cont % 2 == 0 ? "der17" : "mmder17");
      girarMotor(DIR17, PUL17, LOW, Cont % 2 == 0 ? 250 : 25, vel17);
      break;
    case 'B':// Avance rápido a la zona 3 midizq
      // Implementar lógica
      break;
    case 'C':// Avance rápido a la zona 4 midder
      // Implementar lógica
      break;
    case 'D':// Disminuir presión
      angulo -= incremento;  // Disminuimos el ángulo
      if (angulo < 0) {  // Verificamos si el ángulo es menor que cero
        angulo = 0;  // Limitamos el ángulo al mínimo
        anguloActual += paso;
        if (anguloActual <= 0) {
          anguloActual = 0; // Asegurarse de que el ángulo no sea menor que 0 grados
          paso = 1; // Cambiar la dirección del movimiento
        }
        servo4.write(anguloActual);
      }
      servo3.write(angulo);  // Movemos el servo al nuevo ángulo
      break;
    case 'E':// Posición 2-2
      servo2.write(90);  // Enviamos el valor escalado al servo.
      Serial.println("pos2-2");
      break;
    case 'F': // Cambiar entre CM y MM
      Cont++;
      break;
    case 'G':// Avance rápido a la zona 1 upizq
      // Implementar lógica
      break;
    case 'H':// Avance rápido a la zona 2 upder 
      // Implementar lógica
      break;
    case 'I':// Secuencia 2 
      // Implementar lógica
      break;
    case 'J':// Posición 1-2
      servo2.write(45);  // Enviamos el valor escalado al servo.
      Serial.println("pos1-2");
      break;
    case 'K':// Posición 3-1
      servo1.write(135);  // Enviamos el valor escalado al servo.
      Serial.println("pos3-1");
      break;
    /*case 'L':// Implementar lógica
      Serial.println("L");
      break;
    case 'M':
      // Implementar lógica
      Serial.println("M");
      break;*/
    case 'N':// Secuencia 1
      // Implementar lógica
      break;
    case 'O'://pos1-1
      servo1.write(45);  // enviamos el valor escalado al servo.
      Serial.println("pos1-1");
      break;
    case 'P'://pos3-2
      servo2.write(135);  // enviamos el valor escalado al servo.
      Serial.println("pos3-2");
      break;
    default:
      // Si se presiona una tecla no especificada
      Serial.println("Tecla no reconocida");
  }
  delay(100); // Pequeño retardo para evitar rebotes del teclado  
}

void homeX() {
  Serial.println("Homing eje X...");
  while (movX == 0) {
    girarMotor(DIR17, PUL17, HIGH, 1, velH);
    if(digitalRead(homeSwitchXPin) == LOW){
      delay(20);
      if(digitalRead(homeSwitchXPin) == LOW){
        movX = 1;
      }
    }
  }
  Serial.println("Eje X homing completado.");
}

void homeY() {
  Serial.println("Homing eje Y...");
  while (movY == 0) {
    girarMotor(DIR23, PUL23, HIGH, 1, velH);
    if(digitalRead(homeSwitchYPin) == LOW){
      delay(20);
      if(digitalRead(homeSwitchYPin) == LOW){
        movY = 1;
      }
    }
  }
  Serial.println("Eje Y homing completado.");
}

void girarMotor(byte dirPin, byte pulPin, bool direccion, int pasos, int velocidad) {
  digitalWrite(dirPin, direccion);
  delayMicroseconds(10);
  for (int i = 0; i < pasos; i++) {
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(velocidad);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(velocidad);
  }
}

void mostrarInformacion() {
  mostrarLCD();
  mostrarTemperatura();
  mostrarPresion();
}

void mostrarLCD(){
  lcd.setCursor(0,0);
  lcd.print("X:");// Posición X
  // lcd.print(Dx_cm);  // Aquí deberías mostrar la posición X actual
  lcd.print("cm ");
  lcd.setCursor(8,0);
  lcd.print("Y:");// Posición Y
  // lcd.print(Dy_cm);  // Aquí deberías mostrar la posición Y actual
  lcd.print("cm ");
  lcd.setCursor(0,1);
  lcd.print("W:");
  lcd.print(servo1.read()); // Ángulo del servo1
  lcd.print("\337"); // "337" -> "°"
  lcd.setCursor(8,1);
  lcd.print("P:");
  lcd.print(servo2.read()); // Ángulo del servo2
  lcd.print("\337");
}

void mostrarTemperatura() {
  float temp = thermocouple.readCelsius();
  displayTemp.showNumberDec(temp, false, 2, 0);
  displayTemp.setSegments(celsius, 2, 2);
}
/*
void Caudal() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - oldTime;
  if (elapsedTime >= 1000) {  // Cada 1 segundo
    detachInterrupt(digitalPinToInterrupt(PinCaudal));  // Desactivar interrupción temporalmente
    flowRate = ((1000.0 / elapsedTime) * pulseCount) / 7.5;  // Calcular la tasa de flujo
    pulseCount = 0;  // Reiniciar contador de pulsos
    oldTime = currentTime;  // Actualizar el tiempo anterior
    attachInterrupt(digitalPinToInterrupt(PinCaudal), pulseCounter, FALLING);  // Reactivar interrupción
  }
}*/

void mostrarPresion() {
  displayPres.showNumberDec(flowRate, false, 4, 0);
}
/*
void pulseCounter() {
  pulseCount++;
}
*/