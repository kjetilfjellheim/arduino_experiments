// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Senser_H_
#define _Sensert_H_
#include "Arduino.h"
//add your includes for the project SensePlant here


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
void sensorChange();
void motionDetected();
void setTemperature();
void setHumidity();
void setSoilHumidity();
void setPressure();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project SensePlant here


//Do not add code below this line
#endif /* _Senser_H_ */
