// small sensor, positive to 'S', negative to middle, and signal to ground pin. 
// attach the signal to the A0 pin, or provide it with a pin as an argument to run.
// works on a timer not a delay, it can be changed by setting the first peram below. 
// call tempReadToSerial to print to serial0, or call tempRead to return the temp

#pragma once
#include "Arduino.h"
#include "Temp.h"

  const long intervalTemp1 = 200; // interval in milliseconds between Reading sensor
  const long intervalTemp2 = 200; // interval in milliseconds between Reading sensor

  int RawADC;
  int _tempPin = A0;
  unsigned long PMTemp1;   // previous millis
  unsigned long PMTemp2;   // previous millis
  double cTemp;
  int buttonpin = A0; // define the digital temperature sensor interface
  double val; // define numeric variables val

//____________________________________________________________________

Temp::Temp() : _tempPin(A0)
{
  pinMode(buttonpin, INPUT); // define digital temperature sensor output interface
  pinMode( _tempPin , INPUT);   // initialise temp pin
}

Temp::Temp(int tempPin) : _tempPin(tempPin)
{
  pinMode(buttonpin, INPUT); // define digital temperature sensor output interface
  pinMode( _tempPin , INPUT);   // initialise temp pin
}


//____________________________________________________________________




void Temp::tempReadToSerial(){ // Member function in CPP file

  val = analogRead(buttonpin); // digital interface to read val
  cTemp = RThermistor(val);
  Serial.print(" temp in C is....  ");
  Serial.println(cTemp);

                                                    
}

double Temp::tempRead(){ // Member function in CPP file
      val = analogRead(buttonpin); // digital interface to read val
    cTemp = RThermistor(val);
      return cTemp;                                               
}

double Temp::RThermistor(int RawADC) {
  double _Temp;
_Temp = log(((10240000/RawADC) - 10000));
_Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * _Temp * _Temp ))* _Temp );
_Temp = _Temp - 273.15;// Convert Kelvin to Celcius
return _Temp;
}
