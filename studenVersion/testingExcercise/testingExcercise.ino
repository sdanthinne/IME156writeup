//todo: add import of .h file
e2  //Objective: create a testing program to test your main LEDs, while using the potentiometer to change the color.


//Add any global variables that you might need 
int potMax = 0;
int potMin = 10000;
int potValue = 0;

void setup() {
  // this code is run at startup, no need to change

  //This line begins the serial output on channel 9600
  Serial.begin(9600);
  
  const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  // set up the ADC
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1);  // Set our own prescaler to 64; This doubles the sample rate.
  
  pinMode(ledPin0a, OUTPUT), pinMode(ledPin0b, OUTPUT), pinMode(ledPin0c, OUTPUT);//these lines allow the led pins to be labeled as outputs
  pinMode(ledPin1a, OUTPUT), pinMode(ledPin1b, OUTPUT), pinMode(ledPin1c, OUTPUT);
  pinMode(ledPin2a, OUTPUT), pinMode(ledPin2b, OUTPUT), pinMode(ledPin2c, OUTPUT);
  //Serial.begin(9600); // Enable Serial Port for Deugging Purposes.
  
  
}

void loop() {
  //<-- These two forward slashes denote comments, which do not affect the functionality of the code you write.
  //Write your testing code here:
  //The goal of this testing program is to build a LED color modulator that is based on the value read by the Arduino from the potentiometer.
  //This loop method is run continuously on the Arduino, which means that whatever code you write in here will be also run continuously.
  //Use the arduino's analogRead() on the "gainpin" attribute to read the values that the potentiometer puts out.
  //arduino has a Serial.print() function that prints the values that are read by the pin, so using this tool, 
  //with the code provided below, you will find the maximum and minimum values of the potentiometer and take note of these values.
  
  potValue = analogRead(gainPin);
  
  //this code below determines the highest and lowest values of the potentiometer. 
  //When the board is on and reading to the Serial Console that is present within the arduino IDE, 
  //use the Serial.print function to take note of the max and min readings of your potentiometer
  if(potValue<potMin){
    potMin = potValue;
  }else if(potValue>potMax){
    potMax = potValue;
  }
  
  Serial.println(potMin);
  Serial.println(potMax);
  
  //delays 2 ms
  delay(2);
  
  
  
  //Color in this program is interpreted in HSV format, where there are three values: Hue, Saturation, and Value. The only value that should mean something to you is Hue. It ranges from 0 to 360
  //Using this, split the potentiometer values so that each position of the potentiometer has a hue associated with it. (Hint: divide the difference of the max/min values subtracted the min of the potentiometer by 360)

  //finally, connect the read-in of the potentiometer to the HSV_to_RGB_LED method below, using the Saturation and Value values that you would like. Don't forget to pass in the pins of the LEDs to the function. 
  //This code will connect the hardware's input (Potentiometer) to a hardware output (LEDs)
  
  //divide the potentiometer value input to fit within the 360 degree hsv wheel
  //two ways of doing this:
  //using the modulus operator to make the potentiometer value loop back
  //using the division operator and dividing the input to fit within the 360 degree range
  //set the s,v and i parameters to 1,1 and 255, respectively.
  
  HSV_to_RGB_LED(); //parameter order is as follows: hue,saturation,value,i,pin0,pin1,pin2
  
  


}



  //Use this function to convert from HSV format to RGB and write to the LEDs
  
void HSV_to_RGB_LED(float h, float s, float v, float i, int pin0, int pin1, int pin2) {
  float c = s * v;
  float x = c * (1 - abs((fmod((h / 60), 2) - 1)));
  float m = v - c;
  float r, g, b;
  if (0 <= h && h < 60) {
    r = c; g = x; b = 0;
  }
  else if (60 <= h && h < 120) {
    r = x; g = c; b = 0;
  }
  else if (120 <= h && h < 180) {
    r = 0; g = c; b = x;
  }
  else if (180 <= h && h < 240) {
    r = 0; g = x; b = c;
  }
  else if (240 <= h && h < 300) {
    r = x, g = 0; b = c;
  }
  else if (300 <= h && h <= 360) {
    r = c; g = 0; b = x;
  }
  /* analogWrite() writes a PWM signal to the MOSFET transisors, which control the color of the LEDs.
   * 
   */
   
  //this line writes the colors to their respective pins, pin0,pin1,pin2
   
  analogWrite(pin0, floor(i * (r + m))), analogWrite(pin1, floor(i * (g + m))), analogWrite(pin2, floor(i * (b + m)));
}

  
