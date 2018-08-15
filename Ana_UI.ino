//
// motor.cpp
// Code file for motor control portion of syringe pump project
//

#include "motor.h"
#include "ui.h"


//*******************************************************************************************
// Comment out the following line to use the real motor hardware!!!
//*******************************************************************************************
//#define MOTOR_SIMULATOR



//*******************************************************************************************
// Simulated motor software (for testing UI software)

#ifdef MOTOR_SIMULATOR

static bool infusing = false;
static uint32_t targetVolumeTenths = 0;
static uint32_t infusedVolumeTenths = 0;

void motorSetup(void) {}

uint32_t motorProcess(void)
{
  if (infusing)
  {
    infusedVolumeTenths++;
    infusing = (infusedVolumeTenths >= targetVolumeTenths) ? false : true;
    uiDeliveryStatus(infusing, infusedVolumeTenths);
  }

  return 1000000; // 1 second
}

int motorStart(int16_t /*rate*/, uint8_t volume)
{
  targetVolumeTenths = (uint32_t) volume * 10;
  infusedVolumeTenths = 0;
  infusing = true;
  return 0;
}

void motorStop(void)
{
  if (infusing)
  {
    infusing = false;
    uiDeliveryStatus(infusing, infusedVolumeTenths);
  }
}

// End of simulated motor software (for testing UI software)
//*******************************************************************************************




//*******************************************************************************************
// Real motor software (for running actual motor)

#else // else !MOTOR_SIMULATOR

static bool infusing = false;
static uint32_t targetVolumeTenths = 0;
static uint32_t infusedVolumeTenths = 0;

/* -- Defines -- */

#define SYRINGE_VOLUME_ML          (30.0f)
#define SYRINGE_BARREL_LENGTH_MM   (80.0f)
#define THREADED_ROD_PITCH         (1.25f)
#define STEPS_PER_REVOLUTION       (200.0f)
#define MICROSTEPS_PER_STEP        (16.0f)


/* -- Constants -- */
const uint32_t DEFAULT_MOTOR_LOOP_DELAY = 10000UL;    // = 10000 usec (10 msec)


/* -- Pin definitions -- */
int motorDirPin   = 2;
int motorStepPin  = 3;


/* -- Variables -- */
MotorState  mMotorState;              // Motor state (stopped, running, etc.)
int16_t     mRate;                    // Infusion rate (in mL/hr)
uint16_t    mVolumeToBeInfused;       // Volume to be infused (in tenths of mL)
uint16_t    mVolumeInfused;           // Volume infused so far (in tenths of mL)
uint32_t    mMotorLoopDelay;          // Delay (in usec) until next motor update is needed


/* -- Functions -- */
//*******************************************************************************************
// Perform one-time motor setup at power up
//*******************************************************************************************

void motorSetup(void)
{
    // Setup motor hardware pins
    pinMode(motorDirPin,OUTPUT);
    pinMode(motorStepPin,OUTPUT);


    // Initialize motor variables
    mMotorState = STOPPED;   // Changed it from stop
    mMotorLoopDelay = DEFAULT_MOTOR_LOOP_DELAY;

}


//*******************************************************************************************
// Perform motor processing needed for every main loop cycle
// Returns: delay (in usec) until next motor update needed
//*******************************************************************************************
int highorLow = 1;
float volPerMicroStep = ( 1/ MICROSTEPS_PER_STEP)  * (1.0/ STEPS_PER_REVOLUTION) * (THREADED_ROD_PITCH) * (SYRINGE_VOLUME_ML / SYRINGE_BARREL_LENGTH_MM);
float volumeInfused_temp = 0;
//int microSteps = 0;
uint32_t motorProcess (void)
{
    // Check if motor is running

         // If running:
    if (mMotorState == RUNNING)
    {
         // Pulse the motor (toggle the motor step pin)
      if (highorLow == 1)
        digitalWrite(motorStepPin,HIGH);
      else
        digitalWrite(motorStepPin,LOW);
      highorLow = !highorLow;

      // Update number of microsteps infused (motor steps once per high/low transition on motor step pin)
      //microSteps++;  
                 
      // Update volume infused
      volumeInfused_temp += volPerMicroStep;
      Serial.println(volumeInfused_temp);
      Serial.println("\n");
      //uiDeliveryStatus()
      // See if it is time to stop
      if(volumeInfused_temp > mVolumeToBeInfused)
      {
          Serial.println("Trying to stop");
          motorStop();
      }
      Serial.print("mMotorLoopDelay:  ");
      Serial.println(mMotorLoopDelay);
      return mMotorLoopDelay;
      // Update the UI with the volume infused
    }
    return mMotorLoopDelay;
    // Return the delay until next motor update is needed
    
    //return 50;
}


//*******************************************************************************************
// Start the motor
// Parameters:
//   rate   (input): rate to run motor at (in mL/hr), positive = forward, negative = reverse
//                     range: +/- 1 mL/hr to +/- 1,000 mL/hr
//   volume (input): amount of fluid to deliver (in mL)
//                     range: 1 mL to 30 mL
// Returns: 0 if motor was started successfully, -1 if failure
//*******************************************************************************************
int motorStart (int16_t rate, uint8_t volume)
{
    
    Serial.println("Rate:");
    Serial.println(rate);
    int retVal = 0;

    // If motor is stopped: 
    if(mMotorState == STOPPED)
      {
        // Perform range checking on the rate and volume
        if(1 <= abs(rate) && abs(rate) <= 1000 && 1 <= volume && volume <= 30){

        Serial.println("working");

          
            // Calculate loop delay that will give the correct infusion rate
            mMotorLoopDelay = (1.0/(float)rate) * (SYRINGE_VOLUME_ML / SYRINGE_BARREL_LENGTH_MM) * (THREADED_ROD_PITCH) * (1.0/ STEPS_PER_REVOLUTION) * ( 1/ MICROSTEPS_PER_STEP) * (3600000000.0/2.0);
            Serial.println("delay: ");
            Serial.println(mMotorLoopDelay);
            Serial.println("\n");
            
            Serial.println("volPerMicroStep: ");
            Serial.println(volPerMicroStep);
            Serial.println("\n");
            // Reset the volume infused
            mVolumeInfused = 0;
            volumeInfused_temp =0;
            // Set motor direction
            if(rate > 0)
              digitalWrite(motorDirPin,HIGH);
            else
              digitalWrite(motorDirPin,LOW);
  
           //Start motor
           mMotorState = RUNNING;
           infusedVolumeTenths++;
           infusing = (infusedVolumeTenths >= targetVolumeTenths) ? false : true;
           mRate = rate;
           mVolumeToBeInfused = volume;
        }
        else
          return -1;
      }

    // Return value indicating if motor was started successfully
    return retVal;
}


//*******************************************************************************************
// Stop the motor
//*******************************************************************************************
void motorStop (void)
{
    // Add code here
    // Stop the motor
    mMotorState = STOPPED;
}

#endif // MOTOR_SIMULATOR



