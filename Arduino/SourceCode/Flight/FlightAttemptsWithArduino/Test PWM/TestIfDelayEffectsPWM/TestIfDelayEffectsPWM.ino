/* Observations: Pin 5 and 6 operate at double the frequency of 
 *               the other analog out pins but the mean voltage
 *               is the same
 */

//int ledPin = 9;      // LED connected to digital pin 9
int analogPin = 3;   // potentiometer connected to analog pin 3
int val = 0;         // variable to store the read value
int led = 13;

void setup()
{
  pinMode(3, OUTPUT);   // sets the pin as output
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(led, OUTPUT);
}
void loop()
{
  //val = analogRead(analogPin);   // read the input pin
  val = 256/4;
  
  analogWrite(3, val);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(5, val);
  analogWrite(6, val);
  analogWrite(9, val);
  analogWrite(10, val);
  analogWrite(11, val);
  
  // LED blinks to show that delay is being used.
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
}
