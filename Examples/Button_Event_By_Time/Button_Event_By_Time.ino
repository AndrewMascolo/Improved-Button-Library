#include<Button.h>

#define buttonPin 2

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
  byte myButton = B.checkButton(digitalRead(buttonPin));

  if (myButton) // if myButton is anything but 0, it is true
  {
    unsigned long timer = B.GetHeldTime(SECONDS);
    switch (timer)
    {
      case 0: Serial.println("Quick press"); break;
      case 6 ... 10: Serial.println("Longer than 5 seconds"); break;
      default: Serial.print(timer); Serial.println(" Seconds"); break;
    }
  }
}
