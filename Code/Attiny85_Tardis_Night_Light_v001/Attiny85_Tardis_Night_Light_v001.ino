
/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
 Center pin of the potentiometer goes to the analog pin.
 side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 */

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
  
  
  delay(1000);
temp = 0;
  for(int i =0; i<=100; i++){
    analogValTemp = analogRead(analogInPin);          // take value for illuminated room
    temp = temp + analogValTemp;
    delay(4);
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
    delay(4);
  }
  darkVal = temp/100;

  threshold = (darkVal+lightVal)/2;

}

void loop() {
  

  
  // read the analog in value:

  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);  
  // change the analog out value:
  temp = 0.0;
  for(int i =0; i<=100; i++){
    analogValTemp = analogRead(analogInPin);        // take average value currently in room
    temp = analogValTemp + temp;
    delay(4);
  }
  sensorValue = temp/100;
  
/*
     mySerial.println("threshold");
     mySerial.println(threshold);
     mySerial.println(" ");
     mySerial.println("sensorVal");
     mySerial.println(sensorValue);
*/


  //sensorValue = analogRead(analogInPin);  

if(sensorValue <= threshold){
  FadeCycle();
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
