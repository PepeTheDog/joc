#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

byte playerChar[] = {B01100, B01100, B00000, B01110, B11100, B01100, B11010, B10011};
byte obs[] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};

bool pause = false;
uint8_t playerHeight = 0;
uint8_t obsHeight1 = 0, obsHeight2 = 1;
uint8_t obsPos1, obsPos2;
uint8_t obsState = 1;
uint8_t nextObsDistance = 0;
unsigned long int timp = 0;

uint8_t tastatura(uint16_t buttonValue){
  if (buttonValue>25 && buttonValue<35)
    return 1;
  else if (buttonValue>=85 && buttonValue<95)
    return 2;
  else if (buttonValue>=345 && buttonValue<355)
    return 3;
}

void createObstacle(uint8_t t, uint8_t distance, uint8_t oHeight, uint8_t oPos){
  if (distance == 0){
    distance = random(12,16);
    if (oHeight == 0)
      oHeight = 1;
    else oHeight = 0;
  }
  if (t%(distance*500) == 0){
    oPos = 16;
    lcd.setCursor(oPos,oHeight);
    lcd.print((byte)1);
  }
}

void moveChar(uint8_t pHeight){
  if (tastatura(analogRead(A1)) == 1)
    pHeight = 0;
  else if (tastatura(analogRead(A1)) == 2)
    pHeight = 1;
  lcd.setCursor(0,pHeight);
  lcd.print((byte)0);
}

void collision(uint8_t oPos, uint8_t oHeight, uint8_t pHeight){
  if (oPos<=0 && oHeight == pHeight)
    pause = true;
}

void setup(){
  lcd.init();
  lcd.backlight();
  lcd.print("Arduino Game");
  delay(500);
  lcd.clear();
  lcd.createChar(0, playerChar);
  lcd.createChar(1, obs);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
}

void loop(){
  if (pause == false){
    if (millis() - timp >= 100){
      lcd.clear();
      timp = millis();
      moveChar(playerHeight);
      if (obsState == 1){
        createObstacle(timp, nextObsDistance, obsHeight1, obsPos1);
        obsState = 2;
      }
      else if (obsState == 2){
        createObstacle(timp, nextObsDistance, obsHeight2, obsPos2);
        obsState = 1;
      }
      if (timp%500 == 0){
        for (uint8_t obsPos1; obsPos1>=-3; obsPos1--){
          for (uint8_t i=0; i<=3; i++){
            lcd.setCursor(obsPos1-i,obsHeight1);
            lcd.print((byte)1);
          }
        }
        for (uint8_t obsPos2; obsPos2>=-3; obsPos2--){
          for (uint8_t i=0; i<=3; i++){
            lcd.setCursor(obsPos2-i,obsHeight2);
            lcd.print((byte)1);
          }
        }
        obsPos1 -= 1;
        obsPos2 -= 1;
        nextObsDistance -= 1;
      }//miscare obstacol
      collision(obsPos1, obsHeight1, playerHeight);
      collision(obsPos2, obsHeight2, playerHeight);
    }
  }//pause false
  else{
    lcd.setCursor(3,0);
    lcd.print("Game  Over");
  }
}
