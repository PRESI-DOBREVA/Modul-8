#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 6, 8, 9, 10, 11); 

int state;
int maxTemp = 0;              
unsigned long readyStartTime = 0;
const unsigned long readyDuration = 10000; 
bool maxTempSet = false; 
void setup() 
{
  lcd.begin(16, 2);
  pinMode(4, OUTPUT);          
  pinMode(2, OUTPUT);        
  state = 1;
  lcd.clear();
  lcd.print("Set Max T:");
  lcd.setCursor(0, 1);
  lcd.print("Max T: ");
}

void loop() 
{
  float currentTemp = readTemp();
  int potValue = analogRead(A1);
  int tempFromPot = map(potValue, 0, 1023, 0, 100); 
  switch (state) 
  {
    case 1:
      maxTemp = tempFromPot; 
      lcd.setCursor(7, 1); 
      lcd.print(maxTemp);
      lcd.print(" C "); 
      static unsigned long setupStartTime = millis();
      if (!maxTempSet && millis() - setupStartTime > 5000) 
      { 
          maxTempSet = true; 
          state = 2;
          lcd.clear();
          lcd.print("Max T Set: ");
          lcd.print(maxTemp);
          lcd.print(" C");
          delay(2000); 
      } 
      else if (maxTempSet)
      { 
          state = 2;
      }
      break;

    case 2:
      lcd.setCursor(0, 0); 
      lcd.print("Current T: ");
      lcd.print(currentTemp);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Max T: ");
      lcd.print(maxTemp);
      lcd.print(" C");
      if (currentTemp >= maxTemp && maxTemp != 0) 
      {
        state = 3;
        readyStartTime = millis(); 
        lcd.clear();
        lcd.print("Ready!");
        digitalWrite(4, HIGH); 
      }
      break;

    case 3:
      lcd.setCursor(0, 0);
      lcd.print("Ready!"); 
      lcd.setCursor(0, 1);
      lcd.print("Time left: ");
      lcd.print((readyDuration - (millis() - readyStartTime)) / 1000); 
      lcd.print("s");
      if (millis() - readyStartTime >= readyDuration) 
      {
        state = 4;
        digitalWrite(4, LOW);
        lcd.clear();
        lcd.print("End");
        tone(2, 1000); 
      }
      break;

    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("End");
      delay(3000); 
      noTone(2);  
      break;
  }
  delay(50); 
}
float readTemp() {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1024.0); 
  float temp = (voltage - 0.5) * 100.0; 
  return temp;
}