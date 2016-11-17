#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <AnalogTouch.h>
const int timerPin = 2;     // the number of the pushbutton pin
const int ledPin =  4;      // the number of the LED pin
const int touchPin = 7;
const int speakerPin = 9;
const int speakerGroundPin = 8;



// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

//For AnalogTouch
#define pinAnalog A0
#define offset 2
#if offset > 6
#error "Too big offset value"
#endif

#include "pitches.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin);

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  // iterate over the notes of the melody:
  //MELODY LOW, hardware specific
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(timerPin, INPUT);
  digitalWrite(timerPin, HIGH);//activate pull up
  pinMode(speakerGroundPin, OUTPUT);
  digitalWrite(speakerGroundPin, LOW);
  pixels.begin(); 
  
  pixels.setBrightness(50);
  updateColor(0, 255, 0, 1);
  
}

bool readTouch(){
 uint16_t value = analogTouchRead(pinAnalog);
  //value = analogTouchRead(pinAnalog, 100);

  // Self calibrate
  static uint16_t ref = 0xFFFF;
  if (value < (ref >> offset))
    ref = (value << offset);
  // Cool down
  else if (value > (ref >> offset))
    ref++;
  delay(20);
  return (value - (ref >> offset)) > 40;
}

void updateColor(int r, int g, int b, int value){
  for(int i=0;i<value;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
  }
  pixels.show(); 
}

void playMelody(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(9, melody[thisNote], noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(9);
    }
}

void loop() {
  if(readTouch()){
    playMelody();
  }
}
