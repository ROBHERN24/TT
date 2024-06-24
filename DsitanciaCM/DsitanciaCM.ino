#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // Comunicación seria a 9600 baudios
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}


void loop() {
  long tiempo=millis(); //tiempo antes de iniciar la lectura
  int Dx_cm=distanciax(20); //lectura de distancia
  int Dy_cm=distanciay(20);
  Serial.println(Dx_cm);
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
  lcd.print("20"); //1 decimal
  lcd.print("\337"); // "337" -> "°"
  lcd.setCursor(8,1);
  lcd.print("P:");
  lcd.print("15");
  lcd.print("\337");
  delay(100);
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