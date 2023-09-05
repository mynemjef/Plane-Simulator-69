//ne sum pisal komentari
//kusmet

#include <LiquidCrystal.h>
const int RS = 8;
const int EN = 9;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
const int pin_BL = 10;

int playerPos = 2, playerHeight = 0, rocketID = 0, delayT = 200, buttonValue, score, command, rocketPos, rocketHeight, enemyBulletsPos, enemyBulletsHeight, playerBulletsPos, playerBulletsHeight;
bool enemyShot = false, playerShot = false, enemyAlive = false, rocketLaunch = false, paused = true;
long enemyHeight, enemyPos, randEnemy, shootRNG, rocketRNG;

LiquidCrystal lcd( RS,  EN,  d4,  d5,  d6,  d7);

byte playerPlane1[8] = {
  B00000,
  B00010,
  B10011,
  B11011,
  B11111,
  B11011,
  B10011,
  B00010
};

byte playerPlane2[8] = {
  B00000,
  B00000,
  B00000,
  B11000,
  B11111,
  B11000,
  B00000,
  B00000
};

byte enemyPlane1[8] = {
 B00000,
 B00000,
 B00000,
 B00011,
 B11111,
 B00011,
 B00000,
 B00000
};

byte enemyPlane2[8] = {
 B00000,
 B00000,
 B10000,
 B10010,
 B11110,
 B10010,
 B10000,
 B00000
};

byte enemyBigHeli1[8] = {
 B11111,
 B00001,
 B11111,
 B00001,
 B00111,
 B01111,
 B11111,
 B00000
};

byte enemyBigHeli2[8] = {
 B11111,
 B10000,
 B11111,
 B10000,
 B11001,
 B11111,
 B10011,
 B00000
};

byte bullets[8] = {
 B00000,
 B00000,
 B00000,
 B00111,
 B00000,
 B11100,
 B00000,
 B00000
};

byte rocket[8] = {
 B00000,
 B00000,
 B10000,
 B11111,
 B10000,
 B00000,
 B00000,
 B00000
};


void spawnPlane(){
  lcd.setCursor(playerPos, playerHeight);
  lcd.write(1);
  playerPos += 1;
  lcd.setCursor(playerPos, playerHeight);
  lcd.write(4);
}

void spawnEnemy(){
  enemyHeight = random(0, 2);
  enemyPos = random(13,15);
  randEnemy = random(2, 4);
  lcd.setCursor(enemyPos, enemyHeight);
  lcd.write(randEnemy);
  randEnemy += 3;
  enemyPos += 1;
  lcd.setCursor(enemyPos, enemyHeight);
  lcd.write(randEnemy);
  randEnemy -= 3;
}

void enemyShoot() {
  lcd.setCursor(enemyPos, enemyHeight);
  lcd.write(" ");
  enemyPos -= 1;
  lcd.setCursor(enemyPos, enemyHeight);
  lcd.write(" ");

  enemyHeight = playerHeight;
  lcd.setCursor(enemyPos, enemyHeight);
  lcd.write(randEnemy);
  randEnemy += 3;
  enemyPos += 1;
  lcd.setCursor(enemyPos, enemyHeight);
  lcd.write(randEnemy);
  enemyBulletsPos = enemyPos - 2;
  enemyBulletsHeight = enemyHeight;
  lcd.setCursor(enemyBulletsPos, enemyBulletsHeight);
  lcd.write(7);
  randEnemy -= 3;
  enemyShot = true;
}

void shootRocket() {
  rocketHeight = playerHeight;
  rocketPos = 0;
  lcd.setCursor(rocketPos, rocketHeight);
  lcd.write(rocketID);
  rocketLaunch = true;
}

void moveRocket() {
  if (rocketPos==playerPos && rocketHeight==playerHeight){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   TI GUBISH!");
    lcd.setCursor(0,1);
    lcd.print("     BALUK!");
    delay(2000);
    lcd.clear();
    delay(400);
    while (true) {
       lcd.setCursor(0, 0);
       lcd.print("   SCORE: ");
       delay(600);
       lcd.setCursor(10, 0);
       lcd.print(score);
    }
  }
  else if (rocketPos==playerPos && enemyHeight==enemyHeight){
      playerShot = false;
      enemyAlive = false;
      lcd.setCursor(playerBulletsPos, playerBulletsHeight);
      lcd.write(" ");
      score+=2;
  }
  lcd.setCursor(rocketPos, rocketHeight);
  lcd.write(" ");
  rocketPos +=1;
  if (rocketPos <= 16) {
    lcd.setCursor(rocketPos, rocketHeight);
    lcd.write(rocketID);
  }
  else{
    rocketLaunch = false;
  }
}

void moveBullets() {
  //abandon hope all you who enter here
  if (enemyShot) {
    if (enemyBulletsPos==playerPos && enemyBulletsHeight==playerHeight){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   TI GUBISH!");
      lcd.setCursor(0,1);
      lcd.print("     BALUK!");
      delay(2500);
      lcd.clear();
      delay(600);
      while (true) {
        lcd.setCursor(0, 0);
        lcd.print("   SCORE: ");
        delay(1900);
        lcd.setCursor(10, 0);
        lcd.print(score);
      }
    }
  }
  lcd.setCursor(enemyBulletsPos, enemyBulletsHeight);
  lcd.write(" ");
  enemyBulletsPos -=1;
  if (enemyBulletsPos >= 0) {
    lcd.setCursor(enemyBulletsPos, enemyBulletsHeight);
      lcd.write(7);
  }
  else{
    enemyShot = false;
  }
  
  if (playerShot) {
    lcd.setCursor(playerBulletsPos, playerBulletsHeight);
    lcd.write(" ");
    playerBulletsPos +=1;
    if (playerBulletsPos <= 16){
      lcd.setCursor(playerBulletsPos, playerBulletsHeight);
      lcd.write(7);
    }
    else{
      playerShot = false;
    }
    if (playerBulletsPos == enemyPos && playerBulletsHeight == enemyHeight){
      playerShot = false;
      enemyAlive = false;
      lcd.setCursor(playerBulletsPos, playerBulletsHeight);
      lcd.write(" ");
      score += 1;
    }
  }
}

void playerAction(int command) {
  int dir;

  dir = command;

  if (dir != 5){
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(" ");
    playerPos -= 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(" ");
  }
  
  if (dir == 1){
    playerPos += 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(1);
    playerPos += 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(4);
    Serial.println(playerPos);
  }
  
  else if(dir == 2){
    playerHeight -= 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(1);
    playerPos += 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(4);
  }
  
  else if(dir == 3){
    playerHeight += 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(1);
    playerPos += 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(4);
  }
  
  else if(dir == 4){
    playerPos -= 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(1);
    playerPos += 1;
    lcd.setCursor(playerPos, playerHeight);
    lcd.write(4);
  }
  else if(dir == 5){
    playerBulletsPos = playerPos + 1;
    playerBulletsHeight = playerHeight;
    lcd.setCursor(playerBulletsPos, playerBulletsHeight);
    lcd.write(7);
    playerShot = true;
  }
}

void Menu(){
  lcd.setCursor(0, 0);
  lcd.write(" PLANE COMBAT 69");
  lcd.setCursor(0, 1);
  lcd.write("  PRESS SELECT");
  while (paused){
    buttonValue = analogRead(0);
    if (buttonValue < 800 && buttonValue > 600) {
      paused = false;
      lcd.clear();
    }
  }
}

void setup() {
 Serial.begin(9600);
 randomSeed(analogRead(1));
 lcd.begin(16, 2);
 lcd.createChar(1, playerPlane1);
 lcd.createChar(4, playerPlane2);
 lcd.createChar(2, enemyPlane1);
 lcd.createChar(5, enemyPlane2);
 lcd.createChar(3, enemyBigHeli1);
 lcd.createChar(6, enemyBigHeli2);
 
 lcd.createChar(7, bullets);
 lcd.createChar(0, rocket);
}

void loop() {
  if(paused){
    Menu();
    spawnPlane();
    delay(300);
  }
  if (!enemyAlive){
    spawnEnemy();
    enemyAlive = true;
  }
  
  buttonValue = analogRead(0);
  if (buttonValue < 60) {
    //right
    if (playerPos < 11){
      playerAction(1);
    }
  }
  else if (buttonValue < 200) {
    //up
    if (playerHeight > 0){
      playerAction(2);
    }
  }
  else if (buttonValue < 400){
    //down
    if (playerHeight < 1){
      playerAction(3);
    }
  }
  else if (buttonValue < 600){
    //left
    if (playerPos > 1){
      playerAction(4);
    }
  }
  else if (buttonValue < 800){
    //select
    if (!playerShot){
      playerAction(5);
    }
  }
  
  moveBullets();
  if (rocketLaunch){
    moveRocket();
  }
  if (!enemyShot && enemyAlive){
    shootRNG = random(201);
    if (shootRNG < 15){
      enemyShoot();
    }
  }
  if (!rocketLaunch){
    rocketRNG = random(201);
    if (rocketRNG < 7){
      shootRocket();
    }
  }
  delay(100);
}
