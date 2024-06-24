#include <Servo.h>

Servo servo1;  
Servo servo2;
Servo servo3;

int angulo = 90;  // Ángulo inicial del servo
int incremento = 15;  // Incremento o decremento en grados

const int Aumentar = 28;  // Pin donde está conectado el botón para aumentar el ángulo
const int Disminuir = 29;  // Pin donde está conectado el botón para disminuir el ángulo
const int Pos1_1 = 30;
const int Pos2_1 = 31;
const int Pos3_1 = 32;
const int Pos1_2 = 33;
const int Pos2_2 = 34;
const int Pos3_2 = 35;

void setup() {
  Serial.begin(9600);
  pinMode(Aumentar, INPUT); 
  pinMode(Disminuir, INPUT);
  pinMode(Pos1_1, INPUT);
  pinMode(Pos2_1, INPUT);
  pinMode(Pos3_1, INPUT);
  pinMode(Pos1_2, INPUT);
  pinMode(Pos2_2, INPUT);
  pinMode(Pos3_2, INPUT);
  servo1.attach(10);  
  servo2.attach(11);
  servo3.attach(12);
}
 
void loop() {
  if (digitalRead(Aumentar) == HIGH) {  // Verificamos si se presiona el botón de aumentar
    angulo += incremento;  // Aumentamos el ángulo
    if (angulo > 90) {  // Verificamos si el ángulo supera el límite máximo
      angulo = 90;  // Limitamos el ángulo al máximo
    }
    servo3.write(angulo);  // Movemos el servo al nuevo ángulo
    Serial.println(servo3.read());
    delay(200);  // Pequeño retardo para evitar rebotes del botón
  }

  if (digitalRead(Disminuir) == HIGH) {  // Verificamos si se presiona el botón de disminuir
    angulo -= incremento;  // Disminuimos el ángulo
    if (angulo < 0) {  // Verificamos si el ángulo es menor que cero
      angulo = 0;  // Limitamos el ángulo al mínimo
    }
    servo3.write(angulo);  // Movemos el servo al nuevo ángulo
    Serial.println(servo3.read());
    delay(200);  // Pequeño retardo para evitar rebotes del botón
  }
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
  delay(100);
}
  