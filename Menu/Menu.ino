#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>

void menu();
void encoder();
void button();
boolean checkRightRotation();
boolean checkLeftRotation();
boolean checkClick();
void timeSetting();
void volumeSetting();
void printVolume();

volatile char counter = 0; // настройка времени дребезга контактов
volatile unsigned long timer = 0;
volatile char changes = 0;
volatile boolean click_flag = false; 
unsigned long timer_for_menu = 0;
unsigned long mode_timer = 0; // аналог, нужно использовать один и тот же в основном скетче
char wday = -1;
char hour = -1;
char minute = -1;
char second = -1;
char zum = 9;
boolean changer = HIGH;
enum {
  TIME,
  TIME_OF_ALARM,
  VOLUME,
  FREQUENCY,
  NO_SOUND,
  EXIT
} modes;
String mode_string[6] {
  "Time setting",
  "Response time setting",
  "Volume setting",
  "Frequency setting",
  "Configure without sound",
  "Exit"
};
String weekdays[7] {
  "Sun",                    // уточнить соответствие между днями недели и соответсвтующими числами настройки.
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};
byte new_sign[8] {
  B01000,
  B00100,
  B00010,
  B00001,
  B00001,
  B00010,
  B00100,
  B01000
};
byte volume_sign[8] {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void setup() {
  for (byte i = 5; i < 10; i++) {
    pinMode(i, OUTPUT);
  }
  LiquidCrystal_I2C lcd(0x3f, 16, 2);     // инициализировать пины дисплея
  iarduino_RTC time (RTC_DS1302,10,13,12);
  time.begin();
  time.settime(55,59,18,1,12,18,6);
  pinMode(2, INPUT);  // rotate
  pinMode(3, INPUT_PULLUP);  // button
  pinMode(4, INPUT);  // rotate
  attachInterrupt(0, encoder, CHANGE);
  attachInterrupt(1, button, FALLING);
  modes = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
}

void menu() {
  //byte mode_counter = 0;
  lcd.init();
  lcd.display();
  lcd.createChar(0, new_sign);
  lcd.createChar(1, volume_sign);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.setCursor(1, 0)
  lcd.print(mode_string[0]);
  lcd.setCursor(0, 1);
  lcd.print(mode_string[1]);
  while (true) {
    if (checkRightRotation()) {
      modes++;
      constrain(modes, 0, 5);
      lcd.clear(); // чтобы убрать мерцание, первую строку можно переписать с пробелами
      lcd.setCursor(0, 0);
      lcd.print(mode_string[modes-1]);
      if (mode_string[modes].length() <= 15){
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(1, 1);
        lcd.print(mode_string[modes]);
      }
      else {    //если быстро прокручивать, то может не появляться вторая строка, чтобы она появлялась нужно перед while(){} отдельно напечатать вторую строку в начальный момент времени
        while(counter == 0 && click_flag == 0) { // сделать checkClick() с click_flag, проверить, можно ли исользовать уже существующий таймер
          lcd.setCursor(0, 1);
          lcd.write(0);
          lcd.setCursor(1, 1);
          lcd.print(mode_string[modes]);
          timer_for_menu = millis();  
          for(byte i = 0; i < string_array[0].length() - 15; i++) {
            while(millis() - timer_for_menu < 1000) {}
            lcd.scrollDisplayLeft();
            lcd.setCursor(i+1, 1);
            lcd.write(0);
            timer_for_menu = millis();
          }
        }
      }
    }
    else if (checkLeftRotation()) {
      modes--;
      constrain(modes, 0, 5);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(mode_string[modes+1]);
      if (mode_string[modes].length() <= 15) {
        lcd.setCursor(0, 0);
        lcd.write(0);
        lcd.setCursor(1, 0);
        lcd.print(mode_string[modes]);
      }
      else {
        while(counter == 0 && click_flag == 0) { // сделать checkClick() с click_flag, проверить, можно ли использовать уже существующий таймер
          lcd.setCursor(0, 0);
          lcd.write(0);
          lcd.setCursor(1, 0);
          lcd.print(mode_string[modes]);
          timer_for_menu = millis();  
          for(byte i = 0; i < string_array[0].length() - 15; i++) {
            while(millis() - timer_for_menu < 1000) {}
            lcd.scrollDisplayLeft();
            lcd.setCursor(i+1, 0);
            lcd.write(0);
            timer_for_menu = millis();
          }
        }
      }
    }
    if (checkClick()) {
      switch(modes) {
        case 0:
          timeSetting();
          break;
        case 1:
          // Response time setting"
          break;
        case 2:
          volumeSetting();
          break;
        case 3:
          // Frequency setting
          break;
        case 4:
          // Configure without sound
          break;
      }
      if (modes == 5) {
        break;
      }
    }
  }
  modes = 0;
  lcd.noDisplay();
}

void encoder() {
  if (millis() - timer > 10) {
    if (digitalRead(2) == LOW) {
      changes = (digitalRead(4)) ? -1 : 1;
    }
    else if (digitalRead(2) == HIGH) {
      if (digitalRead(4) == HIGH && changes == 1) {
        changes = 0;
        counter++;
      }
      else if (digitalRead(4) == LOW && changes == -1) {
        changes == 0;
        counter--;
      }
      else {
        changes = 0;
      }
    }
  timer = millis();
  }
}

boolean checkRightRotation() {
  if (counter > 0) {
    counter = 0;
    return true;
  }
  else return (false);
}

boolean chackLeftRotation() {
  if (counter < 0) {
    counter = 0;
    return true;
  }
}

void button() {
  if (millis() - timer > 100) {
    click_flag = true;
  }
}

boolean checkClick() {
  if (click_flag == true) {
    click_flag = false;
    return (true);
  }
  else {
    return (false);
  }
}

void timeSetting() {
  wday = -1;
  hour = -1;
  minute = -1;
  second = -1;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Specify the time");
  lcd.setCursor(2, 1);
  lcd.print("DAY HO:MI:SE");
  while(wday == -1 || !checkClick()) {
    if (checkRightRotation()) {
      wday ++;
      constrain(wday, 0, 6);
      lcd.setCursor(2, 1);
      lcd.print(weekdays[wday]);
    }
    else if (checkLeftRotation()) {
      wday --;
      constrain(wday, 0, 6);
      lcd.setCursor(2,1);
      lcd.print(weekdays[wday]);
    }
  }
  while (hour == -1 || !checkClick()) {
    if (checkRightRotation()) {
      hour++;
      constrain(hour, 0, 23);
      lcd.setCursor(6, 1);
      if (hour < 10) lcd.print("0" + (String)hour);
      else lcd.print(hour);
    }
    else if (checkLeftRotation()) {
      hour--;
      constrain(hour, 0, 23);
      lcd.setCursor(6, 1);
      if (hour < 10) lcd.print("0" + (String)hour);
      else lcd.print(hour);
    }
  }
  while (minute == -1 || !checkClick()) {
    if (checkRightRotation()) {
      minute++;
      constrain(minute, 0, 59);
      lcd.setCursor(9, 1);
      if (minute < 10) lcd.print("0" + (String)minute);
      else lcd.print(minute);
    }
    else if (checkLeftRotation()) {
      minute--;
      constrain(minute, 0, 59);
      lcd.setCursor(9, 1);
      if (minute < 10) lcd.print("0" + (String)minute);
      else lcd.print(minute);
    }
  }
  while (second == -1 || !checkClick()) {
    if (checkRightRotation()) {
      second++;
      constrain(second, 0, 59);
      lcd.setCursor(12, 1);
      if (second < 10) lcd.print("0" + (String)second);
      else lcd.print(second);
    }
    else if (checkLeftRotation()) {
      second--;
      constrain(second, 0, 59);
      lcd.setCursor(12, 1);
      if (second < 10) lcd.print("0" + (String)second);
      else lcd.print(second);
    }
  }
  time.settime(second,minute,hour,time.day,time.month,time.year,wday);
}
// для создания функции настройки времени срабатывания
void volumeSetting() {
  printVolume();
  changer = LOW;
  while(!checkClick()) {
    if (millis() - mode_timer >= 500) {
      changer = !changer;
      analogWrite(zum, (changer) ? 150 : 0); // возможно, изменить скважность
    }
    if(checkLeftRotation()) {
      digitalWrite(zum, LOW);
      zum--;
      zum = constrain(zum, 5, 9);
      lcd.clear;
      printVolume();
      analogWrite(zum, (changer) ? 150 : 0); // возможно, изменить скважность
    }
    if(checkRightRotation()) {
      digitalWrite(zum, LOW);
      zum++;
      zum = constrain(zum, 5, 9);
      lcd.clear;
      printVolume();
      analogWrite(zum, (changer) ? 150 : 0); // возможно, изменить скважность
    }
  }
}

void printVolume() {     // мигание можно убрать, не очищая весь дисплей, а выводя вторую строку из пробелов (lcd.print("     ");)
  lcd.setCursor(1, 0);
  lcd.print("Set the volume");
  switch(zum) {
    case 5:
      lcd.setCursor(5, 1);
      lcd.write(1);
      break;
    case 6:
      for (byte i = 5; i < 7; i++) {
        lcd.setCursor(i, 1);
        lcd.write(1);
      }
      break;
    case 7:
      for (byte i = 5; i < 8; i++) {
        lcd.setCursor(i, 1);
        lcd.write(1);
      }
      break;
    case 8:
      for (byte i = 5; i < 9; i++) {
        lcd.setCursor(i, 1);
        lcd.write(1);
      }
      break;
    case 9:
      for (byte i = 5; i < 10; i++) {
        lcd.setCursor(i, 1);
        lcd.write(1);
      }
      break;    
  }
}

