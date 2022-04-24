#include<Time.h>
#include<TimeAlarms.h>
#include<LiquidCrystal.h>

int x = 0;
long previousMillis = 0;
long intervalIdle = 300;
int LCDalarm=0;

LiquidCrystal lcd (12, 11, 5,4,3,2);

const int relayPinA0=A0;
const int relayPinA1=A1;
const int relayPinA2=A2;
const int relayPinA3=A3;
int potPin1 =A4;
int potPin2 =A5;
const int relayOverride = 0;
const int alarm1EnablePin = 1 ;
const int minutePin = 6;
const int hourPin =7;
const int timeSetButton = 8;
int buzzer = 9;
const int alarmLED = 13 ;   
const int alarm1SetButton = 10;
int pumpDuration = 0;
int dayInterval =1;
unsigned char delayms = 250;
int minutePushCounter =0;
int minuteState= 0;
int lastminuteState = 0;
int hourPushCounter =0;
int hourState= 0;
int lasthourState = 0;
int setButtonState = 0;
int alarmHRPushCounter =1;
int alarmMinPushCounter =1;
int alarmHR = 0;
int alarmHRState = 0;
int lastalarmHRState = 0;
int alarmMin = 0;
int alarmMinState = 0;
int lastalarmMinState = 0;

byte water[8] ={
B01110,   B00100,  B10100,  B11110,  B11111,  B10001,  B00000,  B00001};

byte calendar[8] ={  
B01010,  B11111,  B11111,  B10001,  B10001,  B10001,  B10001,  B11111};

 int dayoftheweek = 1 ;
 
void setup() {
  lcd.begin(20,4);
  
  setTime(12,00,05,5,9,1999);
  
 pinMode(minutePin, INPUT_PULLUP);
 pinMode(hourPin, INPUT_PULLUP);
 pinMode(timeSetButton, INPUT_PULLUP);
 pinMode(alarm1SetButton, INPUT_PULLUP);
 pinMode(alarm1EnablePin, INPUT_PULLUP);

 pinMode(alarmLED, OUTPUT);
 pinMode(relayOverride, INPUT_PULLUP);
 
 pinMode(relayPinA0, OUTPUT);
 pinMode(relayPinA1, OUTPUT);
 pinMode(relayPinA2, OUTPUT);
 pinMode(relayPinA3, OUTPUT);
  
 lcd.createChar(1,water);
 lcd.createChar(2,calendar);
 
}

void loop() {
  
  digitalClockDisplay();
  minute_reset59();
  hour_reset24();
  setAlarmHR_Min();
  setTime_Hour_Minute();
  AmPm();
  
 lcd.setCursor(0,3); lcd.write(1); lcd.print(" every "); printDigits(dayInterval); lcd.setCursor(10,3); lcd.print(" days");
 lcd.setCursor(17,3); lcd.write(2); lcd.setCursor(18,3); printDigits(day());
 if (second()==59){lcd.clear(); }
 
 if (digitalRead(alarm1EnablePin)==LOW) { 
   lcd.setCursor(1,1); lcd.print("=ACTIVE");
    if (day()%dayInterval==0)
     alarm();   }
     
  else{
     lcd.setCursor(1,1); lcd.print("=off   ");
     
     digitalWrite(relayPinA0,LOW); digitalWrite(relayPinA1,LOW);
     digitalWrite(relayPinA2,LOW); digitalWrite(relayPinA3,LOW);
     digitalWrite(alarmLED,LOW);
       }

  if (hour()== alarmHRPushCounter && minute()==alarmMinPushCounter && second()==pumpDuration/29)
  {
    digitalWrite(relayPinA0,LOW); delay(200);
    digitalWrite(relayPinA1,LOW); delay(200);
    digitalWrite(relayPinA2,LOW); delay(200);
    digitalWrite(relayPinA3,LOW);  
   }
  
  if (digitalRead(relayOverride)==LOW && digitalRead(alarm1EnablePin)==LOW)
{   lcd.setCursor(15,1); lcd.print("OvrR");
    digitalWrite(relayPinA0,HIGH); delay(200);
    digitalWrite(relayPinA1,HIGH); delay(200);
    digitalWrite(relayPinA2,HIGH); delay(200);
    digitalWrite(relayPinA3,HIGH);          }
   }

 void setAlarmHR_Min() {
 while  (digitalRead(alarm1SetButton)==LOW) {
   
    alarmHRState = digitalRead(hourPin); 
   if (alarmHRState != lastalarmHRState) {
     if (alarmHRState == LOW) {
      alarmHRPushCounter ++; }}
 
   alarmMinState = digitalRead(minutePin); 
   if (alarmMinState != lastalarmMinState) {
     if (alarmMinState == LOW) {
      alarmMinPushCounter ++; }}  
   
     lastalarmHRState = alarmHRState;
     lastalarmMinState = alarmMinState;     
   digitalClockDisplay();
   minute_reset59();
   hour_reset24();
     }
 }

void alarm(){
  
    if (hour()== alarmHRPushCounter && minute()==alarmMinPushCounter && second()<=1){
    digitalWrite(relayPinA0,HIGH);
    delay(100); digitalWrite(relayPinA1,HIGH);
    delay(100); digitalWrite(relayPinA2,HIGH);
    delay(100); digitalWrite(relayPinA3,HIGH);          }
  
  if (hour()== alarmHRPushCounter && minute()==alarmMinPushCounter && second()<=4){  
    analogWrite(buzzer, 10);                      
    delay(delayms);
   analogWrite(buzzer, 0);
    delay(delayms);

 unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis > intervalIdle) {
   previousMillis = currentMillis;   
   if (LCDalarm == 0){
     LCDalarm = LCDalarm + 1;
      lcd.setCursor(15,1); lcd.print(" (");lcd.write(1);lcd.print(") ");
      digitalWrite(alarmLED,HIGH);
       }
   else  {
     lcd.setCursor(15,1); lcd.print("( ");lcd.write(1);lcd.print(" )");                                                             
     LCDalarm = LCDalarm - 1;
     digitalWrite(alarmLED,LOW);
      }             }   }
}

void minute_reset59() {
  if (minutePushCounter > 59){
  minutePushCounter = 0;}
  
  if (alarmMinPushCounter > 59){
  alarmMinPushCounter = 0;}  }

void hour_reset24() {
  if (hourPushCounter >23){
  hourPushCounter = 0;}
  
   if (alarmHRPushCounter >23){
  alarmHRPushCounter = 0;} }

  void setTime_Hour_Minute () {
     minuteState = digitalRead(minutePin);
   if (minuteState != lastminuteState) {
     if (minuteState == LOW) {
       minutePushCounter++;
     }
   }
   
   hourState = digitalRead(hourPin);
   if (hourState != lasthourState) {
     if (hourState == LOW) {
       hourPushCounter++;
      }
   }
    lastminuteState = minuteState;
    lasthourState = hourState;
 
  setButtonState = digitalRead(timeSetButton);
    if (setButtonState == LOW) {
    setTime(hourPushCounter, minutePushCounter,00,dayoftheweek, 7, 2000);
    }
  }

void AmPm () {
  if (hour()>=12) {lcd.setCursor(18,0); lcd.print("PM"); }
  else { lcd.setCursor(18,0); lcd.print("AM"); } }
  
void digitalClockDisplay() {
 pumpDuration = analogRead(potPin1);
 dayInterval = analogRead(potPin2);
 dayInterval = map(dayInterval, 0,1000, 1,7);
 
  lcd.setCursor(4,0); lcd.print("Time");
  lcd.setCursor(9,0); printDigits(hour()); lcd.print(":");
  lcd.setCursor(12,0);printDigits(minute());lcd.print(":");
  lcd.setCursor(15,0); printDigits(second());
  lcd.setCursor(0,1); lcd.write(1);
 
  lcd.setCursor(9,1); lcd.print("Set @ ");
  lcd.setCursor(15,1); printDigits(alarmHRPushCounter); lcd.print(":");
  lcd.setCursor(18,1); printDigits(alarmMinPushCounter);  
  lcd.setCursor(0,2); lcd.print("Pump duration: "); printDigits(pumpDuration/29);
  lcd.setCursor(17,2); lcd.print("sec");
}

void printDigits(int digits) {
  if (digits <10)
  lcd.print('0');
  lcd.print(digits);
}