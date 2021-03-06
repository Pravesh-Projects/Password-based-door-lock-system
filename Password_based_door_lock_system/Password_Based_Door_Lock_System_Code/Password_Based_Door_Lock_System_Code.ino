#include <Password.h>
#include <Keypad.h> 
#include <Servo.h>
#include <LiquidCrystal.h>
Password password = Password( "1234" );
Servo myservo;
int q=0;
const int trigPin=18;
const int echoPin=19;
long duration;
int distance;
const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {14,15,16,17};
byte colPins[COLS] = {6,7,8,9};
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){
  pinMode(13, OUTPUT);
  pinMode(21, OUTPUT);
  digitalWrite(21,HIGH);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  lcd.begin(16, 2);
  lcd.print("ENTER PASSWORD:");
  myservo.attach(10);
  myservo.write(180);  
  //digitalWrite(13, HIGH);   // sets the LED on
  Serial.begin(9600);
  keypad1.addEventListener(keypadEvent);
}
  
void loop(){
  keypad1.getKey();
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration=pulseIn(echoPin,HIGH);
  distance=duration*0.034/2;
  Serial.print("distance: ");
  Serial.println(distance);
  if(distance<10)
  {
    digitalWrite(21,LOW);
  }
}

void keypadEvent(KeypadEvent eKey){
  switch (keypad1.getState()){
  case PRESSED:
    Serial.print("Pressed: ");
    Serial.println(eKey);
    lcd.setCursor(q, 1);
    lcd.print("*");
    lcd.setCursor(q+1, 1);
    lcd.print("            ");
    switch (eKey){
      case '*': guessPassword();  break;
      case '#': password.reset(); digitalWrite(13,LOW);myservo.write(180);q=0;lcd.setCursor(q, 1);lcd.print("              "); digitalWrite(21,HIGH ); break;
      default: 
      password.append(eKey);q++;
    }
  }
}

void guessPassword(){
  if (password.evaluate())
  {
    digitalWrite(13,HIGH);
    myservo.write(90); 
    lcd.setCursor(0, 1);
    lcd.print("DOOR OPENED   "); 
    Serial.print("yes");
  }
  else
  {
    myservo.write(180);  
    digitalWrite(13,LOW);
    lcd.setCursor(0, 1);
    lcd.print("WRONG:-)      ");
  }
}

