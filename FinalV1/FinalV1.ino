//librerias
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "TM1637Display.h"
#include <max6675.h>

//funciones
void mostrarInformacion();
void mostrarLCD();
void mostrarTemperatura();
void mostrarPresion();
void girar_izquierda17(int pasos);
void girar_derecha17(int pasos);
void girar_izquierda23(int pasos);
void girar_derecha23(int pasos);


//Constantes
const int vel17 = 1500;
const int vel23 = 1500;
float factor_conversion=7.5; //para convertir de frecuencia a caudal
int incremento = 15;  // Incremento o decremento en grados
int temperature_celsius;
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};

//Variables
int pasos;
int Cont = 2;
int anguloActual =  0;
int angulo = 45;  // Ángulo inicial del servo
int paso = 1;
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
unsigned long tiempoUltimaLectura = 0; // Variable para almacenar el tiempo de la última lectura
const unsigned long INTERVALO_LECTURA = 1000;
float caudal_L_m;

//Pines de entrada
int PinSensor = 2;    //Sensor conectado en el pin 2
int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;
const int der17 = 22;
const int izq17 = 23;
const int up23 = 24;
const int down23 = 25;
const int CM_MM = 26;
const int upder = 27;
const int midder = 28;
const int lowder= 29;
const int upizq = 30;
const int midizq = 31;
const int lowizq = 32;
const int Aumentar = 34;  // Pin donde está conectado el botón para aumentar el ángulo
const int Disminuir = 35;  // Pin donde está conectado el botón para disminuir el ángulo
const int BombaOn = 36;
const int BombaOff = 37;
const int Pos1_1 = 38;
const int Pos2_1 = 39;
const int Pos3_1 = 40;
const int Pos1_2 = 41;
const int Pos2_2 = 42;
const int Pos3_2 = 43;

const int ledCM = 48;
const int ledMM = 49;
const byte DIR17 = 50;
const byte PUL17 = 51;
const byte DIR23 = 52;
const byte PUL23 = 53;
//objetos
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
  pinMode(der17, INPUT);
  pinMode(izq17, INPUT);
  pinMode(up23, INPUT);
  pinMode(down23, INPUT);
  pinMode(CM_MM, INPUT);
  pinMode(Aumentar, INPUT); 
  pinMode(Disminuir, INPUT);
  pinMode(BombaOn, INPUT);
  pinMode(BombaOff, INPUT);
  pinMode(Pos1_1, INPUT);
  pinMode(Pos2_1, INPUT);
  pinMode(Pos3_1, INPUT);
  pinMode(Pos1_2, INPUT);
  pinMode(Pos2_2, INPUT);
  pinMode(Pos3_2, INPUT);
  pinMode(PinSensor, INPUT); 
  attachInterrupt(0,ContarPulsos,RISING);
  servo1.write(90);
  servo2.write(90);
  servo3.write(45);
  servo4.write(0);
  delay(500);
}

void loop() {
  unsigned long tiempoActual = millis(); // Obtener el tiempo actual
  if (tiempoActual - tiempoUltimaLectura >= INTERVALO_LECTURA) {// Verificar si es hora de realizar una nueva lectura de los sensores
    mostrarInformacion();// Realizar la lectura de sensores
    tiempoUltimaLectura = tiempoActual;// Actualizar el tiempo de la última lectura
  }
  //Movimiento manual
  if (digitalRead(CM_MM) == HIGH) {
    Cont++;
    delay(500);
  }
  
  if (Cont % 2 == 0) {
    digitalWrite(ledCM, HIGH);
    digitalWrite(ledMM, LOW);
  } else {
    digitalWrite(ledMM, HIGH);
    digitalWrite(ledCM, LOW);
  }
  
  if (digitalRead(izq17) == HIGH) {
    if (Cont % 2 == 0) {
      Serial.println("izq17");
      girar_izquierda17(250);
    } else {
      Serial.println("mmizq17");
      girar_izquierda17(25);
    }
  }
  
  if (digitalRead(der17) == HIGH) {
    if (Cont % 2 == 0) {
      Serial.println("derq17");
      girar_derecha17(250);
    } else {
      Serial.println("mmder17");
      girar_derecha17(25);
    }
  }
  
  if (digitalRead(up23) == HIGH) {
    if (Cont % 2 == 0) {
      Serial.println("izq23");
      girar_izquierda23(250);
    } else {
      Serial.println("mmizq23");
      girar_izquierda23(25);
    }
  }
  
  if (digitalRead(down23) == HIGH) {
    if (Cont % 2 == 0) {
      Serial.println("der23");
      girar_derecha23(250);
    } else {
      Serial.println("mmder23");
      girar_derecha23(25);
    }
  }
  //Avance rapido
  if (digitalRead(upder) == HIGH) {
    // Avance rápido a la zona 1
    girar_izquierda17(1000);
  }
  if (digitalRead(upizq) == HIGH) {
    // Avance rápido a la zona 2
 
  }
  if (digitalRead(midder) == HIGH) {
    // Avance rápido a la zona 3
 
  }
  if (digitalRead(midizq) == HIGH) {
    // Avance rápido a la zona 4

  }
  if (digitalRead(lowder) == HIGH) {
    // Avance rápido a la zona 5

  }
  if (digitalRead(lowizq) == HIGH) {
    // Avance rápido a la zona 6

  }
  //Servos
  servo1y2();
  servo3y4();
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
  delayMicroseconds(vel23);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel23);
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

void servo3y4(){
  if(digitalRead(BombaOn)== HIGH){
    anguloActual += paso;
    if (anguloActual >= 180) {
      anguloActual = 180; // Asegurarse de que el ángulo no exceda 180 grados
      paso = -1; // Cambiar la dirección del movimiento
    }
  servo4.write(anguloActual);
  }

  if (digitalRead(Aumentar) == HIGH) {  // Verificamos si se presiona el botón de aumentar
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
  }

  if (digitalRead(Disminuir) == HIGH) {  // Verificamos si se presiona el botón de disminuir
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
  }
  if(digitalRead(BombaOff) == HIGH){
    anguloActual += paso;
      if (anguloActual <= 0) {
        anguloActual = 0; // Asegurarse de que el ángulo no sea menor que 0 grados
        paso = 1; // Cambiar la dirección del movimiento
      }
      servo4.write(anguloActual);
  }
}
void servo1y2(){
  if (digitalRead(Pos1_1) == HIGH){
    servo1.write(45);  // enviamos el valor escalado al servo.
    Serial.println("pos1-1");
  }
  if (digitalRead(Pos2_1) == HIGH){
    servo1.write(90);  // enviamos el valor escalado al servo.
    Serial.println("pos2-1");
  }
  if (digitalRead(Pos3_1) == HIGH){
    servo1.write(135);  // enviamos el valor escalado al servo.
    Serial.println("pos3-1");
  }
  if (digitalRead(Pos1_2) == HIGH){
    servo2.write(45);  // enviamos el valor escalado al servo.
    Serial.println("pos1-2");
  }
  if (digitalRead(Pos2_2) == HIGH){
    servo2.write(90);  // enviamos el valor escalado al servo.
    Serial.println("pos2-2");
  }
  if (digitalRead(Pos3_2) == HIGH){
    servo2.write(135);  // enviamos el valor escalado al servo.
    Serial.println("pos3-2");
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
  int Dx_cm=distanciax(20); //lectura de distancia
  int Dy_cm=distanciay(20);
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