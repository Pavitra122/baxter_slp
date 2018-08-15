// Open syringe pump - Use a stepper motor to push fluid from a syringe

#include <Arduino.h>
#include <Keyboard.h>
#include <LiquidCrystal.h>
#include <string.h>
#include "motor.h"
#include "ui.h"


/* -- Defines -- */
#define DEFAULT_MTR_LOOP_MICROS (10000) // 10ms
#define HEARTBEAT_LOOP_MICROS   (500000) // 500ms
#define UI_LOOP_MICROS          (UI_PROCESS_DELAY_MS * 1000)
#define LOOP_DELAY_MICROS       (10) // 10us


/* -- Variables -- */
static uint32_t lastMicros = 0;                     // Storage for previous main loop time
static int32_t hbMicros = HEARTBEAT_LOOP_MICROS;    // Counter for heartbeat timing
static int32_t uiMicros = UI_LOOP_MICROS;           // Counter for UI timing
static int32_t mtrMicros = DEFAULT_MTR_LOOP_MICROS; // Counter for motor timing


/* -- Functions -- */

//*******************************************************************************************
// Perform one-time setup at power up
//*******************************************************************************************
void setup()
{
  Serial.begin(57600);            // Initialize the serial port.  Note that your serial connection must be set to 57600 to work!
  motorSetup();                   // Perform setup needed for the motor
  uiSetup();                      // Perform setup needed for the User Interface

  lastMicros = micros();
  PORTB |= 1 << PINB5;            // Use LED on PB5 as program heartbeat
}


//*******************************************************************************************
// Main program loop
//*******************************************************************************************
void loop()
{
  // Get the time and calculate change since last time through main loop
  uint32_t now = micros();
  uint32_t delta = (now > lastMicros) ? now - lastMicros : (0xFFFFFFFF - lastMicros) + now;
  lastMicros = now;

  // Handle motor processing
  mtrMicros -= delta;
  if (mtrMicros <= 0)
  {
    mtrMicros = motorProcess();  // Process motor control
  }

  // Handle UI processing
  uiMicros -= delta;
  if (uiMicros <= 0)
  {
    uiProcess();                    // Process key presses and display updates
    uiMicros = UI_LOOP_MICROS;
  }

  // Handle heartbeat timing
  hbMicros -= delta;
  if (hbMicros <= 0)
  {
    PORTB ^= 1 << PINB5;            // Toggle the heartbeat LED
    hbMicros = HEARTBEAT_LOOP_MICROS;
  }

  // Do main loop delay
  delayMicroseconds(LOOP_DELAY_MICROS);
}

