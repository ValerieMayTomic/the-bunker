#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LED 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, LED, NEO_GRB + NEO_KHZ800);

//paired key button init
#define BTTN_1 2
#define BTTN_2 3

//////MORSE SETUP///////
//length of dot in morse code - use this to adjust speed!
//All other characters/spaces based on the length of a dot
#define DOT 300
const char MORSE_MSG[ ] = "RECALL CODE 9286";
const char* const MORSE_LETTERS[] = {"-----",
                                ".----",
                                "..---",
                                "...--",
                                "....-",
                                ".....",
                                "-....",
                                "--...",
                                "---..",
                                "----.",
                                ".-   ",
                                "-... ",
                                "-.-. ",
                                "-..  ",
                                ".    ",
                                "..-. ",
                                "--.  ",
                                ".... ",
                                "..   ",
                                ".--- ",
                                "-.-  ",
                                ".-.. ",
                                "--   ",
                                "-.   ",
                                "---  ",
                                ".--. ",
                                "--.- ",
                                ".-.  ",
                                "...  ",
                                "-    ",
                                "..-  ",
                                "...- ",
                                ".--  ",
                                "-..- ",
                                "-.-- ",
                                "--.. "};
void setup() {
  Serial.begin(9600);
  pinMode(BTTN_1, INPUT);
  pinMode(BTTN_2, INPUT);
  strip.begin();
  //start all LEDs green
  for(int i = 4; i < 24; i++){
    strip.setPixelColor(i, strip.Color(0,155,0));
  }
  strip.show();
}

void loop() {
  if(simulPress(HIGH)){
    //ARMED
    strip.setPixelColor(0, strip.Color(155,0,0));
    strip.setPixelColor(2, strip.Color(155,0,0));
    strip.show();
    if(simulPress(LOW)){
      //LAUNCHED
      strip.setPixelColor(1, strip.Color(155,0,0));
      strip.setPixelColor(3, strip.Color(155,0,0));
      strip.show();
      morseCode();
    }
  }
}

/*
 * A function that detects whether both buttons are pressed (LOW)
 * or have their key turned (HIGH) at the same time
 */
boolean simulPress(boolean high_low){
  while(digitalRead(BTTN_1) != high_low && digitalRead(BTTN_2) != high_low);
  unsigned long init_time = millis();
  while((millis() - init_time) < 1500){
    if(digitalRead(BTTN_1) == high_low && digitalRead(BTTN_2) == high_low)
      return(true);
  }
  while(digitalRead(BTTN_1) == high_low || digitalRead(BTTN_2) == high_low);
  return(simulPress(high_low));
}

/*
 * A function which continuously loops over the MORSE_MSG
 * global constant, calling the appropriate functions
 * to make it repeatedly blink in morse
 */
void morseCode(){
  while(true){
    Serial.println(MORSE_MSG);
    for(int i=0; i < (sizeof(MORSE_MSG)/sizeof(char))-1; i++)
      makeMorse(MORSE_MSG[i]);
    delay(3000);
  }
}

 /* A function which processes individual letters from the
  *  MORSE_MSG global, determining which values in the
  *  MORSE_LETTERS conversion table to access, and delaying
  *  or calling the blinkMorse function as appropriate
  */
void makeMorse(char letter){
  if(letter == ' ')
    delay(DOT*4);
  else if(letter <= 57)
    blinkMorse(letter-48); //number
  else
     blinkMorse(letter-55); //letter
}

/*
 * A function which processes strings of
 * dots and dashes from the MORSE_LETTERS
 * conversion array, blinking and dleaying
 * as appropriate
 */
void blinkMorse(int morseNum){
  Serial.println(MORSE_LETTERS[morseNum]);
  for(int i=0; i < 5; i++){
    if(MORSE_LETTERS[morseNum][i] == '-'){
      lightsOn();
      delay(DOT*3);
      lightsOff();
      delay(DOT);
    }
    else if(MORSE_LETTERS[morseNum][i] == '.'){
      lightsOn();
      delay(DOT);
      lightsOff();
      delay(DOT);
    }
  }
  delay(DOT*3);
}

//turns all lights on red
void lightsOn(){
  for(int i = 4; i < 24; i++)
    strip.setPixelColor(i, strip.Color(155,0,0));
  strip.show();
}

//turns all lights off
void lightsOff(){
  for(int i = 4; i < 24; i++)
    strip.setPixelColor(i, strip.Color(0,0,0));
  strip.show();
}

