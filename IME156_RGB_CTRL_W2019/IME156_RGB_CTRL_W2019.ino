#include "IME156_RGB_CTRL_W2019.h"

/* LED LIGHT CONTROLLER FOR EDM PRISM
LAST UPDATE: 1/13/2019
*/

/* TRY PLAYING WITH THE NUMBERS HERE TO SEE HOW THE LIGHT COLORS CHANGE!
  Make sure that the upper value is larger than the lower value.
  (ex. range_lower_a = 30, range_higher_a = 50)
*/

int range_lower_a  = 0;
int range_upper_a = 120;
int range_lower_b = 80;
int range_upper_b = 280;
int range_lower_c = 240;
int range_upper_c = 360;

/* CHANGE "Transition_Speed" VALUE TO CONTROL THE FADE SPEED. 
 *  This function adds a delay (in milliseconds) inside the ones-shot-counter (read forward) 
 *  to slow it down. This makes the lights transition mores slowly as the program state
 *  changes.
*/

int Transition_Speed = 50;

/* The "sensorValueN, thresholdN, and potentiometerValue" are variables to store 
 *  the input to the analogRead pins (A0-A3). We initialize them here. Note that 
 *  these lines do not do anything other than reserve space in memory to store the 
 *  variables.
*/
int sensorValue0 = 0;  // variable to store the value coming from the sensor
int sensorValue1 = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;  // variable to store the value coming from the sensor
int potentiometerValue = 0;

int threshold1 = 0;
int threshold2 = 0;
int threshold3 = 0;

/* "i, j, k" act as counter variables. Since there are three LEDs on the board, we
 *  need a separate counter to control each of them. Recall that each time the void loop()
 *  runs, the counter is incremented (or decremented) ONCE. Note that "i, j, k" have a difference
 *  of about 120 (degrees) from each other. This is done to create the maximum seperation in the
 *  HSV color wheel.
*/
int i = 1;
int j = 120;
int k = 240;

int delayVal = 12;
bool state = 0;
bool one_shot = 0;
int one_shot_counter_state0 = 0;

void setup() {

/* The code in the void setup() block initializes the hardware on the Arduino. You do not need to touch
 *  any of this code to modify how the code operates, but you can read more about pinMode functions here:
 *  pinMode: https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 *  ADCSRA sets up the clock speed of analog to digital conversion. DO NOT MAKE CHANGES TO IT!
*/

  const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  // set up the ADC
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1);  // Set our own prescaler to 64; This doubles the sample rate.

  pinMode(ledPin0a, OUTPUT), pinMode(ledPin0b, OUTPUT), pinMode(ledPin0c, OUTPUT);//these lines allow the led pins to be labeled as outputs
  pinMode(ledPin1a, OUTPUT), pinMode(ledPin1b, OUTPUT), pinMode(ledPin1c, OUTPUT);
  pinMode(ledPin2a, OUTPUT), pinMode(ledPin2b, OUTPUT), pinMode(ledPin2c, OUTPUT);
  //Serial.begin(9600); // Enable Serial Port for Deugging Purposes.
}

/* The vood loop() is where the bulk of your program runs. This loop runs over and over again until the
 *  Arduino is shutoff or reset.
*/
void loop() {

/* analogRead() reads the values from each of the analog to digital pins (A0-A3). This is the best way
 *  read sensor data.
*/
 
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);//maybe values for microphone?
  potentiometerValue = analogRead(gainPin);//dial given

/* This "if" statment simply says that if the value read from the potentiometer is higher than 900, transition
 *  the program to "state 0". State 0 corresponds to the debug/test mode, where the LEDS should cycle through all
 *  colors at 120 HSV degrees difference across the LEDs. When state is set to 1, the program is set to normal
 *  microphone-reactive operation.
*/

  if (potentiometerValue > 900) {
    state = 0;
  }
  else {
    state = 1;
  }

/* Note that code follows a sequential order -- now that the state has been decided by lines 94-99, we can modify
 *  the functionality for each of the states.
 */
  if (state == 0) { //THIS PART OF THE CODE CONTROLS DEBUG MODE.

/* The one_shot_counter_state0 allows a smooth fade transition between state 1 and 0. If we made the state change 
 *  instantaneously, it would appear too abrupt and unprofessional.
 */
 
    if (one_shot_counter_state0 < 255) {
      delayVal = 20;
      one_shot_counter_state0++;
    }
    else {
      delayVal = Transition_Speed;
    }

/* The HSV_to_RBG_LED() function simply converts HSV values to RGB. We do this because HSV is intuitively easier 
 *  for a human to understand, while the machine only understands RGB. You can read more about the HSV color space
 *  here: https://en.wikipedia.org/wiki/HSL_and_HSV
 */
 
    HSV_to_RGB_LED(i, 1, 1, one_shot_counter_state0, ledPin0a, ledPin0b, ledPin0c);
    HSV_to_RGB_LED(j, 1, 1, one_shot_counter_state0, ledPin1a, ledPin1b, ledPin1c);
    HSV_to_RGB_LED(k, 1, 1, one_shot_counter_state0, ledPin2a, ledPin2b, ledPin2c);

/* Here we modify the counters by incrementing them once. For example, "i++" means to add a value of "1" to the
 * value of "i". A value of 1 is added each time this loop runs, which means that the Hue is incremented by "1"
 * (see how we put the variable "i" into the function in line 123: HSV_to_RGB_LED(i, 1, 1, one_shot_counter...).
 * We repeat this procedure for j and k. If i, j, or k exceed 360 (degrees), the counter wraps around and restarts
 * at 0
 */

    if (i >= 360) {
      i = 0;
    }
    else {
      i++;
    }
    if (j >= 360) {
      j = 0;
    }
    else {
      j++;
    }
    if (k >= 360) {
      k = 0;
    }
    else {
      k++;
    }
  }

/* In this "else if" clause, we assume the program is in state 1, determined by lines 94-99. Modifying any of the
 *  code here will change how the microphone-reactive state functions.
 */
  
  else if (state == 1) { //THIS PART OF THE CODE CONTROLS SOUND-REACTIVE MODE.

/* Again, we include one_shot_counter_state0 to allow a smooth fade transition between state 0 and 1. If we made the state change 
 *  instantaneously, it would appear too abrupt and unprofessional.
 */

    if (one_shot_counter_state0 > 0) {
      delayVal = 5;
      one_shot_counter_state0--;
      HSV_to_RGB_LED(i, 1, 1, one_shot_counter_state0, ledPin0a, ledPin0b, ledPin0c);
      HSV_to_RGB_LED(j, 1, 1, one_shot_counter_state0, ledPin1a, ledPin1b, ledPin1c);
      HSV_to_RGB_LED(k, 1, 1, one_shot_counter_state0, ledPin2a, ledPin2b, ledPin2c);
    }
    else {
      
/* delayVal changes the time for how long the program loop should run. By increasing this value, the lights may blink faster (if lower)
 *  or slower (if higher), since this also increases the time for how long the microphone values are read. If you make this value too long,
 *  the values read from the microphone will persist longer so that the program cannot respond as quickly.
 */
      delayVal = 5;
      
/* Since the gain coming from each of the filters are not perfect, we need to adjust them digitally. Here, we experimented with the gain and arrived
 *  at the values you see here(These can be considered the "sensitivity" of the LEDs: 
 */

      threshold1 = round(potentiometerValue / 15);
      threshold2 = round(potentiometerValue / 30);
      threshold3 = round(potentiometerValue / 20);

/* This is the part of the code that controls how the LEDs blink. However, making the LEDs blink one predictable color is boring. Therefore, we added
 *  a bit of randomness centered around red, green, and blue to make it more interesting. The "range_lower" and "range_upper" values changes the range
 *  of random values, where a smaller difference of the two values correlates to less variation. Too much randomness will make the lights appear too
 *  chaotic, where not enough randomness will make the lights appear too predictable.
 */

      if (sensorValue0 > threshold1) { //40
        HSV_to_RGB_LED(random(range_lower_a, range_upper_a), 1, 1, 255, ledPin0a, ledPin0b, ledPin0c);
      }
      else {
        HSV_to_RGB_LED(0, 0, 0, 0, ledPin0a, ledPin0b, ledPin0c);
      }
      if (sensorValue1 > threshold2) { //20
        HSV_to_RGB_LED(random(range_lower_b, range_upper_b), 1, 1, 150, ledPin1a, ledPin1b, ledPin1c);
      }
      else {
        HSV_to_RGB_LED(0, 0, 0, 0, ledPin1a, ledPin1b, ledPin1c);
      }
      if (sensorValue2 > threshold3) { //30
        HSV_to_RGB_LED(random(range_lower_c, range_upper_c), 1, 1, 255, ledPin2a, ledPin2b, ledPin2c);
      }
      else {
        HSV_to_RGB_LED(0, 0, 0, 0, ledPin2a, ledPin2b, ledPin2c);
      }
    }
  }
/* The delay() function changes how quickly the program runs (how fast the void loop runs), which controls how fast the LEDs blink.
 */

  delay(delayVal); 
}

/* HSV FUNCTIONS BELOW HERE - DO NOT TOUCH CODE BELOW THIS LINE! 
 * This function does HSV to RGB conversion. It implements the algorithm shown in: https://en.wikipedia.org/wiki/HSL_and_HSV
 * Note that functions are not written inside the main void loop(). The function is merely a "prototype" or copy of some code that gets
 * run each time you call it inside the void loop(). You can intuitively think of a function as you might in math, where you have
 * some function f(x) that returns some value for an arbitrary input, x. In math, you might define f(x) only once -- for example,
 * f(x) = x + x. There after, you don't need to keep writing f(x) = x + x. To save space, you instead write f(1) = 2, f(2) = 4, etc.
 * This is a similar concept in programming where once you have the function defined, you can keep reusing it to save space.
*/

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
  analogWrite(pin0, floor(i * (r + m))), analogWrite(pin1, floor(i * (g + m))), analogWrite(pin2, floor(i * (b + m)));
}
