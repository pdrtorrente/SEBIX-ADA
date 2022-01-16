#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int xPlayer = 2;
int yPlayer = 1;

int xObstacle = 15;
int yObstacle = 1;

int btnJump = 13;
int btnStart = 9;

int speed = 300;

int onAir = 0; // 0 - chao // 1- primeiro bloco no ar // 2- segundo bloco no ar

int player;

int flagStart = 0;

int score = 0;

int highScore;

int isAlreadyOnStartScreen = 0;

byte dinosaur[8]={
  B00111,
  B00111,
  B00110,
  B01111,
  B01110,
  B11110,
  B01010,
  B01010
 };


byte human[8]={
  B01110,
  B01110,
  B00100,
  B11111,
  B10101,
  B00100,
  B01010,
  B01010
};


byte obstacle[8]={
  B00000,
  B00000,
  B01110,
  B00100,
  B01110,
  B00100,
  B01110,
  B00100
};



void drawPlayer(int posX, int posY, int player){
  
  lcd.setCursor(posX,posY);
    
  lcd.write(byte(player));
  
}



void drawObstacle(int posX,int posY){
  
  lcd.setCursor(posX, posY);
    
  lcd.write(byte(3));
  
}


void drawScore(int score){
  
  lcd.setCursor(13, 0);
  
  lcd.print(score);
}



void initializeAllChars(){
  
  lcd.createChar(0, dinosaur);
  
  lcd.createChar(1, human);
  
  lcd.createChar(3, obstacle);
  
}



void refresh(int speed){

  delay(speed);
  
  lcd.clear();
  
}



bool isCrash(int xPlayer,int yPlayer,int xObstacle,int yObstacle){
    
  if( (xPlayer == xObstacle) && (yPlayer == yObstacle) ){
    return true;
  }else{
  	return false;
  }
  
}



bool addScore(int xPlayer, int yPlayer, int xObstacle, int yObstacle){
  
  if( (xPlayer == xObstacle) && (yPlayer != yObstacle) ){
    return true;
  }else{
   	return false;
  }
    
}



void startScreen(){
  lcd.print("PRESSIONE START!");
}



void setup() {
  
  Serial.begin(9600);
  
  pinMode(btnJump, INPUT); 
  
  pinMode(btnStart, INPUT);
  
  randomSeed(analogRead(0));
  
  player = random(0,2);
  
  initializeAllChars();
  
  lcd.begin(16,2);
 
  startScreen();
  
}


void loop() {
  
  
  if(digitalRead(btnStart) == LOW){
      flagStart = 1;
      isAlreadyOnStartScreen = 0;
  }else if(!isAlreadyOnStartScreen){
    startScreen();
    isAlreadyOnStartScreen = 1;
  }
  
  
  if(flagStart == 1){
    
    lcd.clear();
    
    
    if( isCrash(xPlayer,yPlayer,xObstacle,yObstacle) ){ 
    	flagStart = 0;
      	lcd.clear();
        isAlreadyOnStartScreen = 0;
        score = 0;
        xObstacle = 15;
    }
    
    
    if( addScore(xPlayer,yPlayer,xObstacle,yObstacle) ){  
     	score++; 
    }
    
    drawScore(score);
    
    //Movimento da arvore.
    drawObstacle(xObstacle,yObstacle);
    xObstacle--;

    //Se a arvore ja chegou no comeco do display, volta pro final.
    if(xObstacle<0){
    	xObstacle = 15;
    }

    drawPlayer(xPlayer,yPlayer,player);

    if(onAir == 2){
    	yPlayer++;
        onAir = 0;
    }else if( (digitalRead(btnJump) == LOW) && (onAir == 0) ){
        yPlayer--;
        onAir++;
      	tone(10,500,40);
    }else if(onAir == 1){
        onAir++;
    }

    refresh(speed);
    
  }
  
}
