#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
#include <iarduino_SensorPulse.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);
iarduino_RTC time (RTC_DS1302,10,13,12);
iarduino_SensorPulse Pulse(A0,4); //инициализировать вывод зуммера (1)
#define zum 4
#define sound 1 // возможно удалить
#define butt 3
boolean changer=1;
unsigned long timer = 0;
unsigned long butt_timer = 0;
boolean mode = 1; // возможно удалить
byte debounce = 60; // настройка времени дребезга контактов
int heartbeat = 120;
boolean flag = 0;
unsigned long last_good = 0;
int count_pulse = 0;
void setup() {
    pinMode(zum, OUTPUT);
    Serial.begin(9600); // возможно удалить
    Pulse.begin();
    time.begin();
    time.settime(30,59,18,1,12,18,6);
    lcd.init();
    lcd.noBacklight();
    lcd.noDisplay();
    pinMode(butt,INPUT_PULLUP);
}
void loop() {
    digitalWrite(zum, LOW);
      time.gettime();
  if ((time.weekday == 1) && (time.Hours == 7 || 15 || 16 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && (time.minutes == 0) && (time.seconds ==0)) {
        alarm();
  }
    else if ((time.weekday == 2 || 3) && (time.Hours == 7 || 16 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && (time.minutes == 0) && (time.seconds ==0)) {
        alarm();
        }
    else if ((time.weekday == 4) && (time.Hours == 7 ||  20 || 21 || 22 || 23 || 24) && (time.minutes == 0) && (time.seconds ==0)) {
        alarm();
        }
    else if ((time.weekday == 5) && (time.Hours == 7 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && (time.minutes == 0) && (time.seconds ==0)) {
        alarm();
        }
    else if ((time.weekday == 6) && (time.Hours == 12 || 13 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && (time.minutes == 0) && (time.seconds ==0)) {
        alarm();
        }
    else if ((time.weekday == 0) && (time.Hours == 12 || 13 || 14 || 15 || 16 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && (time.minutes == 0) && (time.seconds ==0)) {
        alarm();
        }              
      }
void alarm () {
  lcd.backlight();
  while (flag == 0) {
  while (Pulse.check(ISP_VALID)==ISP_DISCONNECTED  || (Pulse.check(ISP_VALID)==ISP_CONNECTED && Pulse.check(ISP_PULSE)< heartbeat ) && flag == 0) { //ввести значение пульса после нагрузки
  if (changer == 1 && Pulse.check(ISP_VALID)==ISP_DISCONNECTED ) {
  digitalWrite(zum,changer);
  lcd.display(); 
  lcd.setCursor(5, 0);
  lcd.print("Alarm,");
  lcd.setCursor(1, 1);
  lcd.print("time to sport!");
    }
    else if(changer == 0 && Pulse.check(ISP_VALID)==ISP_DISCONNECTED){
      digitalWrite(zum,changer);
      lcd.noDisplay();
      }
  if (millis()-timer >= 500) {  // проверить работает ли переключение, можно заменить на delay
  changer= !changer;
  timer = millis();
     }     
  if (Pulse.check(ISP_VALID)==ISP_CONNECTED) { // если мерцает неприятно, то можно сначала if (Pulse.check(ISP_VALID)==ISP_CONNECTED), а потом while((Pulse.check(ISP_VALID)==ISP_CONNECTED) && (Pulse.check(ISP_PULSE)<180)) и убрать lcd.clear()
  lcd.display();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Pulse:");
  lcd.setCursor(6, 1);
  lcd.print(Pulse.check(ISP_PULSE));
    }
  if (!digitalRead(butt) && millis() - butt_timer > debounce){ 
      int a = 10;
      int b = 0;
    for (int count = 0; count <= 600; count++){
      lcd.display();
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Pause");
      lcd.setCursor(2, 1);
      lcd.print(a);
      lcd.setCursor(5, 1);
      lcd.print("min");
      lcd.setCursor(9, 1);
      lcd.print(b);
      lcd.setCursor(12, 1);
      lcd.print("sec");
      unsigned long t = millis(); // попробовать инициализировать t над циклом for
      while (millis()-t < 1000){
        }
      b--;
      if (b<0){
        a--;
        b=59;
        }  
             }
    }    
   } 
   if (Pulse.check(ISP_VALID)==ISP_CONNECTED && Pulse.check(ISP_PULSE)> heartbeat && millis()-last_good > 30 ){
    last_good = millis();
    count_pulse++;
    if(count_pulse > 30){
      flag = 1;
      count_pulse = 0;
      }
    }
  }
  flag = 0;
   while (Pulse.check(ISP_VALID)==ISP_CONNECTED && Pulse.check(ISP_PULSE)>=heartbeat)  {  //ввести значение пульса после нагрузки; при мерцании перед While поставить if  с тем же условием
  lcd.display();
  lcd.setCursor(5, 0);
  lcd.print("Good!");
  lcd.setCursor(4, 1);
  lcd.print("Pulse:");
  lcd.setCursor(10, 1);
  lcd.print(Pulse.check(ISP_PULSE));
    }  
  lcd.noBacklight();
  lcd.noDisplay();   
}
