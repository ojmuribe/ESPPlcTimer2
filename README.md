# EspPlcTimers2
## An easier way to use ESPPLCTimers
## V1.0 ©2023 Juan M. Uribe

---
This is an improvement of the ESPPlcTimer library.

---
### USAGE

First, include the library in the project:

    #include "EspPlcTimers2.h"

Declare timer objects as global instances:

    espPlcTimer t1(TON, 20000); //TON timer, 20s
    espPlcTimer t2(TOFF); // TOFF timer, 10s

   
Then add the TIMERHANDLER macro in the setup()

    void setup() {
    
      TIMERHANDLER; //Call to TimerHandler function
    
      Serial.begin(115200);
      pinMode(GPIO_NUM_13, INPUT_PULLUP); //input for the TON timer (green)
      pinMode(GPIO_NUM_12, INPUT_PULLUP); //input for the TOFF timer (blue)
      pinMode(LED_BUILTIN, OUTPUT); //output for the TON timer
      pinMode(GPIO_NUM_15, OUTPUT); //output for the TOFF timer
    };

And, use it!

    void loop() {
      t1.tin(!digitalRead(GPIO_NUM_13));
      digitalWrite(LED_BUILTIN, t1.q());
      t2.tin(!digitalRead(GPIO_NUM_12));
      digitalWrite(GPIO_NUM_15, t2.q());
      if (t1.tin() || t1.q()) {
        Serial.print("T1: ");
        Serial.println(t1.et());
      }
      if (t2.tin() || t2.q()) {
        Serial.print("T2: ");
        Serial.println(t2.et());
      }
    };

---
## What is different?

EspPlcTimers.h implements only the timers routine and not the auxiliary functions.
The reason for doing this is because the auxiliary functions do not require the use of the FREE RTOS.

Therefore I have decided to keep them in a separate .h file so that you can use them on any type of Arduino and not just on an ESP32.

Here there is only one timer object class:

    espPlcTimer
        
When declaring an instance, you must specify the type of timer to be used: TON or TOFF

Optionally, the timer time can be preloaded, although it can later be altered using the preset method.
If no preload time is indicated, by default the timer takes a time of 10s

    espPlcTimer t1(TON, 20000); //TON timer, 20s
    espPlcTimer t2(TOFF); // TOFF timer, 10s

The other main difference is the abstraction of the use of update functions.

You do not need to add the declared timers to the update function, as is done in ESPPLCTimers.
This is done automatically and implicitly when an instance of an espPlcTimer object is declared.

In fact, when declaring the instance, the created object is added to a list of timers. The update routine takes that list and runs through it exhaustively updating each timer included in it.

The update routine runs in a parallel task declared by the TIMERHANDLER macro

EspPlcTimers2 uses the Luis Llamas Arduino List library (https://www.luisllamas.es/libreria-arduino-list/) that must be added to the project (https://github.com/luisllamasbinaburo/Arduino-List). (Thanks @luisllamasbinaburo)


