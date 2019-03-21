#pragma once

class Temp{
  public:
  double cTemp;
  double RThermistor(int RawADC);
  int RawADC;
  int _tempPin;
  void tempReadToSerial(); // member function in the .h
  double tempRead(); // member function in the .h
  Temp(int tempPin); // Constructor in the .h
  Temp(); // Constructor in the .h

};


