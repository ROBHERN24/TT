#include <Key.h>
#include <Keypad.h>

void girarMotor(byte dirPin, byte pulPin, bool direccion, int pasos, int velocidad);

int Cont = 2;
const int vel17 = 500;
const int vel23 = 1000;
int pasos;
const byte FILAS = 5;
const byte COLUMNAS = 5;
char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', 'A'},
  {'B', 'C', 'D', 'E', 'F'},
  {'G', 'H', 'I', 'J', 'K'},
  {'L', 'M', 'N', 'O', 'P'}
};

byte pinFilas[FILAS] = {23, 25, 27, 29, 31};           
byte pinColumnas[COLUMNAS] = {33, 35, 37, 39, 41};

const int ledCM = 22;
const int ledMM = 24;
const byte DIR17 = 47;
const byte PUL17 = 49;
const byte DIR23 = 51;
const byte PUL23 = 53;
Keypad teclado = Keypad(makeKeymap(teclas), pinFilas, pinColumnas, FILAS, COLUMNAS);


void setup() {
  Serial.begin(9600);
  pinMode(DIR17, OUTPUT);
  pinMode(PUL17, OUTPUT);
  pinMode(DIR23, OUTPUT);
  pinMode(PUL23, OUTPUT);
  pinMode(ledCM, OUTPUT);
  pinMode(ledMM, OUTPUT);

}

void loop() {
  char tecla = teclado.getKey();

  digitalWrite(ledCM, Cont % 2 == 0 ? HIGH : LOW);
  digitalWrite(ledMM, Cont % 2 == 0 ? LOW : HIGH);
  if (tecla) {
  switch(tecla) {
    case NO_KEY:
      // No se ha presionado ninguna tecla
      break;
    case '1':// Avance rápido a la zona 6 lowizq
      // Implementar lógica
      break;
    case '2':// Bomba apagado
     
      break;
    case '3':// Movimiento a la izquierda en eje 17
      Serial.println(Cont % 2 == 0 ? "izq17" : "mmizq17");
      girarMotor(DIR17, PUL17, HIGH, Cont % 2 == 0 ? 250 : 25, vel17);
      break;
    case '4':// Movimiento hacia abajo en eje 23
      Serial.println(Cont % 2 == 0 ? "down23" : "mmdown23");
      girarMotor(DIR23, PUL23, HIGH, Cont % 2 == 0 ? 250 : 25, vel23);
      break;
    case '5':// Movimiento hacia arriba en eje 23
      Serial.println(Cont % 2 == 0 ? "up23" : "mmup23");
      girarMotor(DIR23, PUL23, LOW, Cont % 2 == 0 ? 250 : 25, vel23);
      break;
    case '6':// Avance rápido a la zona 5 lowizq
      // Implementar lógica
      break;
    case '7':// Bomba encendido
      
      break;
    case '8':// Aumentar presión
      
      break;
    case '9':// Posición 2-1
      
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
      
      break;
    case 'E':// Posición 2-2
      
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
      
      break;
    case 'K':// Posición 3-1
      
      break;
    case 'L':// Implementar lógica
      Serial.println("L");
      break;
    case 'M':
      // Implementar lógica
      Serial.println("M");
      break;
    case 'N':// Secuencia 1
      // Implementar lógica
      break;
    default:
      // Si se presiona una tecla no especificada
      Serial.println("Tecla no reconocida");
  }
  delay(100); // Pequeño retardo para evitar rebotes del teclado  
}
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