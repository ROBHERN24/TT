#include <Key.h>
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "TM1637Display.h"
#include <max6675.h>

//funciones
void girar_izquierda17(int pasos);
void girar_derecha17(int pasos);
void girar_izquierda23(int pasos);
void girar_derecha23(int pasos);
void mostrarInformacion();
void mostrarLCD();
void mostrarTemperatura();
void mostrarPresion();

//Constantes
const int vel17 = 500;
const int vel23 = 1000;
const int velH = 750;
float factor_conversion=7.5; //para convertir de frecuencia a caudal
int incremento = 15;  // Incremento o decremento en grados
int temperature_celsius;
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};
const byte FILAS = 4;
const byte COLUMNAS = 4;
char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', '10', '11', '12'},
  {'13', '14', '15', '16'},
};

//Variables
int movX =0;
int movY =0;
int pasos;
int Cont = 2;
int anguloActual =  0;
int angulo = 45;  // Ángulo inicial del servo
int paso = 1;
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
unsigned long tiempoUltimaLectura = 0; // Variable para almacenar el tiempo de la última lectura
const unsigned long INTERVALO_LECTURA = 1000;
float caudal_L_m;
//unsigned long time;

//Pines de entrada
int PinSensor = 2;    //Sensor conectado en el pin 2
int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;
byte pinFilas[FILAS] = {22, 23, 24, 25};
byte pinColumnas[COLUMNAS] = {26, 27, 28, 29};
const int ledCM = 30;
const int ledMM = 31;
const byte DIR17 = 32;
const byte PUL17 = 33;
const byte DIR23 = 34;
const byte PUL23 = 35;
const int homeSwitchXPin = 36;
const int homeSwitchYPin = 37;

//objetos
Keypad teclado = Keypad(makeKeymap(teclas), pinFilas, pinColumnas, FILAS, COLUMNAS);
LiquidCrystal_I2C lcd(0x27,16,2);
TM1637Display displayTemp(6,7);
TM1637Display displayPres(8,9);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Servo servo1;  
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
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
  Serial.begin(9600);
  pinMode(DIR17, OUTPUT);
  pinMode(PUL17, OUTPUT);
  pinMode(DIR23, OUTPUT);
  pinMode(PUL23, OUTPUT);
  pinMode(ledCM, OUTPUT);
  pinMode(ledMM, OUTPUT);
  pinMode(homeSwitchXPin, INPUT_PULLUP);
  pinMode(homeSwitchYPin, INPUT_PULLUP);
  attachInterrupt(0,ContarPulsos,RISING);
  servo1.write(90);
  servo2.write(90);
  servo3.write(45);
  servo4.write(0);
  //homeX();
  //homeY();
  delay(500);
}

void loop() {
  /*Serial.println("Tiempo:  ");
  time = millis();*/
  char tecla = teclado.getKey();
  unsigned long tiempoActual = millis(); // Obtener el tiempo actual
  if (tiempoActual - tiempoUltimaLectura >= INTERVALO_LECTURA) {// Verificar si es hora de realizar una nueva lectura de los sensores
    mostrarInformacion();// Realizar la lectura de sensores
    tiempoUltimaLectura = tiempoActual;// Actualizar el tiempo de la última lectura
  }

  if (Cont % 2 == 0) {
    digitalWrite(ledCM, HIGH);
    digitalWrite(ledMM, LOW);
  } else {
    digitalWrite(ledMM, HIGH);
    digitalWrite(ledCM, LOW);
  }
  switch(tecla) {
    case NO_KEY:
      // No se ha presionado ninguna tecla
      break;
    case '1':
      servo1.write(50);  // enviamos el valor escalado al servo.
      Serial.println("pos1-1");
      break;
    case '2':
      servo1.write(90);  // enviamos el valor escalado al servo.
      Serial.println("pos2-1");
      break;
    case '3':
      servo1.write(130);  // enviamos el valor escalado al servo.
      Serial.println("pos3-1");
      break;
    case '4':
      servo2.write(45);  // enviamos el valor escalado al servo.
      Serial.println("pos1-2");
      break;
    case '5'://
      servo2.write(90);  // enviamos el valor escalado al servo.
      Serial.println("pos2-2");
      break;
    case '6'://
      servo2.write(135);  // enviamos el valor escalado al servo.
      Serial.println("pos3-2");
      break;
    case '7'://
      Cont++;
      break;
    case '8'://
      if (Cont % 2 == 0) {
      Serial.println("der17");
      girar_izquierda17(200);
    } else {
      Serial.println("mmder17");
      girar_izquierda17(20);
    }
      break;
    case '9'://
      if(digitalRead(homeSwitchXPin) == HIGH){
        if (Cont % 2 == 0) {
          Serial.println("izq17");
          girar_derecha17(200);
        } else {
          Serial.println("mmizq17");
          girar_derecha17(20);
        }
      }
      break;
    case '10'://
      if (Cont % 2 == 0) {
      Serial.println("up23");
      girar_izquierda23(200);
    } else {
      Serial.println("mmup23");
      girar_izquierda23(20);
    }
      break;
    case '11'://
      if(digitalRead(homeSwitchYPin) == HIGH){
        if (Cont % 2 == 0) {
        Serial.println("down23");
        girar_derecha23(200);
      } else {
        Serial.println("mmdown23");
        girar_derecha23(20);
      }
    }
      break;
    case '12'://bomba encendido
      
      break;
    case '13'://aumentar
      
      break;
    case '14'://disminuir
      
      break;
    case '15': //bomba apagado
      
      break;
    case '16'://pos1-1
      
      break;
/*  case '':
      
      break;*/
    default:
      // Si se presiona una tecla no especificada
      Serial.println("Tecla no reconocida");
  }
  delay(100); // Pequeño retardo para evitar rebotes del teclado  
}


void homeX() {
  Serial.println("Homing eje X...");
  while (movX == 0) {
    digitalWrite(DIR17, HIGH);  // Mover el motor en la dirección negativa
    digitalWrite(PUL17, HIGH);
    delayMicroseconds(velH);
    digitalWrite(PUL17, LOW);
    delayMicroseconds(velH);
    if(digitalRead(homeSwitchXPin) == LOW){
      delay(20);
      if(digitalRead(homeSwitchXPin) == LOW){
        movX =1;
      }
    }
  }
  while (movX == 1) {
    digitalWrite(DIR17, LOW);  // Mover el motor en la dirección negativa
    digitalWrite(PUL17, HIGH);
    delayMicroseconds(velH);
    digitalWrite(PUL17, LOW);
    delayMicroseconds(velH);
    if(digitalRead(homeSwitchXPin) == HIGH){
      delay(20);
      if(digitalRead(homeSwitchXPin) == HIGH){
        movX =0;
      }
    }
  }
  Serial.println("Eje X homing completado.");
}

void homeY() {
  Serial.println("Homing eje Y...");
  while (movY == 0) {
    digitalWrite(DIR23, HIGH);  // Mover el motor en la dirección negativa
    digitalWrite(PUL23, HIGH);
    delayMicroseconds(vel23);
    digitalWrite(PUL23, LOW);
    delayMicroseconds(vel23);
    if(digitalRead(homeSwitchYPin) == LOW){
      delay(20);
      if(digitalRead(homeSwitchYPin) == LOW){
        movY =1;
      }
    }
  }
  while (movY == 1) {
    digitalWrite(DIR23, LOW);  // Mover el motor en la dirección negativa
    digitalWrite(PUL23, HIGH);
    delayMicroseconds(vel23);
    digitalWrite(PUL23, LOW);
    delayMicroseconds(vel23);
        if(digitalRead(homeSwitchXPin) == HIGH){
      delay(20);
      if(digitalRead(homeSwitchXPin) == HIGH){
        movY =0;
      }
    }
  }
  Serial.println("Eje Y homing completado.");
}

void girar_izquierda17(int pasos){
digitalWrite(DIR17, LOW);
delayMicroseconds(10);
for(int j=0; j<pasos; j++){
  digitalWrite(PUL17, HIGH);
  delayMicroseconds(vel17);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel17);
  }
}

void girar_derecha17(int pasos){
digitalWrite(DIR17, HIGH);
delayMicroseconds(10);
for(int i=0; i<pasos; i++){
  digitalWrite(PUL17, HIGH);
  delayMicroseconds(vel17);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel17);
  }
}
void girar_izquierda23(int pasos){
digitalWrite(DIR23, LOW);
delayMicroseconds(10);
for(int j=0; j<pasos; j++){
  digitalWrite(PUL23, HIGH);
  delayMicroseconds(vel23);
  digitalWrite(PUL23, LOW);
  delayMicroseconds(vel23);
  }
}

void girar_derecha23(int pasos){
digitalWrite(DIR23, HIGH);
delayMicroseconds(10);
for(int i=0; i<pasos; i++){
  digitalWrite(PUL23, HIGH);
  delayMicroseconds(vel23);
  digitalWrite(PUL23, LOW);
  delayMicroseconds(vel23);
  }
}
void mostrarInformacion() {
  // Leer la temperatura del termopar
  temperature_celsius = thermocouple.readCelsius();
  //float frecuencia=ObtenerFrecuencia(); //obtenemos la Frecuencia de los pulsos en Hz
  //float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
 // float caudal_L_h=caudal_L_m*60; //calculamos el caudal en L/h
  mostrarLCD();
  mostrarTemperatura();
  mostrarPresion();
}

void mostrarLCD(){
  long tiempo=millis(); //tiempo antes de iniciar la lectura
  int Dx_cm = distanciax(20); //lectura de distancia
  int Dy_cm = distanciay(20);
  //Serial.println(Dx_cm);
  //Serial.println(Dy_cm);
  lcd.setCursor(0,0);
  lcd.print("X:");//pocision x
  lcd.print(Dx_cm);
  lcd.print("cm ");
  lcd.setCursor(8,0);
  lcd.print("Y:");//posicion y
  lcd.print(Dy_cm);
  lcd.print("cm ");
  lcd.setCursor(0,1);
  lcd.print("W:");
  lcd.print(servo1.read()); //1 decimal
  lcd.print("\337"); // "337" -> "°"
  lcd.setCursor(8,1);
  lcd.print("P:");
  lcd.print(servo2.read());
  lcd.print("\337");
}

void mostrarTemperatura() {// Actualizar la pantalla TM1637 con la temperatura
  displayTemp.showNumberDec(temperature_celsius, false, 2, 0);
  displayTemp.setSegments(celsius, 2, 2);
}

void mostrarPresion() {// Actualizar la pantalla TM1637 con la Presion
  displayPres.showNumberDec(caudal_L_m, false, 4, 0);
}

float distanciax(int n)
{
  long suma1=0;
  for(int i=0;i<n;i++)
  {
    suma1=suma1+analogRead(A0);
  }  
  float adc1=suma1/n;
  float distanciax_cm = 17569.7 * pow(adc1, -1.2062);
  return(distanciax_cm);
}

float distanciay(int n)
{
  long suma2=0;
  for(int i=0;i<n;i++)
  {
    suma2=suma2+analogRead(A1);
  }  
  float adc2=suma2/n;
  float distanciay_cm = 17569.7 * pow(adc2, -1.2062);
  return(distanciay_cm);
}

void ContarPulsos ()
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

int ObtenerFrecuencia() 
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Desabilitamos las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}

