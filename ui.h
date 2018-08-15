//
// ui.h
// Header file for User Interface (UI) portion of syringe pump project
//

#ifndef UI_H
#define UI_H

/* -- Includes -- */
#include <Arduino.h>
#include <stdint.h>

#define UI_PROCESS_DELAY_MS   (20) // uiProcess is called every 20ms

/* UI functions */
void     uiSetup    (void);                             // Perform setup needed for the UI
void     uiProcess  (void);                             // Perform processing needed for the UI every main loop cycle
void     uiDeliveryStatus (bool moving, uint32_t tenthsMLDelivered);

#endif  // ifndef UI_H
