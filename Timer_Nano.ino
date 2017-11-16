#include "Arduino.h"
/*
  On-Off Timer based on time settings in minutes
  Single output is driven accordingly to control any external load.
  4-digit 7segment display is used to show
  two digit on time and two digit off time
  4 push buttons are '+' and '-' commands to increase and decrease for on-time and
  off-time settings.
  one extra push button shows the current settings on press.

  created by Masood Alam
*/
#include <SevSeg.h>
#include <EEPROM.h>

SevSeg sevseg;

const int OUTPUT1= 1;
const int LED1 = LED_BUILTIN;//13;
int ledState = LOW;         // the current state of the output pin

const int button1Pin = A0;    // the number of the pushbutton pin
int button1State = HIGH;             // the current reading from the input pin
int lastButton1State = HIGH;   // the previous reading from the input pin
// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can't be stored in an int.
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled

const int button2Pin = A1;    // the number of the pushbutton pin
int button2State = HIGH;             // the current reading from the input pin
int lastButton2State = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled

const int button3Pin = A2;    // the number of the pushbutton pin
int button3State = HIGH;             // the current reading from the input pin
int lastButton3State = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime3 = 0;  // the last time the output pin was toggled

const int button4Pin = A3;    // the number of the pushbutton pin
int button4State = HIGH;             // the current reading from the input pin
int lastButton4State = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime4 = 0;  // the last time the output pin was toggled

const int button5Pin = A4;    // the number of the pushbutton pin
int button5State = HIGH;             // the current reading from the input pin

const int button6Pin = A5;    // the number of the pushbutton pin
int button6State = HIGH;             // the current reading from the input pin


unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


   int offTime=0;
   int onTime=0;
   int current_ontime;
   int current_offtime;
   char current_state;

// the setup function runs once when you press reset or power the board
void setup() {
  byte numDigits = 4;
  byte digitPins[] = {2,3,4,5};
  byte segmentPins[] = {6,7,8,9,10,11,12,1};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_ANODE;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED1, OUTPUT);
  pinMode(OUTPUT1, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(OUTPUT1, LOW);
  pinMode(button1Pin, INPUT);
  digitalWrite(button1Pin, HIGH);

  pinMode(button2Pin, INPUT);
  digitalWrite(button2Pin, HIGH);
  pinMode(button3Pin, INPUT);
  digitalWrite(button3Pin, HIGH);
  pinMode(button4Pin, INPUT);
  digitalWrite(button4Pin, HIGH);

  pinMode(button5Pin, INPUT);
  digitalWrite(button5Pin, HIGH);

  pinMode(button6Pin, INPUT);
  digitalWrite(button6Pin, HIGH);

  digitalWrite(LED1, ledState);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(OUTPUT1, ledState);
  Serial.begin(115200);
  onTime = EEPROM.read(2);
  if (onTime > 99)
    onTime = 0;
  offTime = EEPROM.read(3);
  if (offTime > 99)
    offTime = 0;

  current_ontime = EEPROM.read(4);
  if (current_ontime > onTime || current_ontime == 0xff)
    current_ontime = 0;
  offTime = EEPROM.read(3);
  if (offTime > 99)
    offTime = 0;
  current_offtime = EEPROM.read(5);
  if (current_offtime > offTime || current_offtime == 0xff)
    current_offtime = 0;

  current_state = EEPROM.read(6);
  if (current_state == 0)
    ledState = LOW;
   else
    ledState = HIGH;
  digitalWrite(LED1, ledState);
  digitalWrite(OUTPUT1, ledState);
  //if (onTime > 0 && offTime > 0)
  //    ledState = HIGH;
}

char charValue[4];

// the loop function runs over and over again forever
void loop() {

   static unsigned long timer = millis();
   int deciSeconds;
   if (button5State == LOW) {
      deciSeconds = (onTime*100)+ offTime;
//      if (ledState == HIGH) {
       charValue[0] = onTime/10 + '0';
       charValue[1] = onTime%10 + '0';
       charValue[2] = offTime/10 + '0';//'-';
       charValue[3] = offTime%10 + '0';//'-';
 //     }
  //    else {
   //    charValue[0] = onTime/10 + '0';//'-';
   //    charValue[1] = onTime%10 + '0';//'-';
   //    charValue[2] = offTime/10 + '0';
   //    charValue[3] = offTime%10 + '0';
   //   }
   }
   else {
      deciSeconds = ((onTime-current_ontime)*100)+ (offTime-current_offtime);
      if (ledState == HIGH) {
    	  charValue[0] = (onTime-current_ontime)/10 + '0';
    	  charValue[1] = (onTime-current_ontime)%10 + '0';
    	  charValue[2] = '-';
    	  charValue[3] = '-';
      }
      else {
    	  charValue[0] = '-';
    	  charValue[1] = '-';
    	  charValue[2] = (offTime-current_offtime)/10 + '0';
    	  charValue[3] = (offTime-current_offtime)%10 + '0';
      }
   }

   static unsigned long tick = millis();
   static int seconds=0;
  if (millis() >= tick)
  {
    seconds++;
      tick += 100;  // 100ms
  }

      if (button6State == LOW)
      {
          ledState = LOW;
          current_offtime = 0;
      }

    if (seconds > 600)		//10
    {
    Serial.print("onTime=");
    Serial.print(onTime);
    Serial.print(",current_ontime=");
    Serial.println(current_ontime);
    Serial.print("offTime=");
    Serial.print(offTime);
    Serial.print(",current_offtime=");
    Serial.println(current_offtime);


      if (ledState == HIGH)
      {
       current_offtime = 0;
       current_ontime++;
        EEPROM.write(4, current_ontime);
        EEPROM.write(5, current_offtime);
        EEPROM.write(6, ledState);
       if (current_ontime >= onTime)
          ledState = LOW;
      }
      else
      {
        current_offtime++;
        current_ontime = 0;
        if (current_offtime >= offTime)
          ledState = HIGH;
        EEPROM.write(4, current_ontime);
        EEPROM.write(5, current_offtime);
        EEPROM.write(6, ledState);
      }
    //Serial.print("ledState=");
    //Serial.println(ledState);

      seconds = 0;
    }
  if (millis() >= timer) {
//    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
    timer += 100;
//    if (deciSeconds == 10000) { // Reset to 0 after counting for 1000 seconds.
//      deciSeconds=0;
 //   }
    //sevseg.setNumber(deciSeconds, 1);
    sevseg.setChars(charValue);
  }

  sevseg.refreshDisplay(); // Must run repeatedly




  int reading = digitalRead(button1Pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButton1State) {
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != button1State) {

      if (reading == HIGH && button1State == LOW) {
        if (onTime < 99)
          onTime++;
          EEPROM.write(2, onTime);
      }
      button1State = reading;
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButton1State = reading;

  reading = digitalRead(button2Pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButton2State) {
    // reset the debouncing timer
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != button2State) {

      // only toggle the LED if the new button state is HIGH
      if (reading == HIGH && button2State == LOW) {
        if (onTime > 0)
          onTime--;
          EEPROM.write(2, onTime);
      }
      button2State = reading;

    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButton2State = reading;


  reading = digitalRead(button3Pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButton3State) {
    // reset the debouncing timer
    lastDebounceTime3 = millis();
  }

  if ((millis() - lastDebounceTime3) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != button3State) {

      // only toggle the LED if the new button state is HIGH
      if (reading == HIGH && button3State == LOW) {
        if (offTime < 99)
          offTime++;
          EEPROM.write(3, offTime);
      }
      button3State = reading;

    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButton3State = reading;


  reading = digitalRead(button4Pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButton4State) {
    // reset the debouncing timer
    lastDebounceTime4 = millis();
  }

  if ((millis() - lastDebounceTime4) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != button4State) {

      // only toggle the LED if the new button state is HIGH
      if (reading == HIGH && button4State == LOW) {
        if (offTime > 0)
          offTime--;
          EEPROM.write(3, offTime);
      }
      button4State = reading;

    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButton4State = reading;


  reading = digitalRead(button5Pin);

  button5State = reading;

  reading = digitalRead(button6Pin);

  button6State = reading;

  //Serial.println(lastButton2State);
    // set the LED:
  digitalWrite(LED1, ledState);
  digitalWrite(OUTPUT1, ledState);

}
