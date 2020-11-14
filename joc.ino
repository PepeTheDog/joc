#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

class perete{
  private:
    uint8_t coordX;
    uint8_t coordY;
  public:
    perete(){
      coordX = 
}

void setup(){
  lcd.init();
  lcd.backlight();
  lcd.priht("Arduino Game");
  lcd.clear();
}

void loop(){
  
}
