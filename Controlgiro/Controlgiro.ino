void girar_derecha17();
void girar_izquierda17();
void girar_derecha23();
void girar_izquierda23();
void mm_derecha17();
void mm_izquierda17();
void mm_derecha23();
void mm_izquierda23();
/*
void mov_upder();
void mov_midder();
void mov_lowder();
void mover_upizq();
void mover_midizq();
void mover_lowizq();*/
int Cont = 2;
const int vel17 = 1500;
const int vel23 = 1500;
//botones
const int der17 = 22;
const int izq17 = 23;
const int up23 = 24;
const int down23 = 25;
const int CM_MM = 26;
/*
const int upder = 7;
const int midder = 8;
const int lowder= 9;
const int upizq = 10;
const int midizq = 11;
const int lowizq = 12;*/

const int ledCM = 48;
const int ledMM = 49;
const byte DIR17 = 50;
const byte PUL17 = 51;
const byte DIR23 = 52;
const byte PUL23 = 53;

void setup() {
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
  /*
  pinMode(upder, INPUT);
  pinMode(midder, INPUT);
  pinMode(lowder, INPUT);
  pinMode(upizq, INPUT);
  pinMode(midizq, INPUT);
  pinMode(lowizq, INPUT);*/
}

void loop() {
if (digitalRead(CM_MM) == HIGH){
Cont++;
}
if (Cont % 2==0){
  digitalWrite(ledCM, HIGH);
  digitalWrite(ledMM, LOW);
  if (digitalRead(der17) == HIGH){
  Serial.println("der17");
  girar_izquierda17();
  }
  if (digitalRead(izq17) == HIGH){
  Serial.println("izq17");
  girar_derecha17();
  }
  
  if (digitalRead(up23) == HIGH){
    Serial.println("der23");
  girar_izquierda23();
  }
  if (digitalRead(down23) == HIGH){
    Serial.println("izq23");
  girar_derecha23();
  }
}
else{
  digitalWrite(ledMM, HIGH);
  digitalWrite(ledCM, LOW);
  if (digitalRead(der17) == HIGH){
  Serial.println("mmder17");
  mm_izquierda17();
  }
  if (digitalRead(izq17) == HIGH){
  Serial.println("mmizq17");
  mm_derecha17();
  }
  if (digitalRead(up23) == HIGH){
    Serial.println("mmder23");
  mm_izquierda23();
  }
  if (digitalRead(down23) == HIGH){
    Serial.println("mmizq23");
  mm_derecha23();
  }
}
  /*
  if (digitalRead(upder) == HIGH){
  Serial.println("upder");
  mov_upder();
  }
  if (digitalRead(midder) == HIGH){
  Serial.println("midder");
  mov_midder();
  }
  if (digitalRead(lowder) == HIGH){
    Serial.println("lowder");
  mov_lowder();
  }
  if (digitalRead(upizq) == HIGH){
  Serial.println("upizq");
  mov_izq();
  }
  if (digitalRead(midizq) == HIGH){
  Serial.println("midizq");
  mov_midizq();
  }
  if (digitalRead(lowizq) == HIGH){
    Serial.println("lowizq");
  mov_lowizq();
  }
  */
  delay(100);
}

void girar_izquierda17(){
digitalWrite(DIR17, LOW);
delayMicroseconds(10);
for(int j=0; j<200; j++){
  digitalWrite(PUL17, HIGH);
  delayMicroseconds(vel17);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel17);
  }
}

void girar_derecha17(){
digitalWrite(DIR17, HIGH);
delayMicroseconds(10);
for(int i=0; i<200; i++){
  digitalWrite(PUL17, HIGH);
  delayMicroseconds(vel17);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel17);
  }
}

void girar_izquierda23(){
digitalWrite(DIR23, LOW);
delayMicroseconds(10);
for(int j=0; j<200; j++){
  digitalWrite(PUL23, HIGH);
  delayMicroseconds(vel23);
  digitalWrite(PUL23, LOW);
  delayMicroseconds(vel23);
  }
}

void girar_derecha23(){
digitalWrite(DIR23, HIGH);
delayMicroseconds(10);
for(int i=0; i<200; i++){
  digitalWrite(PUL23, HIGH);
  delayMicroseconds(vel23);
  digitalWrite(PUL23, LOW);
  delayMicroseconds(vel23);
  }
}

void mm_izquierda17(){
digitalWrite(DIR17, LOW);
delayMicroseconds(10);
for(int j=0; j<20; j++){
  digitalWrite(PUL17, HIGH);
  delayMicroseconds(vel17);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel17);
  }
}

void mm_derecha17(){
digitalWrite(DIR17, HIGH);
delayMicroseconds(10);
for(int i=0; i<20; i++){
  digitalWrite(PUL17, HIGH);
  delayMicroseconds(vel17);
  digitalWrite(PUL17, LOW);
  delayMicroseconds(vel17);
  }
}

void mm_izquierda23(){
digitalWrite(DIR23, LOW);
delayMicroseconds(10);
for(int j=0; j<20; j++){
  digitalWrite(PUL23, HIGH);
  delayMicroseconds(vel23);
  digitalWrite(PUL23, LOW);
  delayMicroseconds(vel23);
  }
}

void mm_derecha23(){
digitalWrite(DIR23, HIGH);
delayMicroseconds(10);
for(int i=0; i<20; i++){
  digitalWrite(PUL23, HIGH);
  delayMicroseconds(vel23);
  digitalWrite(PUL23, LOW);
  delayMicroseconds(vel23);
  }
}
