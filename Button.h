#ifndef Button_h
#define Button_h

#define WAITING 0
#define PRESSED 1
#define HELD 2
#define RELEASED 3

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
    unsigned long   onTime, holdTime, heldTime, DBTime, DBInterval, RO_Time;
    byte            _P;
    byte            ButtonState;
    void            (*F_on)();
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
	  static byte _button;
      // Button has not been pressed yet
      ButtonState = WAITING;
      _button = digitalRead(_buttonPin);
	    
      // First check to see if the button is pressed ie. State -> (LOW or HIGH)
      // and if it is different from it's lastState
      if (_button == State && lastState == !State)
      {
	    DBTime = _P? micros() : millis();
        // record time
        onTime = _P ? micros() : millis();

        // Execute ON function
        if (*F_on) // if something was assigned to that function, do it
          F_on();

        // This prevents the code from entering this IF statement,
        // until lastState is set to the preferred State again.
        lastState = State;

        // Button was pressed
        ButtonState = PRESSED;
      }
	  
      if((_P ? micros() : millis()) - DBTime  >= DBInterval)
	  {
	    heldTime = 0;
        // button is still held down
        if (_button == State && lastState == State)
        {
		  // Check for Rollover	  		  
		  RO_Time = (_P ? micros() : millis()); // current time into RollOver variable
		  if(RO_Time < onTime) // is the RollOver variable smaller than ontime?
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
	  if(divisor > 0)
	    return heldTime / divisor;
      else 
	    return -1;
	}
};// End Of CLass

#endif