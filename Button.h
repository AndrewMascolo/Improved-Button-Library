#ifndef Button_h
#define Button_h

/*
The MIT License (MIT)

Copyright (c) 2016 Andrew Mascolo Jr

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Add Double Press Feature

enum {WAITING = 0, PRESSED, HELD, RELEASED};

#define SECONDS 1000000
#define MILLISECONDS 1000
#define MICROSECONDS 1

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
//just in case someone is still using old versions
#include "WProgram.h"
#endif

class Button
{
  private:
    bool            State, lastState;
    unsigned long   onTime, holdTime, heldTime, DBTime, DBInterval, RO_Time;//, DubPTime;
    byte            _P;
    byte            ButtonState;
    void            (*F_on)();
	//void            (*F_Don)();
    void            (*F_hold)();
    void            (*F_off)();

  public:
    Button(byte Precision = LOW) : _P(Precision) { } // precision default = LOW or Milliseconds

    void SetStateAndTime(byte S = HIGH, unsigned long T = 500)
    {
      State = S; // Set the preferred state of the button. (Is the button HIGH or LOW when pressed?)
      lastState = !State; //lastState should be inverted from State
      holdTime = _P ? (T * 1000) : T; // Set the hold time in seconds
    }

    void SetDebounceTime(unsigned long T = 50)
    {
      DBInterval = _P ? (T * 1000) : T;
    }

    void onPressed(void (*ON)() )
    {
      F_on = ON;
    }
	
	/*void onD_Pressed(void (*DON)() )
    {
      F_Don = DON;
    }*/

    void onHold(void (*HOLD)() )
    {
      F_hold = HOLD;
    }

    void onReleased(void (*OFF)() )
    {
      F_off = OFF;
    }

    byte checkButton(byte _buttonPin )
    {
      byte _button;
      // Button has not been pressed yet
      ButtonState = WAITING;
      _button = digitalRead(_buttonPin);

      // First check to see if the button is pressed ie. State -> (LOW or HIGH)
      // and if it is different from it's lastState
      if (_button == State && lastState == !State)
      {
	    // record time
        DBTime = _P ? micros() : millis();
        onTime = DBTime;

        // Execute ON function
        if (*F_on) // if something was assigned to that function, do it
          F_on();
		
        // This prevents the code from entering this IF statement,
        // until lastState is set to the preferred State again.
        lastState = State;

        // Button was pressed
        ButtonState = PRESSED;
      }
	
      if ((_P ? micros() : millis()) - DBTime  >= DBInterval)
      {
        heldTime = 0;
        // button is still held down
        if (_button == State && lastState == State)
        {
          // Check for Rollover
          RO_Time = (_P ? micros() : millis()); // current time into RollOver variable
          if (RO_Time < onTime) // is the RollOver variable smaller than ontime?
            onTime = 0; // if yes,  reset ontime to zero

          // check to see if button is held down for X seconds
          if ( (heldTime = (RO_Time - onTime)) >= (holdTime - DBInterval)) // 1 second is 1,000 milliseconds or 1,000,000 microseconds
          {
            if (*F_hold) // if something was assigned to that function, do it
              F_hold(); // if button is held down for X seconds, execute the HOLD function

            ButtonState = HELD;
          }
        }
		
        if (_button == !State && lastState == State)//Button was released
        {
          if (*F_off) // if something was assigned to that function, do it
            F_off(); // otherwise execute the OFF function

          ButtonState = RELEASED;
          lastState = !State; // Update lastState
        }
      }
      return ButtonState;
    }

    // This returns the elapsed held time
    float GetHeldTime(float divisor = SECONDS)
    {
      if (divisor > 0)
        return heldTime / divisor;
      else
        return -1;
    }
};// End Of CLass

#endif