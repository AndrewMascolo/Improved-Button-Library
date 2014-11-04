#include<Button.h>

#define buttonPin 2
#define ledPin 13

unsigned long blinkTime = millis();
Button B(HIGH); // LOW = milliseconds, HIGH = microseconds, default is LOW
//========================================================================================
//
//========================================================================================
void LED_ON()
{
  digitalWrite(ledPin, HIGH);
}

void LED_OFF()
{
  digitalWrite(ledPin, LOW);
}

void BLINK()
{
  if (millis() - blinkTime >= 100)
  {
    blinkTime += 100;
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}
//========================================================================================

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Set what the button will be when pressed (default is HIGH)
  // and set the hold time (default is 500)
  B.SetStateAndTime(LOW, 1000);

  B.onPressed(LED_ON);
  B.onHold(BLINK);
  B.onReleased(LED_OFF);
}

void loop()
{
  byte myButton = B.checkButton(buttonPin);
  
  if (myButton) // if myButton is anything but 0, it is true
  {
    switch (myButton)
    {
      case PRESSED:
        Serial.print("Button was Pressed ");
        break;
      case HELD:
        Serial.print("Buttons is Held:");
        break;
      case RELEASED:
        Serial.print("Button was Released ");
        break;
      default: break;
    }
    Serial.println(B.GetHeldTime(SECONDS));
  }
}

//void loop()
//{
//  byte myButton = B.checkButton(digitalRead(buttonPin));
//
//  if (myButton) // if myButton is anything but 0, it is true
//  {
//    unsigned long timer = B.GetHeldTime(SECONDS);
//    switch (timer)
//    {
//      case 0: Serial.println("Quick press"); break;
//      case 6 ... 10: Serial.println("Longer than 5 seconds"); break;
//      default: Serial.print(timer); Serial.println(" Seconds"); break;
//    }
//  }
//}
