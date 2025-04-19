#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>  // Optional if not using USB serial
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button pin definitions
#define sw1 2 // BJP
#define sw2 3 // INC
#define sw3 4 // AAP
#define sw4 5 // OTH
#define sw5 6 // RESULT

int vote1 = 0, vote2 = 0, vote3 = 0, vote4 = 0;
bool allowVoting = false;  // 🔑 Controls voting permission

void setup() {
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);
  pinMode(sw5, INPUT_PULLUP);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  Serial.begin(9600);  // 🛜 Listen for PC approval

  // Welcome screen
  lcd.setCursor(0, 0);
  lcd.print("     ECN244 ");
  lcd.setCursor(0, 1);
  lcd.print("    PROJECT");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   ELECTRONIC ");
  lcd.setCursor(0, 1);
  lcd.print(" VOTING MACHINE");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Waiting Approval");
}

void loop() {
  // ✅ Step 1: Listen for approval from PC
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "APPROVED") {
      allowVoting = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BJP INC BSP NOTA");
    }
  }

  // ✅ Step 2: Voting only allowed after approval
  if (allowVoting) {
    lcd.setCursor(0, 1);
    lcd.print(vote1);
    lcd.setCursor(4, 1);
    lcd.print(vote2);
    lcd.setCursor(8, 1);
    lcd.print(vote3);
    lcd.setCursor(12, 1);
    lcd.print(vote4);

    if (digitalRead(sw1) == LOW) {
      vote1++; allowVoting = false;  // One vote per approval
      lcd.clear(); lcd.print("Vote Cast: BJP"); delay(800); lcd.clear();
      delay(300);
    }

    if (digitalRead(sw2) == LOW) {
      vote2++; allowVoting = false;
      lcd.clear(); lcd.print("Vote Cast: INC"); delay(800); lcd.clear();
      delay(300);
    }

    if (digitalRead(sw3) == LOW) {
      vote3++; allowVoting = false;
      lcd.clear(); lcd.print("Vote Cast: BSP"); delay(800); lcd.clear();
      delay(300);
    }

    if (digitalRead(sw4) == LOW) {
      vote4++; allowVoting = false;
      lcd.clear(); lcd.print("Vote Cast: NOTA"); delay(800); lcd.clear();
      delay(300);
    }
    lcd.print("Next Voter...");
  }

  // ✅ Step 3: Show result when sw5 pressed
  if (digitalRead(sw5) == LOW) {
    int totalVotes = vote1 + vote2 + vote3 + vote4;
    lcd.clear();

    if (totalVotes > 0) {
      int bjpPerc = (vote1 * 100) / totalVotes;
      int incPerc = (vote2 * 100) / totalVotes;
      int aapPerc = (vote3 * 100) / totalVotes;

      lcd.setCursor(0, 0);
      lcd.print("B:");
      lcd.print(bjpPerc);
      lcd.print("% I:");
      lcd.print(incPerc);
      lcd.print("%");

      lcd.setCursor(0, 1);
      lcd.print("Bs:");
      lcd.print(aapPerc);
      lcd.print("%");

      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Winner:");

      if (vote1 > vote2 && vote1 > vote3) {
        lcd.print(" BJP");
      } else if (vote2 > vote1 && vote2 > vote3) {
        lcd.print(" INC");
      } else if (vote3 > vote1 && vote3 > vote2) {
        lcd.print(" BSP");
      } else {
        lcd.setCursor(0, 0);
        lcd.print(" Tie Vote");
        lcd.setCursor(0, 1);
        lcd.print(" Vote Again");
      }
    } else {
      lcd.print("No Voting Yet");
    }

    delay(5000);
    lcd.clear();
    vote1 = vote2 = vote3 = vote4 = 0;
    Serial.println("RESET");
    lcd.setCursor(0, 0);
    lcd.print("Start New Voting..");
  }
}
