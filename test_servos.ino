/*
  Steffen Camarato
  Nicolas Jaramillo
*/

#include <Servo.h>

// Declare left and right servos
Servo servoLeft;
Servo servoRight;

void setup() // Built-in initialization block
{
  tone(4, 3000, 1000); // Play tone for 1 second
  delay(1000); // Delay to finish tone
  int irLeft = 1; // Declare IR variables and
  int irRight = 1; // initialize both to 1

  servoLeft.attach(13); // Attach left signal to pin 13
  servoRight.attach(12); // Attach right signal to pin 12

  while ((irLeft == 1) && (irRight == 1)) // Wait for detection
  {
    irLeft = irDetect(9, 10, 38000); // Check for object on left
    irRight = irDetect(2, 3, 38000); // Check for object on right
  }
}

void loop() // Main loop auto-repeats
{

  int irLeft = irDetect(9, 10, 38000); // Check for object on left
  int irRight = irDetect(2, 3, 38000); // Check for object on right

  if ((irLeft == 0) && (irRight == 0)) // If both sides detect
  {
    maneuver(200, 200, 20); // Forward 20 ms, chase object
  }
  else if (irLeft == 0) // If only left side detects
  {
    maneuver(-200, 200, 20); // Left toward object 20 ms
  }
  else if (irRight == 0) // If only right side detects
  {
    maneuver(200, -200, 20); // Right toward object 20 ms
  }
  else // Otherwise, no IR detects
  {
    maneuver(150, -150, 20); // Rotate 360 so we dont run into walls
  }
}

// IR Object Detection Function
int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8); // IRLED 38 kHz at least 1 ms
  delay(1); // Wait 1 ms
  int ir = digitalRead(irReceiverPin); // IR receiver -> ir variable
  delay(1); // Down time before recheck
  return ir; // Return 1 no detect, 0 detect
}

void maneuver(int speedLeft, int speedRight, int msTime)
{
  // speedLeft, speedRight ranges: Backward Linear Stop Linear Forward
  // -200 -100....0.....100 200
  servoLeft.writeMicroseconds(1500 + speedLeft); // Left servo speed
  servoRight.writeMicroseconds(1500 - speedRight); // Right servo speed
  if (msTime == -1) // If msTime = -1
  {
    servoLeft.detach(); // Stop servo signals
    servoRight.detach();
  }
  delay(msTime); // Delay for msTime
}
