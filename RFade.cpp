#pragma once;
#include "Arduino.h"
#include "RFade.h"
  
  int _ledPin = 13;
  int ledBrightness = 10;   	// how bright the LED is
  int fadeAmount = 3;    	// how many points to fade the LED by
  unsigned long PMFade;  	// previous millis
  const long intervalFade = 20;	// interval between adjustments

// Useful to avoid LED values outside the bounds [0;255]
int checkValue(int val) {
  if (val > 255)
    val = 255;
  else if(val < 0)
    val = 0;
  return val;
}

// Time period of fading in millisecs
#define PERIOD 5000 
// Angular Frequency by definition
#define OMEGA 2*PI/PERIOD
// No Phase
#define PHASE 0
// Offset of the sine wave
#define OFFSET 130
// Amplitude of the sine wave
#define AMPLITUDE 127
 
// Used to generate time for the cos wave
unsigned long timer = 0;


//____________________________________________________________________

Fade::Fade() : _ledPin(13)
{
  pinMode( _ledPin , OUTPUT);   // initialise led pin for fade
}

Fade::Fade(int ledPin) : _ledPin(ledPin)
{
  pinMode( _ledPin , OUTPUT);   // initialise led pin for fade
}


//____________________________________________________________________


void Fade::breathe(){ // Member function in CPP file
   timer = millis(); // updating time 
 int ledValue = ledValue = OFFSET + AMPLITUDE*(cos(OMEGA*timer)+PHASE);
 analogWrite(_ledPin, checkValue(ledValue));
}

 
 
