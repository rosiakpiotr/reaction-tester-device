#define CONNECTION_CHECK 1000

#include <LiquidCrystal.h>
#include <Chrono.h>

LiquidCrystal lcd(2, 3, 12, 11, 10, 8);

enum actions { beginGame, checkVoltage, showAuthor, resetLCD, settingTestCount };

struct InputController {

  // Variable that holds number, telling us how many buttons is pressed.
  int currentlyPressedSum = 0;
  int recentlyPressedSum = 0;
  bool canTriggerAction = false;
  Chrono chrono;

  String stringAction;
  actions possibleAction = -1;

  InputController() {
    // Initialize chrono.
    chrono = Chrono();
  }

  void tick() {
    currentlyPressedSum = (PIND & 0xF0);

    if (currentlyPressedSum != recentlyPressedSum || (currentlyPressedSum == 0)) {
      // Pressed or released something, restart timer.
      chrono.restart();
      canTriggerAction = false;
      recentlyPressedSum = currentlyPressedSum;
    }
    else if (chrono.hasPassed(1500)) {
      // If current buttons configuration has been pressed for 2 seconds.
      canTriggerAction = true;
    }

    switch (currentlyPressedSum) {
      case 240: possibleAction = actions::settingTestCount; break;
      case 96: possibleAction = actions::checkVoltage; break;
      case 128: possibleAction = actions::showAuthor; break;
      case 16: possibleAction = actions::resetLCD; break;
      default: possibleAction = -1; break;
    }

  }
};

struct {
  int low;
  int high;
  int level;
  String caption;
} difficulty;

InputController icontr;

void setup() {
  Serial.begin(9600);

  for (int pin = 0x4; pin < 0x8; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }

  randomSeed(analogRead(A0));

  DDRB |= 0x2;
  DDRC = 0xFE;

  lcd.begin(16, 2);

  difficulty.level = 2;

  mainScreen();
}

long long startTime;
int choosenIndex;
bool isGameOn = false;
int roundCount = 0;
int timingsSum = 0;
int correctCount = 0;
int testCount = 5;

bool isSettingTestsCount = false;
bool isSettingDifficulty = false;
bool canSet = false;
void loop() {

  icontr.tick();

  if (icontr.canTriggerAction && (~icontr.currentlyPressedSum) == 128) {
    canSet = true;
  }
      
  if (icontr.canTriggerAction && icontr.possibleAction == actions::settingTestCount) {
    isSettingTestsCount = true;
    canSet = false;
  }

  if (icontr.currentlyPressedSum == 128 && isSettingTestsCount) {
    isSettingDifficulty = true;
    isSettingTestsCount = false;
  }

  if (isSettingTestsCount) {

    if (icontr.currentlyPressedSum == 64) { if (testCount < 9) testCount++; }
    if (icontr.currentlyPressedSum == 32) { if (testCount > 1) testCount--; }
  
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Liczba testow");
    lcd.setCursor(8, 1);
    lcd.print(testCount);
    delay(200);
  }

  if (isSettingDifficulty) {

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Trudnosc");
    
    if (icontr.currentlyPressedSum == 64) { if (difficulty.level < 3) difficulty.level++; }
    if (icontr.currentlyPressedSum == 32) { if (difficulty.level > 1) difficulty.level--; }


    switch (difficulty.level) {
      case 1:
        difficulty.low = 1000;
        difficulty.high = 2500;
        difficulty.caption = "Poczatkujacy";
        lcd.setCursor(2, 1);
      break;

      case 2:
        difficulty.low = 500;
        difficulty.high = 1500;
        difficulty.caption = "Sredni";
        lcd.setCursor(5, 1);
      break;

      case 3:
        difficulty.low = 250;
        difficulty.high = 500;
        difficulty.caption = "Mistrz";
        lcd.setCursor(5, 1);
      break;
    }
    
    lcd.print(difficulty.caption);
    delay(200);
  }

  if (icontr.currentlyPressedSum == 128 && isSettingDifficulty && icontr.canTriggerAction) {
    PORTC |= 0x20;
    PORTB |= 0x2;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Zaczynam gre");
    delay(1500);
    PORTC &= ~0x20;
    PORTB &= ~0x2;
    delay(1000);
    lcd.clear();
    testCount--;
    startGame();
    isSettingDifficulty = false;
  }

  if (icontr.canTriggerAction && icontr.possibleAction == actions::checkVoltage) {
    lcd.clear();
    lcd.setCursor(0, 0);
    int voltageRead = (analogRead(A7) / 122);
    float offset = 0.5 + 0.8;
    if (voltageRead < 8) {
      offset += 1;
    }
    lcd.print("Napiecie baterii");
    lcd.setCursor(6, 2);
    lcd.print(String(voltageRead + offset) + "V");
    delay(500);
  }

  if (icontr.canTriggerAction && icontr.possibleAction == actions::showAuthor && !isSettingDifficulty) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Autor:");
    lcd.setCursor(4, 1);
    lcd.print("Piotr Rosiak");
    delay(500);
  }

  if (icontr.canTriggerAction && icontr.possibleAction == actions::resetLCD) {
    roundCount = testCount++;
    mainScreen();
  }

  // Communication with PC
  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');
    int question = data.toInt();

    if (question == 1000) {
      //Serial.println(CONNECTION_CHECK);
    }

    if (question == 1100) {
      digitalWrite(13, HIGH);
      Serial.println("Light is on buddy! Don't worry tho, I will turn it off soon!");
      delay(2000);
      digitalWrite(13, LOW);
    }

    if (question == 2000) {
      testCount = 4;
      startGame();
    }

    // Stop game
    if (question == 2999) {
      PORTC &= B0000000;
      isGameOn = false;
      PORTB &= ~0x3;
    }

    // Codes with '3' in the front are animations, next numbers are animation types.
    // If code doesnt fit to any animation, it just won't execute.
    startAnimation(question);
  }

  if (isGameOn) {
    for (short i = 0x4; i < 0x8; i++) {
      if (PIND & (0x1 << i)) {
        PORTC &= ~0xDF;
        if (i == 7 - choosenIndex) {
          int reactiontime = (long)((micros() - startTime) / (float)pow(10, 3));
          timingsSum += reactiontime;
          correctCount++;
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("Czas: " + String(reactiontime) + "ms");
          Serial.println("2010[" + String(reactiontime));
          PORTC |= 0x1 << 5;
          delay(250);
          PORTC &= ~(0x1 << 5);
        } else {
          PORTB |= 0x2;
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("Nie trafione");
          Serial.println("2009");
          delay(250);
          PORTB &= ~(0x1 << 1);
        }

        // Off all leds.
        PORTC &= ~0xDF;
        isGameOn = 0x0;

        if (++roundCount > testCount) {
          roundCount = 0;
          PORTC &= B0000000;
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("Koniec!");
          if (correctCount > 0) {
            lcd.setCursor(1, 1);
            lcd.print("Srednia: " + String((int)(timingsSum / (float)correctCount)) + "ms");
          }
          else {
            lcd.print("0 trafionych ;(");
          }

          timingsSum = 0;
          correctCount = 0;

          delay(2500);
          startAnimation(random(3000, 3005));
        } else {
          // Delay random time before next round.
          delay(random(difficulty.low, difficulty.high));
          startGame();
        }
      }
    }
  }
}

void mainScreen() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Oczekuje");
  lcd.setCursor(2, 1);
  lcd.print("na polecenia");
}


void startGame() {
  // Start game here.
  choosenIndex = random(4);
  PORTC = 0x1 << choosenIndex + 1;
  Serial.println("2001");
  isGameOn = true;
  startTime = micros();
}

void startAnimation(int number) {
  switch (number) {
    case 3010:
      PORTC &= B0000000;
      break;

    case 3000:
      defaultAnimation();
      break;

    case 3001:
      fromCenterAnimation();
      break;

    case 3002:
      toCenterAnimation();
      break;

    case 3003:
      leftToRightAnimation();
      break;

    case 3004:
      rightToLeftAnimation();
      break;
  }
}

// Just blinks all LEDs simultaneously.
void defaultAnimation() {
  PORTC |= B0011110;
  delay(450);
  PORTC = B0000000;
}

void fromCenterAnimation() {
  PORTC |= B0001100;
  delay(200);
  PORTC = B0011110;
  delay(200);
  PORTC = B0010010;
  delay(200);
  PORTC = B0000000;
}

void toCenterAnimation() {
  PORTC |= B0010010;
  delay(200);
  PORTC = B0011110;
  delay(200);
  PORTC = B0001100;
  delay(200);
  PORTC = B0000000;
}

void rightToLeftAnimation() {
  PORTC |= 0x2;
  delay(100);
  PORTC = 0x6;
  delay(100);
  PORTC = 0xE;
  delay(100);
  PORTC = 0x1E;
  delay(100);

  PORTC = 0x1C;
  delay(100);
  PORTC = 0x18;
  delay(100);
  PORTC = 0x10;
  delay(100);
  PORTC = 0x0;
}

void leftToRightAnimation() {
  PORTC = 0x10;
  delay(100);
  PORTC = 0x18;
  delay(100);
  PORTC = 0x1C;
  delay(100);
  PORTC = 0x1E;
  delay(100);

  PORTC = 0xE;
  delay(100);
  PORTC = 0x6;
  delay(100);
  PORTC = 0x2;
  delay(100);
  PORTC = 0x0;
}
