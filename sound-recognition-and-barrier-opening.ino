#include <Wire.h>
#include <OLED_SH1106.h>
#include <SimpleTimer.h>
#include <TimeLib.h>
#include <Blynk.h>

#define BLYNK_DEBUG Serial
#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#define SerialAT Serial3

SimpleTimer timer;
TinyGsm modem(SerialAT);
WidgetTerminal terminal(V1);

#define OLED_RESET 4
OLED_SH1106 display(OLED_RESET);

#define LED_RED   6  //red LED for closed barrier
#define LED_GREEN 7  //green LED for opened barrier 
#define BUZZER    11 //piezoelectric speaker
#define servoPin  A0 //servomotor

int a, b;                       //for time counter
bool start_program = true;      
int  shlag_open_BLYNK_command;  //command to open the barrier via BLYNK
long time_open_shlag;           //counting the barrier opening time in sec
bool flag_open_shlag_button_BLYNK;
long time_interval_slag_display = 10000;   //time of showing the phrase SHLAG cloused on the displayin milisec
bool flag_close_slag_display;              //flag to remove the phrase SHLAG cloused
long time_close_slag_display;
long time_otkritie_shlaga = 6000;          //time in milisec when the barrier is open
int  shlag_open_sound_command;  //command for opening the barrier by the sound
int  number_sound;
long time_interval_1500_msec;   //to count the interval of 1500 milisec
bool flag_interval_1500_msec;   //flag when counting the time of 1500 milisec
bool flag_open_shlag_sound;     //flag when the barrier opens by the sound
String _response = "";          // module response

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix OLED_SH1106.h!");
#endif

BLYNK_WRITE(V1) //terminal
{
  if (String("clear") == param.asStr()) {
    terminal.clear();
  } 
  terminal.flush();
}

BLYNK_WRITE(V2) //button for the barrier opening
{
  shlag_open_BLYNK_command = param.asInt(); 
}

//controlling the servomotor
void servoPulse(int pin, int angle)
{
  // convert angle to 500-2480 pulse width
  int pulseWidth = (angle * 11) + 500; 
  digitalWrite(pin, HIGH); // set the level of servo pin as high
  delayMicroseconds(pulseWidth); // delay microsecond of pulse width
  digitalWrite(pin, LOW); // set the level of servo pin as low
  delay(20 - pulseWidth / 1000);
}

void sendOnOff() 
{
  long time_temp; 
  time_temp = millis();
  if((flag_close_slag_display) && ((time_temp - time_close_slag_display) > time_interval_slag_display)){
  //if the phrase is showing for more than 10 seconds, remove
    flag_close_slag_display = false;
    display.clearDisplay();
    display.display();      // Show initial text
    Serial.println("removing the phrase SHLAG cloused");
    if(Blynk.connected()) { 
      display.setCursor(30, 0);
      display.println(F("BLYNK"));
      display.setCursor(10, 25);
      display.println(F("connected"));
      display.display();
    }
  }
  
  a = a + 1;     //time counter++
  
  //LED blinking
  if(a%2) Blynk.virtualWrite(V4, 0); 
  else Blynk.virtualWrite(V4, 255);   
   if(start_program) 
   {
      terminal.print("Program loading    "); 
      terminal.flush(); 
      Serial.println("start sendOnOff");
      start_program = false;
   }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  //pins of sound module for the input
  pinMode(sound_01, INPUT); //A8
  pinMode(sound_02, INPUT); //A9
  pinMode(sound_03, INPUT); //A10
  pinMode(sound_04, INPUT); //A11
  pinMode(sound_05, INPUT); //A12
  pinMode(sound_06, INPUT); //A13
  pinMode(sound_07, INPUT); //A14

  pinMode(BUZZER, OUTPUT);  //piezoelectric speaker
  pinMode(LED_RED,   OUTPUT); //
  pinMode(LED_GREEN, OUTPUT); //
  digitalWrite(LED_RED,   HIGH); //turning the LED on for the test
  digitalWrite(LED_GREEN, HIGH);
  pinMode(servoPin,   OUTPUT); //
  //turning the servomotor off and setting to the start point
  for (int i = 0; i <= 50; i++) {
    servoPulse(servoPin, 17);
  }
  
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // I2C initialization with 0x3C address (for 128x64)
  display.display();
  
  SerialAT.begin(9600);
  delay(5000);
  
  Serial.println("Initializing modem...");
  modem.restart();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 0);
  display.println(F(" SIM800L"));
  display.setCursor(22, 25);
  display.println(F("restart"));
  display.display();  
  delay(5000);
  
  Blynk.begin(auth, modem, apn, user, pass);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 0);
  display.println(F("BLYNK"));
  display.setCursor(10, 25);
  display.println(F("connected"));
  display.display(); 
  
  timer.setInterval(1000L, sendOnOff);
        
  //turning the servomotor on
  for (int i = 0; i <= 50; i++) {
    servoPulse(servoPin, 90);
  }
  
  digitalWrite(BUZZER,   HIGH); 
  delay(200);
  digitalWrite(BUZZER,    LOW); 
  delay(200);
  digitalWrite(BUZZER,   HIGH); 
  delay(200);
  digitalWrite(BUZZER,    LOW); 
  delay(200);
  digitalWrite(BUZZER,   HIGH); 
  delay(200);
  digitalWrite(BUZZER,    LOW); 
  
  //turning the servomotor off 
  for (int i = 0; i <= 50; i++) {
    servoPulse(servoPin, 17);
  }  
  digitalWrite(LED_RED,  HIGH); //
  digitalWrite(LED_GREEN, LOW);
  Serial.println("START");
}

void loop()
{
  Blynk.run();
  timer.run();
  //opening the barrier if signal sounds
  if(digitalRead(sound_01) == 0 && shlag_open_sound_command == 0) {
    shlag_open_sound_command = 1;
    number_sound = 1;
    Serial.println("signal 1"); //
  }
  
  if(digitalRead(sound_02) == 0 && shlag_open_sound_command == 0) {
    shlag_open_sound_command = 1;
    number_sound = 2;
    Serial.println("signal 2"); //
  }
  
  if(digitalRead(sound_03) == 0 && shlag_open_sound_command == 0) {
    shlag_open_sound_command = 1;
    number_sound = 3;
    Serial.println("signal 3"); //
  }
  
  if(flag_open_shlag_button_BLYNK || flag_open_shlag_sound) {    
    if(millis() > time_open_shlag) {
      //закрываем шлагбаум
      digitalWrite(BUZZER,   LOW);
      digitalWrite(LED_RED, HIGH);
      if(flag_open_shlag_button_BLYNK) flag_open_shlag_button_BLYNK = 0;
      if(flag_open_shlag_sound)        flag_open_shlag_sound        = 0;
      digitalWrite(LED_GREEN, LOW);
      
      for (int i = 0; i <= 50; i++) {
        servoPulse(servoPin, 17);
      }
      
      Serial.println("закрываем шлагбаум"); 
      Blynk.virtualWrite(V3, 0); //turning the LED in BLYNK off
      Blynk.virtualWrite(V2, 0); //turning the button in BLYNK off
      display.clearDisplay();
      display.setTextSize(2); 
      display.setTextColor(WHITE);
      display.setCursor(15, 0);
      display.println(F(" SHLAG"));
      display.setCursor(22, 25);
      display.println(F("cloused"));
      display.display();    
      time_close_slag_display = millis(); //counting the time of showing the phrase "SHLAG cloused"
      flag_close_slag_display = true;  //flag = True of the counting start time of showing the phrase
    }
  }
  
  if(shlag_open_BLYNK_command || shlag_open_sound_command) {    
    if(shlag_open_BLYNK_command) {
      digitalWrite(LED_RED,   LOW); 
      digitalWrite(LED_GREEN, HIGH); 
      digitalWrite(BUZZER,   HIGH);
      
      for (int i = 0; i <= 50; i++) {
        servoPulse(servoPin, 90);
      }
      
      Serial.println("opening the barrier via BLYNK"); 
      shlag_open_BLYNK_command = 0;
      if(!flag_open_shlag_button_BLYNK) {
        flag_open_shlag_button_BLYNK = 1;
        Blynk.virtualWrite(V3, 255); //turning the LED on in "BLYNK"
        time_open_shlag = millis() + time_otkritie_shlaga; 
      }
      
      terminal.println(F("opening the barrier via the button"));
      terminal.flush();
      display.clearDisplay();
      display.setTextSize(2); 
      display.setTextColor(WHITE);
      display.setCursor(15, 0);
      display.println(F(" SHLAG"));
      display.setCursor(22, 25);
      display.println(F("is open"));
      display.display();    
    }
    
    if(shlag_open_sound_command && !flag_interval_1500_msec) {
      digitalWrite(LED_RED,   LOW); 
      digitalWrite(BUZZER,   HIGH);
      digitalWrite(LED_GREEN, HIGH);
      for (int i = 0; i <= 50; i++) {
        servoPulse(servoPin, 90);
      }
      
      Serial.println("opening the barrier by the signal"); 
      time_interval_1500_msec = millis();
      flag_interval_1500_msec = 1;
      flag_open_shlag_sound = 1;
      Blynk.virtualWrite(V3, 255); //turning the LED on in "BLYNK"
      time_open_shlag = millis() + time_otkritie_shlaga;
      terminal.print("opening the barrier by the signal");
      terminal.print(number_sound); 
      terminal.flush();
      display.println(F(" SHLAG is open"));
    }
  }
}
