#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
#include <iarduino_SensorPulse.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);
iarduino_RTC time (RTC_DS1302,10,13,12);
iarduino_SensorPulse Pulse(A5,4); //инициализировать вывод зуммера (1)
#define zum 4;
#define sound 1;
#define butt 3;
boolean a=1;
unsigned long timer = 0;
unsigned long butt_timer = 0;
boolean mode = 1;
byte debounce = 60; // настройка времени дребезга контактов
void setup() {
    pinMode(zum, OUTPUT);
    Serial.begin(9600);
    Pulse.begin();
    time.begin();
    time.settime(0,45,1,1,12,18,6);
    lcd.init();
    lcd.noBacklight();
    lcd.noDisplay();
    pinMode(butt,INPUT_PULLUP);

}

void loop() {
      time.gettime();
  if (time.weekday  == 1 && (time.Hours == 7 || 15 || 16 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && time.minutes == 0) {
        alarm();
  }
    else if ((time.weekday  == 2 || 3)  && (time.Hours == 7 || 16 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && time.minutes == 0) {
        alarm();
        }
    else if (time.weekday  == 4  && (time.Hours == 7 ||  20 || 21 || 22 || 23 || 24) && time.minutes == 0) {
        alarm();
        }
    else if (time.weekday  == 5  && (time.Hours == 7 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && time.minutes == 0) {
        alarm();
        }
    else if ((time.weekday  == 6  && (time.Hours == 12 || 13 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && time.minutes == 0) {
        alarm();
        }
    else if ((time.weekday  == 0  && (time.Hours == 12 || 13 || 14 || 15 || 16 || 17 || 18 || 19 || 20 || 21 || 22 || 23 || 24) && time.minutes == 0) {
        alarm();
        }              
      }
void alarm () {
  lcd.backlight();
  while (Pulse.check(ISP_VALID)==ISP_DISCONNECTED  || (Pulse.check(ISP_VALID)==ISP_CONNECTED && Pulse.check(ISP_PULSE)<180 )) { //ввести значение пульса после нагрузки
  digitalWrite(zum,a); // возможно, строчкой выше нужно будет добавить if (Pulse.check(ISP_VALID)==ISP_DISCONNECTED) {}
  if (a == 1 & Pulse.check(ISP_VALID)==ISP_DISCONNECTED ) {
  lcd.display();
  lcd.setCursor(5, 0);
  lcd.print("Alarm,");
  lcd.setCursor(1, 1);
  lcd.print("time to sport!");
    }
    else if(a == 0 & Pulse.check(ISP_VALID)==ISP_DISCONNECTED){
      lcd.noDisplay();
      }
  if (millis()-timer >= 500) {  // проверить работает ли переключение, можно заменить на delay
  a= !a;
  timer = millis();
     }
      
  if (Pulse.check(ISP_VALID)==ISP_CONNECTED) {
  lcd.display();
  lcd.setCursor(5, 0);
  lcd.print("Pulse:");
  lcd.setCursor(6, 1);
  lcd.print(Pulse.check(ISP_PULSE));
    }
  if (!digitalRead(butt) && millis() - butt_timer > debounce){ 
    delay (600000);
    butt_timer = millis();
    }    
   } 
   while (Pulse.check(ISP_VALID)==ISP_CONNECTED & Pulse.check(ISP_PULSE)>=180)  {  //ввести значение пульса после нагрузки; при мерцании перед While поставить if  с тем же условием
  lcd.display();
  lcd.setCursor(5, 0);
  lcd.print("Good!");
  lcd.setCursor(4, 1);
  lcd.print"Pulse:"();
  lcd.setCursor(10, 1);
  lcd.print(Pulse.check(ISP_PULSE));
    }  
  lcd.noBacklight();
  lcd.noDisplay();   
}
