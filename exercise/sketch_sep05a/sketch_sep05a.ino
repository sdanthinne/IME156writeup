int ledPin = 10;
int switchPin = A3;

int potvalue = 0;

void setup() {
  // put your setup code here, to run once:
 pinMode(ledPin,OUTPUT);
 Serial.begin(9600);
 //what is 9600 baud?
 //9600 bits per second
 //serial ports are one bit after another, unlike parallel
}

void loop() { 
  // put your main code here, to run repeatedly:
  potvalue = analogRead(switchPin);
  
  analogWrite(ledPin,potvalue);
  Serial.println(potvalue);

  
  //digitalWrite(ledPin,HIGH);
//  delay(100);
//  digitalWrite(ledPin,LOW);
//  delay(100);
}
