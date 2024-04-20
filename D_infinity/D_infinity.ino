#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Variables determining pins connected to the different buttons
const int selButton = 5;
const int leftButton = 4;
const int rightButton = 3;
const int acceptButton = 2;

// Variables for button states
int currentButtonStateSel = 0; 
int lastButtonStateSel = 0; 
int currentButtonStateL = 0; 
int lastButtonStateL = 0; 
int currentButtonStateR = 0; 
int lastButtonStateR = 0; 
int currentButtonStateA = 0; 
int lastButtonStateA = 0; 

// Variables for menuing
int menuValue = 0;
int subMenuValue = 0;
int choiceValue = 0;
bool inResultMenu = false;
bool inSubMenu = false;

// Values for dice rolls and results
int diceValue = 20;
long diceResult = 0;

// Variables for creating the proper display
String result = "0000"; 
String leadingZeroes = "0";
String displayValue = " d20";

void setup()
{
  // Initialize lcd, serial monitor and pins
  lcd.init();                      
  lcd.backlight();
  Serial.begin(9600);
  pinMode(selButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(acceptButton, INPUT);

  // Generate random seed based on error readings from analog pin 0
  randomSeed(analogRead(0));
}

void loop()
{
  // Save previous button values to register one button press on hold of the button
  lastButtonStateSel = currentButtonStateSel;
  lastButtonStateL = currentButtonStateL;
  lastButtonStateR = currentButtonStateR;
  lastButtonStateA = currentButtonStateA;

   // Register current input from the 4 buttons
  currentButtonStateSel = digitalRead(selButton);
  currentButtonStateL = digitalRead(leftButton);
  currentButtonStateR = digitalRead(rightButton);
  currentButtonStateA = digitalRead(acceptButton);
  
  // Switch through main menus using the select button
  if (currentButtonStateSel == HIGH && lastButtonStateSel == LOW) {        
        menuValue++;
        if(menuValue >= 3){
          menuValue = 0;
        }
        choiceValue = 0;
        subMenuValue = 0;
        inResultMenu = false;
        result = "0000";
        diceResult = 0;
  } 

  // If we're not displaying a roll result perform the functions of each different menu to combine dice and modifiers
  if(inResultMenu){
      displayValue = result;
    if(currentButtonStateA == HIGH && lastButtonStateA == LOW){
      inResultMenu = false;
      result = "0000";
    }
  }else{
    switch (menuValue) {
    case 0:
      singleDie();
      break;
    case 1:
      combinationDice();
      break;  
    case 2:
      customDice();
      break;
    }
  }

  // Render graphical interface 
  lcd.clear();
  lcd.print(displayValue);

  // All of these extra lcd prints are just for debugging and completely unnecessary
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

  // Delay to reduce refresh rate of the screen in a janky way -> should use custom delay that doesn't interfere with any processes. Counter system?
  delay(50);
}

void singleDie(){
  // Use left and right buttons to determine a dice choice.
  if (currentButtonStateR == HIGH && lastButtonStateR == LOW) {        
    choiceValue++;
    choiceValue = choiceValue >= 8 ? 0 : choiceValue;
  } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW){
    choiceValue--;
    choiceValue = choiceValue < 0 ? 7 : choiceValue;
  }

  // Choose die and display properties based on the choice value
  chooseDie();

  // Go to result menu and determine what to display there.
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
  }
}

void combinationDice(){
  switch (subMenuValue){
    case 0:
      if(currentButtonStateA == HIGH && lastButtonStateA == LOW){    
        subMenuValue++;
      }else{
        displayValue = "COMB";
      }
      break;
    case 1:
      // Use left and right buttons to determine a dice choice.
      if (currentButtonStateR == HIGH && lastButtonStateR == LOW) {        
        choiceValue++;
        choiceValue = choiceValue >= 10 ? 0 : choiceValue;
      } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW){
        choiceValue--;
        choiceValue = choiceValue < 0 ? 9 : choiceValue;
      }

      // Choose die and display properties based on the choice value
      if(choiceValue==8){
        displayValue = " MOD";
        diceValue = 1; 
      }else if(choiceValue==9){
        displayValue = "ROLL";
      }else{
        chooseDie();
      }

      // Go to result menu if ROLL is selected, otherwise choose number of dice to add to result.
      if(currentButtonStateA == HIGH && lastButtonStateA == LOW && choiceValue == 9){    
        inResultMenu = true;
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
      }else if(currentButtonStateA == HIGH && lastButtonStateA == LOW){
        choiceValue = 1;
        subMenuValue++;
      }
      break;
    case 2:
      // Choose the number of dice to add to the total
      if (currentButtonStateR == HIGH && lastButtonStateR == LOW) {        
        choiceValue++;
        choiceValue = choiceValue >= 101 ? 0 : choiceValue;
      } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW){
        choiceValue--;
        choiceValue = choiceValue < 0 ? 100 : choiceValue;
      }

      // Add leading zeros to the display
      if(choiceValue<10){
        leadingZeroes = "   ";
        displayValue = leadingZeroes + choiceValue;
      }else if(choiceValue<100){
        leadingZeroes = "  ";
        displayValue = leadingZeroes + choiceValue;
      }else if(choiceValue<=1000){
        leadingZeroes = " ";
        displayValue = leadingZeroes + choiceValue;
      }else{
        displayValue = String(choiceValue);
      }

      // If the accept button is pressed add the chosen die choiceValue number of times and add it to the diceresult
      if(currentButtonStateA == HIGH && lastButtonStateA == LOW){    
        for (int i = 1; i <= choiceValue; i++) {
          diceResult = diceResult + random(1, diceValue+1); 
          Serial.println(diceResult);
        }
        subMenuValue = 1;
        choiceValue = 0;
      }
      break;
  }
}

void customDice(){
  switch (subMenuValue){
    case 0:
      // Enter custom menu
      if(currentButtonStateA == HIGH && lastButtonStateA == LOW){    
        choiceValue = 2;
        subMenuValue++;
      }else{
        displayValue = "CUST";
      }
      break;
    case 1:
      // Use left and right buttons to determine a dice choice.
      if (currentButtonStateR == HIGH && lastButtonStateR == LOW) {        
        choiceValue++;
        choiceValue = choiceValue >= 101 ? 2 : choiceValue;
      } else if(currentButtonStateL == HIGH && lastButtonStateL == LOW){
        choiceValue--;
        choiceValue = choiceValue < 2 ? 100 : choiceValue;
      }

      diceValue = choiceValue;

      // Choose die and display properties based on the choice value
      if(choiceValue<10){
        leadingZeroes = "  d";
        displayValue = leadingZeroes + choiceValue;
      }else if(choiceValue<100){
        leadingZeroes = " d";
        displayValue = leadingZeroes + choiceValue;
      }else if(choiceValue<=1000){
        leadingZeroes = "d";
        displayValue = leadingZeroes + choiceValue;
      }else{
        displayValue = String(choiceValue);
      }

      // Go to result menu and determine what to display there.
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
      }
  }
}

void chooseDie(){
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
    }
}