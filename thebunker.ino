#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LED 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, LED, NEO_GRB + NEO_KHZ800);

#define BTTN_1 2
#define BTTN_2 3

//////MORSE SETUP///////
#define DOT 300 //length of dot in morse code
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
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BTTN_1, INPUT);
  pinMode(BTTN_2, INPUT);
  strip.begin();
  for(int i = 4; i < 24; i++){
    strip.setPixelColor(i, strip.Color(0,155,0));
  }
  strip.show();
}

void loop() {
  if(simulPress(HIGH)){
    Serial.println("ARMED");
    strip.setPixelColor(0, strip.Color(155,0,0));
    strip.setPixelColor(2, strip.Color(155,0,0));
    strip.show();
    if(simulPress(LOW)){
      Serial.println("LAUNCHED");
      strip.setPixelColor(1, strip.Color(155,0,0));
      strip.setPixelColor(3, strip.Color(155,0,0));
      strip.show();
      morseCode();
    }
  }
}

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

void morseCode(){
  while(true){
    Serial.println(MORSE_MSG);
    for(int i=0; i < (sizeof(MORSE_MSG)/sizeof(char))-1; i++)
      makeMorse(MORSE_MSG[i]);
    delay(3000);
  }
}

void makeMorse(char letter){
  if(letter == ' ')
    delay(DOT*4);
  else if(letter <= 57)
    blinkMorse(letter-48); //number
  else
     blinkMorse(letter-55); //letter
}

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

void lightsOn(){
  for(int i = 4; i < 24; i++)
    strip.setPixelColor(i, strip.Color(155,0,0));
  strip.show();
}

void lightsOff(){
  for(int i = 4; i < 24; i++)
    strip.setPixelColor(i, strip.Color(0,0,0));
  strip.show();
}

