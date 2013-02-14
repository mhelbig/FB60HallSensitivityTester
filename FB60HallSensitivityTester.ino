// Libraries:
#include <LiquidCrystal.h>

// Constants:
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);                   // LCD I/O assignments

// Global Variables:
int ButtonState = 0;                             // On/Off button state
int LastButtonState = 0;                         // Previous On/Off button state
int Mode = 0;                                    // On/Off state of the motor
int HallEffectSensorRotations = 0;               // Counter for Hall Effect Sensor Pulses
int SensorCounts;                                // Holder for measured sensor counts
int ShaftSensorState;                            // Holder for shaft sensor used for edge triggering
int LastShaftSensorState;                        // Holder for the previous state

// I/O Ports:
const int ShaftSensor = 2;
const int HallEffectSensor = 3;
const int Motor = 4;
const int LED = 13;
const int PushButton = 12;

void setup()
{                
  // I/O pin setup:
  pinMode(ShaftSensor, INPUT);
  pinMode(HallEffectSensor, INPUT);
  pinMode(Motor, OUTPUT);     
  pinMode(LED, OUTPUT);     
  pinMode(PushButton, INPUT);
  digitalWrite(PushButton, HIGH);               // Enable the built-in pullup resistor on the pushbutton
  
  // LCD setup
  lcd.begin(16, 2);                             // set up the LCD's number of columns and rows

  // Interrupt Setup:
  attachInterrupt(1,AccumulateHallEffectSensorRotations,RISING);
}

void loop()
{
  // Check for someone pressing the On/Off button:
  ButtonState = digitalRead(PushButton);
  
  if(ButtonState ==0 && LastButtonState ==1)
  {
    if (Mode ==0)
    {
      SetMotor(HIGH);
      Mode = 1;
    }
    else
    {
      SetMotor(LOW);
      Mode = 0;
    }
  }
  LastButtonState = ButtonState;

  ShaftSensorState = digitalRead(ShaftSensor);  // Grab the current state of the shaft sensor

  if(ShaftSensorState == 1 && LastShaftSensorState == 0)  // The shaft sensor just went high - it's an edge!
  {  
    SensorCounts = HallEffectSensorRotations;     // Save the current count
    HallEffectSensorRotations = 0;                // Reset the sensor counts for the next rotation
  
    lcd.setCursor(8,0);                           // Set cursor to position on first row
    lcd.print(SensorCounts);                      // Print out the counts
    lcd.print("  ");                              // Add some white space to the end to clear off leftover digits
  }
  
  LastShaftSensorState = ShaftSensorState;        // Update the last state for the edge sensing comparison
}

void AccumulateHallEffectSensorRotations(void)    // ISR function that adds up sensor counts
{
  HallEffectSensorRotations++;
}

void SetLED(bool State)
{
  if(State == HIGH) digitalWrite(LED, HIGH);
  else digitalWrite(LED,LOW);
}

void SetMotor(bool State)
{
  if(State == HIGH) digitalWrite(Motor, HIGH);
  else digitalWrite(Motor, LOW);
}
