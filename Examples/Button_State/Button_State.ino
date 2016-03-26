#include<Button.h>

#define buttonPin A0

Button B(HIGH); // LOW = milliseconds, HIGH = microseconds, default is LOW
//========================================================================================
//
//========================================================================================

void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  // Set what the button will be when pressed (default is HIGH)
  // and set the hold time (default is 500)
  B.SetStateAndTime(LOW, 1000);
}

void loop()
{
  byte myButton = B.checkButton(buttonPin);
  
  if (myButton) // if myButton is anything but 0, it is true
  {
    switch (myButton)
    {
      case PRESSED:
        Serial.println("Button was Pressed ");
        break;
      case HELD:
        Serial.println("Buttons is Held:");
        break;
      case RELEASED:
        Serial.println("Button was Released ");
        break;
      default: break;
    }
  }
}
