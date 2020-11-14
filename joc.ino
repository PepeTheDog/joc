#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

uint8_t tastatura(uint8_t buttonValue){
  if (buttonValue<20)
    return 1;
  if (buttonValue>=20 && buttonValue<35)
    return 2;
  if (buttonValue>=35 && buttonValue<50)
    return 3;
}

void setup(){
  lcd.init();
  lcd.backlight();
  lcd.priht("Arduino Game");
  delay(500);
  lcd.clear();
  randomSeed(analogRead(A0));
  Serial.begin(9600);
}

void loop(){
  if (tastatura(analogRead(A5)) == 1)
    Serial.println("buton1");
}
