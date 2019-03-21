#pragma once
#include "IOT_board.h"



void setup() {
  Serial.begin(115200); // start serial port to arduino
  Serial.println("\n\n\n\n\n\n\n\n\n\n                                Arduino is Ready...\n\n\n\n");
  Serial3.begin(115200);
  pinMode(ESPPower, OUTPUT);
  pinMode(airSens, INPUT);  // assign pin for air quality sensor
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014
  Serial3.println("ATE0\r\n");
  Serial.println("                                Running...\n\n\n\n\n\n\n\n\n\n");
  digitalWrite(ESPPower, HIGH);
}

void loop() {
  messageHandler();
  showNewData();
  SMESP(); // call the esp8266 state machine.
  if (state_SMESP != state_prev_SMESP) { // check to see if the state has changed from last.
    Serial.print("State of machine is: ");
    Serial.println(state_SMESP);
    Serial.print("The time is now ");
    Serial.print(rtc.getTimeStr());
    Serial.print(" and the current temperature is ");
    Serial.print(rtc.getTemp());
    Serial.println(" Degrees Centergrade");
  }
  uTime = rtc.getUnixTime(rtc.getTime()); // grab the time from the RTC.
  fade.breathe(); // Fade led with a sine wave value.
}


void SMESP() { //State Machine ESP8266.
  state_prev_SMESP = state_SMESP;
  switch (state_SMESP) {

    case 0://reset
      state_SMESP = 1;
      break;


    case 1: // break power, and reset flags.
      iConnect = 0;
      iGotIP = 0;
      iReady = 0;
      Serial.println("Flags Lowered...");
      digitalWrite(ESPPower, LOW);
      delay(10);
      digitalWrite(ESPPower, HIGH);
      Serial3.print("ATE0\r\n");
      state_SMESP = 2;
      break;

    case 2: // start esp8266 and get a connection with an IP Address.

      TOcms2 = millis();
      if ((TOcms2 - TOpms2) >= TOInterv) { // 2 second watchdog
        TOpms2 = millis();
        state_SMESP = 0;
        break;
      }

      char * p;
      p = strstr (receivedChars, kwReady);
      Serial.println(kwReady);
      Serial.println(receivedChars);
      Serial.println(p);
      if (p) {
        iReady = 1;
        Serial.print("ESP 8266 indicates Ready Status: ");
        Serial.println(iReady);

        state_SMESP = 0;
        break;
      }
      else {
        iReady = 0;
        if (oldReady != iReady) {
          Serial.print("ESP 8266 not yet Ready - ");
          Serial.println(iReady);
          oldReady = iReady;
          break;
        }
      }

    case 3:
      // check for keyword disconnected or connected status.
      if (iConnect != oldConnected) {
        Serial.print("Connect Change Detected... ");
        state_SMESP = 4;

        if (iConnect == 0) {
          Serial.print("  Wifi Disconnected, Flag Lowered... Connection State is ");
          Serial.println(iConnect);
          oldConnected = iConnect;
          break;
        }
        else if (iConnect == 1) {
          Serial.print("  Wifi Connected, Flag Raised... Connection State is ");
          Serial.println(iConnect);
          oldConnected = iConnect;
          break;
        }
        else {
          Serial.print("  Wifi Uninitialised.... Connection State is ");
          Serial.println(iConnect);
          oldConnected = iConnect;
          break;
        }
      }

    case 4:
      if (strstr(receivedChars, kwConnect) > 0) {
        iConnect = 1;
      }
      if (strstr(receivedChars, kwDISC) > 0) {
        iConnect = 0;
      }

    // check for keyword got ip


    case 5:
      // if all flags are up then proceed....
      if (iConnect == 1 || iGotIP == 1) {
        state_SMESP = 6;
      }
      TOpms4 = millis(); //initialise the case 4 timeout

      break;



    case 6: // ask for the IP address and gateway.
      Serial.println("Sending CIPSTA? to the ESP8266...");
      Serial3.print("AT+CIPSTA?\r\n");
      state_SMESP = 7;
      break;

    case 7: //Show the IP Address and gateway.
      TOcms4 = millis();
      if ((TOcms4 - TOpms4) >= TOInterv) { //Eight seconds
        Serial.println("Timer Over-Run Timer Over-Run Timer Over-Run\nTimer Over-Run Timer Over-Run Timer Over-Run\nTimer Over-Run Timer Over-Run Timer Over-Run\nTimer Over-Run Timer Over-Run Timer Over-Run\nTimer Over-Run Timer Over-Run Timer Over-Run\n");
        TOpms4 = TOcms4;
        state_SMESP = 0;
        break;
      }

      if ( LocalIP != "" && Gateway != "") {
        Serial.print("request duration is: ");
        Serial.println(TOcms4 - TOpms4);
        state_SMESP = 8;
        break;
      }
      else {
        state_SMESP = 3;
        break;
      }

    case 8:
      Serial.print("Local IP Adress is: ");
      // serial print the ip address
      Serial.print("Local Gateway is: ");
      // serial print the gateway
      state_SMESP = 9;
      break;

    case 9:
      Serial.println("Setting mode of ESP8266. ");
      Serial3.print("AT+CWMODE=3\r\n\r\n");
      state_SMESP = 10;
      break;

    case 10:
      char * q;
      q = strstr (receivedChars, kwOK);
      if (q) {
        Serial.print("ESP 8266 returned OK. ");
        state_SMESP = 11;
        break;
      }
      else {
        if (oldOK == 1){
          break;
        }
        Serial.print("ESP 8266 did not return an OK. ");
        //this will neet tweaking or a timer
        oldOK =1;
        break;
      }
    case 11:
      break;
  }
}



char messageHandler() { // gets what is sitting on the serial port and parses it back to receivedChars array.
  static byte ndx = 0;  // this is a byte that saves the index of the array where the bytes will be stored.
  char endMarker = '\n'; // this is the char that the function is looking for to break.
  char rc; //this is the char variable that is allocated the actual char that is read before its passed to the array of 'recievedChars.
  if (Serial3.available() > 0) { //If there is something on the serial port,
    while (Serial3.available() > 0 && newData == false) {  // do this while there is,
      rc = Serial3.read(); // the recieved char is allocaed what is read off the serial port.

      if (rc != endMarker) {   // if the char that is read is not the end marker of \n, do what follows.
        receivedChars[ndx] = rc;  // This array at the given index number will be assigned what is in the received char variable.
        ndx++;    // the index is incremented.
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0';  //then add a line ending to the array,
        ndx = 0;    // set the index to zero for the next use,
        newData = true; // set the new data bool to true for the show new data function.
      }
    }
  }
}


void showNewData() { // function called next;
  if (newData == true) {  // If the new data function was set true, this will run.
    Serial.print("Showing New Data: ");  // display a message to the serial monitor,
    Serial.println(receivedChars); // display the chars to the serial monitor from the receieved chars array,
    newData = false; // sets the bool flag back to false for the next use.
  }
}







