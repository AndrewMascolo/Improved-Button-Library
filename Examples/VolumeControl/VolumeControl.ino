#include<Button.h>

#define buttonPin 2

Button B(HIGH); // LOW = milliseconds, HIGH = microseconds, default is LOW
byte volumeLevel = 0;
bool lock = false;
//========================================================================================
void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  // Set what the button will be when pressed (default is HIGH)
  // and set the hold time (default is 500)
  B.SetStateAndTime(LOW, 1000);
  B.SetDebounceTime(50);
}

void loop()
{
  byte myButton = B.checkButton(buttonPin);

  if (myButton) // if myButton is anything but 0, it is true
  {
    switch (myButton)
    {
      case PRESSED:
        //Serial.println("Button was Pressed ");
        if (volumeLevel < 5) ++volumeLevel;
        break;

      case HELD:
        //Serial.println("Buttons is Held:");
        if ((uint32_t(B.GetHeldTime(MILLISECONDS)) % 1000) == 0)
        {
          if (!lock && volumeLevel > 0)
          {
            --volumeLevel;
            Serial.println(volumeLevel);
            lock = true;
          }
        }
        else lock = false;
        break;

      case RELEASED:
        //Serial.println("Button was Released ");
        Serial.println(volumeLevel);
        break;
      default: break;
    }
  }
}

