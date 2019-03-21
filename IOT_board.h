

#pragma once
#include "IOT_board_global.h"
#include <RFade.h>
#include <Arduino.h>
#include <stdio.h>
#include <Temp.h>
#include <DS3231.h>



//constructors:
Fade fade; // Fade led 13 class constructor. takes an optional pin number.
Temp temp;
DS3231 rtc(SDA, SCL);
