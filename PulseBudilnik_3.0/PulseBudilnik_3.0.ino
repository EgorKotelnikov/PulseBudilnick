#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);
iarduino_RTC time (RTC_DS1302,10,13,12);
#define zum 4
#define sound 1 // возможно удалить
#define butt 3
int myInts[5];
boolean changer=1;
unsigned long mode_timer = 0;
unsigned long butt_timer = 0;
unsigned long good_timer = 0;
unsigned long last_good = 0;
unsigned long timer = 0;
unsigned long lasttime = 0;
unsigned long timer2 = 0;
boolean mode = 1; // возможно удалить
byte debounce = 60; // настройка времени дребезга контактов
int heartbeat = 120;
int lowline = -45;
int maxval = -100;
int highline = 0;
int pulse = 0;
int w = 0;
int count = 0;
boolean flag = 0;
boolean flag2 = 0;
boolean pyf = 0;
int count_pulse = 0;
void setup() {
    pinMode(A0, INPUT);
    pinMode(zum, OUTPUT);
    Serial.begin(9600); // возможно удалить
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
  while (flag2 == 0) {
  checking();     
  if (pyf == 0  || (pyf == 1 && w < heartbeat )) { //ввести значение пульса после нагрузки
  if (changer == 1 && pyf == 0 ) {
  digitalWrite(zum,changer);
  lcd.display(); 
  lcd.setCursor(5, 0);
  lcd.print("Alarm,");
  lcd.setCursor(1, 1);
  lcd.print("time to sport!");
    }
    else if(changer == 0 && pyf == 0){
      digitalWrite(zum,changer);
      lcd.noDisplay();
      }
  if (millis()-mode_timer >= 500) {  // проверить работает ли переключение, можно заменить на delay
  changer= !changer;
  mode_timer = millis();
     }     
  if (pyf == 1) { // если мерцает неприятно, то можно сначала if (Pulse.check(ISP_VALID)==ISP_CONNECTED), а потом while((Pulse.check(ISP_VALID)==ISP_CONNECTED) && (Pulse.check(ISP_PULSE)<180)) и убрать lcd.clear()
  lcd.display();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Pulse:");
  lcd.setCursor(6, 1);
  lcd.print(w);
  }
  if (!digitalRead(butt) && millis() - butt_timer > debounce){ 
      int a = 10;
      int b = 0;
      butt_timer = millis();
      digitalWrite(zum, 0);
    for (int count_pause = 0; count_pause <= 600; count_pause++){
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
   if (pyf == 1 && w > heartbeat && millis()-last_good > 1000 ){
    last_good = millis();
    count_pulse++;
    if(count_pulse > 5){
      flag2 = 1;
      count_pulse = 0;
      }
    }
  }
  flag2 = 0;
  good_timer = millis();
   while (millis() - good_timer < 5000)  {  //ввести значение пульса после нагрузки; при мерцании перед While поставить if  с тем же условием
  checking();
  lcd.display();
  lcd.clear(); 
  lcd.setCursor(5, 0);
  lcd.print("Good!");
  lcd.setCursor(4, 1);
  lcd.print("Pulse:");
  lcd.setCursor(10, 1);
  lcd.print(w);
    }  
  lcd.noBacklight();
  lcd.noDisplay();   
}
int readMean(int pin, int samples){
  // переменная для хранения суммы считанных значений
  int sum = 0;
  // чтение и складывание значений
  for (int i = 0; i < samples; i++){
    sum = sum + analogRead(pin);
  }
  // делим сумму значений на количество измерений
  sum = sum/samples;
  // возвращаем среднее значение
  return sum;
}
void checking()  {
  int pul1 = readMean(0, 350);
  if(pul1 >= lowline && pul1 <= highline && pul1 > maxval && flag ==0) {
    maxval = pul1;
    timer = millis();
    }
  if(pul1 > highline) {
    maxval = -100;
    timer = lasttime;
    flag = 1;   
    }  
  if(pul1 < lowline) {
    flag = 0;
    pulse = 60000/(timer - lasttime);
    lasttime = timer;
    maxval = -100;
    }
  if((pulse < 50 || pulse > 150)&& (millis()-timer2 < 1500) && w != 0) {
    pyf = 1;
    }
  if(pulse >= 50 && pulse <= 150) {
      myInts[count] = pulse;
      count++;
      if (count > 4){
        count = 0;
      }
      if (myInts[4] != 0) {
        w = (myInts[0]+myInts[1]+myInts[2]+myInts[3]+myInts[4])/5;
      }
      else {
        pyf = 0;   
      }
    timer2 = millis();
    }
  if (w != 0 && millis() - timer2 < 1500) {
    pyf = 1;
  }
  if (myInts[0] != 0 && myInts[4] == 0) {    // можно придумать флажок с надписью "wait"
  }
  if (millis() - timer2 >= 1500) {
    pyf = 0;
    for (int i=0; i<5; i++) {
      myInts[i] = 0;
    }
    count = 0;
    w = 0;
    } 
}
