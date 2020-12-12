#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

byte playerChar[] = {B01100, B01100, B00000, B01110, B11100, B01100, B11010, B10011};
byte obs[] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte bullet[] = {B00000, B00000, B11100, B11111, B11111, B11100, B00000, B00000};

bool pause = false;
uint8_t playerHeight = 0;
uint8_t obsHeight1 = 0, obsHeight2 = 1;
uint8_t obsPos1 = 16, obsPos2 = 16;
bool obsMotion1 = true, obsMotion2 = false, bulletMotion = false;
uint8_t bulletPos = -1, bulletHeight = 1;
uint8_t frameCounter = 1;
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

void moveChar(uint8_t pHeight){
  if (tastatura(analogRead(A1)) == 1)
    pHeight = 0;
  else if (tastatura(analogRead(A1)) == 2)
    pHeight = 1;
  lcd.setCursor(0,pHeight);
  lcd.write((byte)0);
}

void createBullet(bool bMotion, uint8_t bPos, uint8_t bHeight, uint8_t pHeight){
  if (tastatura(analogRead(A1)) == 3){
    bMotion = true;
    bPos = 1;
    bHeight = pHeight;
  }
}

void createObs(uint8_t oPos, bool oMotion){
  oPos = random(16, 18);
  oMotion = true;
}

void destroyObs(uint8_t oPos, bool oMotion){
  oPos = 17;
  oMotion = false;
}

void collision(uint8_t oPos, uint8_t oHeight, uint8_t pHeight){
  if (oPos<=0 && oPos>=-3 && oHeight == pHeight)
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
  lcd.createChar(2, bullet);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
}

void loop(){
  if (pause == false){
    if (millis() - timp >= 100){
      Serial.println("frame");
      lcd.clear();
      timp = millis();
      moveChar(playerHeight);
      createBullet(bulletMotion, bulletPos, bulletHeight, playerHeight);
      if (obsPos1 <= 3)
        createObs(obsPos2, obsMotion2);
      if (obsPos2 <= 3)
        createObs(obsPos1, obsMotion1);
      if (obsMotion1 == true){
        for (uint8_t i=0; i<=3; i++){
          lcd.setCursor(obsPos1+i,obsHeight1);
          lcd.write((byte)1);
        }
      }
      if (obsMotion2 == true){
        for (uint8_t i=0; i<=3; i++){
          lcd.setCursor(obsPos2+i,obsHeight2);
          lcd.write((byte)1);
        }
      }
      if (bulletMotion == true)
        lcd.setCursor(bulletPos, bulletHeight);
        lcd.write((byte)2);
      if (frameCounter%5 == 0){
        if (obsMotion1 == true)
          obsPos1 = obsPos1 - 1;
        if (obsMotion2 == true){
          obsPos2 = obsPos2 - 1;
        if (bulletMotion == true){
          bulletPos = bulletPos + 1;
        }
        if (bulletHeight == 1 || bulletPos >= obsPos1)
          destroyObs(obsPos1, obsMotion1);
          bulletMotion = false;
          bulletPos = -1;
        if (bulletHeight == 2 || bulletPos >= obsPos2)
          destroyObs(obsPos2, obsMotion2);
          bulletMotion = false;
          bulletPos = -1;
        nextObsDistance = nextObsDistance - 1;
        Serial.println("obstacole miscate");
      }//miscare obstacol
      collision(obsPos1, obsHeight1, playerHeight);
      collision(obsPos2, obsHeight2, playerHeight);
      frameCounter = frameCounter + 1;
      }
    }
  }//pause false
  else{
    lcd.setCursor(3,0);
    lcd.print("Game  Over");
  }
}
