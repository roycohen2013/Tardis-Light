


#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


volatile boolean f_wdt = 1;

//http://www.insidegadgets.com/2011/02/05/reduce-attiny-power-consumption-by-sleeping-with-the-watchdog-timer/


// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin =  3;  // Analog input pin that the potentiometer is attached to
const int ledPin = 0; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

int threshold = 0;
int darkVal=0;
int lightVal = 0;
float temp = 0;
float analogValTemp = 0;


void setup() {


  setup_watchdog(1); // approximately 4 seconds sleep


  delay(1000);
  temp = 0;
  for(int i =0; i<=100; i++){
    analogValTemp = analogRead(analogInPin);          // take value for illuminated room
    temp = temp + analogValTemp;
    delay(2);
  }
  lightVal = temp/100;

  delay(1000);


  analogWrite(ledPin, 255);         // blink the Leds 3 times
  delay(300);
  analogWrite(ledPin, 0);
  delay(300);  
  analogWrite(ledPin, 255); 
  delay(300);
  analogWrite(ledPin, 0);
  delay(300);
  analogWrite(ledPin, 255); 
  delay(300);
  analogWrite(ledPin, 0);


  temp = 0;
  for(int i =0; i<=100; i++){
    analogValTemp = analogRead(analogInPin);        // take average value for dark room
    temp = temp + analogValTemp;
    delay(2);
  }
  darkVal = temp/100;

  threshold = (darkVal+lightVal)/2;

}

void loop() {

  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag


    // read the analog in value:

    // map it to the range of the analog out:
    //outputValue = map(sensorValue, 0, 1023, 0, 255);  
    // change the analog out value:
    temp = 0.0;
    for(int i =0; i<=100; i++){
      analogValTemp = analogRead(analogInPin);        // take average value currently in room
      temp = analogValTemp + temp;
      delay(2);
    }
    sensorValue = temp/100;

    //sensorValue = analogRead(analogInPin);  

    if(sensorValue <= threshold){
      FadeCycle();
    }


  }


}


void FadeCycle(){
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  }
}


// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

    sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}

