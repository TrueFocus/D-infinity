#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int selButton = 5;
const int leftButton = 4;
const int rightButton = 3;
const int acceptButton = 2;

int currentButtonStateSel = 0; 
int lastButtonStateSel = 0; 
int currentButtonStateL = 0; 
int lastButtonStateL = 0; 
int currentButtonStateR = 0; 
int lastButtonStateR = 0; 
int currentButtonStateA = 0; 
int lastButtonStateA = 0; 

int menuValue = 0;
int subMenuValue = 0;
int choiceValue = 0;
int diceValue = 20;
bool inResultMenu = false;
long diceResult = 0;
String result = "0000"; 
String leadingZeroes = "0";
bool inSubMenu = false;
String displayValue = " d20";

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  pinMode(selButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(acceptButton, INPUT);
  randomSeed(analogRead(0));
}

void loop()
{
  lastButtonStateSel = currentButtonStateSel;
  lastButtonStateL = currentButtonStateL;
  lastButtonStateR = currentButtonStateR;
  lastButtonStateA = currentButtonStateA;
  currentButtonStateSel = digitalRead(selButton);
  currentButtonStateL = digitalRead(leftButton);
  currentButtonStateR = digitalRead(rightButton);
  currentButtonStateA = digitalRead(acceptButton);
  
  // switch through menus
  if (currentButtonStateSel == HIGH && lastButtonStateSel == LOW) {        
        menuValue++;
        if(menuValue >= 3){
          menuValue = 0;
        }
        choiceValue = 0;
        subMenuValue = 0;
        inResultMenu = false;
  } 

  // debug that shows current menu


switch (menuValue) {
case 0:
    if(inResultMenu == false){
      if (currentButtonStateR == HIGH && lastButtonStateR == LOW) {        
        choiceValue++;
        if(choiceValue >= 8){
          choiceValue = 0;
        }
      } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW){
        choiceValue--;
        if(choiceValue <= 0){
          choiceValue = 7;
        }
      }

      switch(choiceValue){
       case 0:
          displayValue = " d20"; 
          diceValue = 20;
          break;
       case 1:
          displayValue = "  d2"; 
          diceValue = 2; 
          break;
       case 2:
          displayValue = "  d4"; 
          diceValue = 4; 
          break;
       case 3:
          displayValue = "  d6"; 
          diceValue = 6; 
          break;
       case 4:
          displayValue = "  d8"; 
          diceValue = 8; 
          break;
       case 5:
          displayValue = " d10"; 
          diceValue = 10; 
          break;
       case 6:
          displayValue = " d12"; 
          diceValue = 12; 
          break;
       case 7:
          displayValue = "d100"; 
          diceValue = 100; 
          break;
      }
    } else if (inResultMenu){
       displayValue = result;
    }
    
      if (currentButtonStateA == HIGH && lastButtonStateA == LOW && inResultMenu == false) {        
          
          inResultMenu = true;
          diceResult = random(1, diceValue+1);
  
          if(diceResult<10){
            leadingZeroes = "   ";
            result = leadingZeroes + diceResult;
          }else if(diceResult<100){
            leadingZeroes = "  ";
            result = leadingZeroes + diceResult;
          }else if(diceResult<=1000){
            leadingZeroes = " ";
            result = leadingZeroes + diceResult;
          }else{
            result = String(diceResult);
          }
          
          Serial.println(diceResult);
          Serial.println(result);
          
      } else if(currentButtonStateA == HIGH && lastButtonStateA == LOW && inResultMenu == true){
        
          inResultMenu = false;
          result = "0000";
          
      }
      
      break;
      
case 1:
      if (currentButtonStateA == HIGH && lastButtonStateA == LOW) {    
          choiceValue = 0;
          if(subMenuValue == 1 && choiceValue == 9){
            inResultMenu = true;
            subMenuValue = 0;
          } else{
            if(subMenuValue<2){
               subMenuValue++;
            }else{
              subMenuValue = 0;
            }
            inResultMenu = false;
          }
      }

      if(inResultMenu){
        displayValue = result;
        break;
      }
      
      switch(subMenuValue){
        case 0:
        
          displayValue = "COMB";
        
        break;
        case 1:
        
        if(!inResultMenu){
          if (currentButtonStateR == HIGH && lastButtonStateR == LOW) 
          {        
              choiceValue++;
              if(choiceValue >= 10)
              {
                choiceValue = 0;
              }
          } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW)
          {
            choiceValue--;
            if(choiceValue < 0)
            {
              choiceValue = 9;
            }
  
           switch(choiceValue)
           {
               case 0:
                  displayValue = " d20"; 
                  diceValue = 20;
                  break;
               case 1:
                  displayValue = "  d2"; 
                  diceValue = 2; 
                  break;
               case 2:
                  displayValue = "  d4"; 
                  diceValue = 4; 
                  break;
               case 3:
                  displayValue = "  d6"; 
                  diceValue = 6; 
                  break;
               case 4:
                  displayValue = "  d8"; 
                  diceValue = 8; 
                  break;
               case 5:
                  displayValue = " d10"; 
                  diceValue = 10; 
                  break;
               case 6:
                  displayValue = " d12"; 
                  diceValue = 12; 
                  break;
               case 7:
                  displayValue = "d100"; 
                  diceValue = 100; 
                  break;
               case 8:
                 displayValue = " MOD";
                 diceValue = 1; 
                 break;
               case 9:
                 displayValue = "ROLL";
                 break; 
            }
          } 
        }   
        break;
        case 2:
//          if (currentButtonStateR == HIGH && lastButtonStateR == LOW) 
//          {        
//              choiceValue++;
//              if(choiceValue >= 10)
//              {
//                choiceValue = 0;
//              }
//          } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW)
//          {
//            choiceValue--;
//            if(choiceValue < 0)
//            {
//              choiceValue = 7;
//            }
//          }
        break;
      }
      break;
case 2:
      displayValue = "CUST";
      break;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(displayValue);
  lcd.setCursor(0,1);
  lcd.print("M");
  lcd.setCursor(1,1);
  lcd.print(menuValue);
  lcd.setCursor(13,1);
  lcd.print("SM");
  lcd.setCursor(15,1);
  lcd.print(subMenuValue);
  lcd.setCursor(4,1);
  lcd.print("C");
  lcd.setCursor(5,1);
  lcd.print(choiceValue);
  lcd.setCursor(14,0);
  lcd.print("R");
  lcd.setCursor(15,0);
  if(inResultMenu){
    lcd.print("y");
  } else{
    lcd.print("n");
  }

    delay(100);

 
  
}
