#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


int GMinterruptPin = 2;
int resetPin = 3;
int calibrationPin = A7;
int buzzerPin = 4;

//double radiationCalibration = 0.0057;
double radiationCalibration = 0.0000000057;

double radiationDose_sievert = 0;

int counts = 0;
int counts_min = 0;

long millis_start;
long time_min_old;

long time_sec;
long time_min;
long time_hour;

String message = "test1";
String secondMessage = "test2";
bool showMessage = false;
bool showSecondMessage = false;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);
  lcd.setCursor(0,1);
  lcd.print(secondMessage);
  digitalWrite(buzzerPin,LOW);
  pinMode(GMinterruptPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GMinterruptPin),pulse,RISING);
  millis_start = millis();
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  time_sec = (millis()-millis_start)/1000;
  time_min = time_sec/60;
  time_hour = time_min/60;

  //radiationCalibration = (map(analogRead(calibrationPin),0,1023,50,100))/10000.0;
  //Serial.println("radiationCalibration:"+String(radiationCalibration,4));


  delay(50);

  if(showMessage)
  {
  digitalWrite(buzzerPin,HIGH);
  lcd.setCursor(0,0);
  lcd.print(message);
  
  digitalWrite(buzzerPin,LOW);
  showMessage = false;
  }

  if(showSecondMessage)
  {
      lcd.setCursor(0,1);
      //lcd.print(secondMessage);
      lcd.print(radiationDose_sievert,9);
      lcd.setCursor(11,1);
      lcd.print(" Sv/h");
      showSecondMessage = false;
  }
  
  
  if(time_min>time_min_old)
  {
    radiationDose_sievert = (counts_min*radiationCalibration);
    /*Serial.println((String)time_hour+":"+(String)time_min+":"+(String)time_sec+"|CPM: "+(String)counts_min+"|");
    Serial.print(radiationDose_microsievert,2);
    Serial.println(" uSv/h");*/
    
  secondMessage = "CPM:"+(String)counts_min+"|"+(String)((double)radiationDose_sievert); //+"Sv/h";
  //radiationDose_sievert = (counts_min*radiationCalibration)/1000000;
  
  showSecondMessage = true;
  //lcd.setCursor(0,1);
  //lcd.println("radiationCalibration:"+String(radiationCalibration,4));*&
    
    counts_min = 0;
  }

  if(digitalRead(resetPin) == LOW)
  {
    millis_start = millis();
    counts = 0;
    counts_min = 0;
  }

  time_min_old = time_min;
  

}

void pulse()
{
  //digitalWrite(buzzerPin,HIGH);
  counts++;
  /*Serial.println("Total counts: "+(String)counts);
  Serial.println("radiationCalibration:"+String(radiationCalibration,4));*/
  message = "TCs:"+(String)counts;
  showMessage = true;
  //lcd.setCursor(0,1);
  //lcd.println("radiationCalibration:"+String(radiationCalibration,4));*/
  counts_min++;
  
}
