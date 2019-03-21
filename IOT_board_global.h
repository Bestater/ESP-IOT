#pragma once

// Global variables:

int state_SMESP = 0;
int state_prev_SMESP = 0;

//for the timer pulled from unix time on the RTC
unsigned long uTime;



// time out previous millis for case 2.
unsigned long TOpms2 = 2;
unsigned long TOcms2;
unsigned long TOpms4 = 2;
unsigned long TOcms4;
unsigned long TOInterv = 2000;

//the air sensor:
int airQual;
float airSens = A1;

//the temperature sensor:
float tempVal;

//grabbing the serial data:
int ESPPower = 2;
char kwBusy = "busy p...";
char kwOK = "OK";
char kwDISC = "WIFI DISCONECT";
char kwConnect = "WIFI CONNECTED";
char kwGotIP = "WIFI GOT IP";
char kwError = "ERROR";
char kwIP = "+CIPSTA:ip:";
char kwGW = "+CIPSTA:gateway:";
char kwReady = "ready";
char LocalIP = "";
char Gateway = "";

String ESPSSID = "";
String ESPPassword = "";

// FLAGS
int iConnect = -1;
int oldConnected = -1;
int iGotIP = -1;
int iReady = -1;
int oldReady = 20;
int oldOK = -1;


char charBuf[3000];

// for the message handler:
const byte numChars = 3000; // so the complier knows how many chars the array size needs to be.
char receivedChars[numChars]; // an array to store the received data.
boolean newData = false;
char rc2;
