#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include "EventQueue.h"
#include "HardwareConfig.h"
#include <Blynk.h>

typedef enum
{
    ACT_TANK_INLET = 0,
    ACT_COOLER,
    NO_ACTUATORS
} Actuator;

typedef enum
{
    SEN_TEMPERATURE = 0,
    SEN_WATER_VOLUME,
    SEN_LIGHT_BRIGHTNESS,
    NO_SENSORS
} Sensor;

typedef struct
{
    int sensors[NO_SENSORS];
    int actuators[NO_ACTUATORS];
    unsigned long lastUpdateTime;
} HardwareManager;

void HM_Init(void);
void HM_Update(void);

void HM_SetHeater(int val);
void HM_SetCooler(int val);
void HM_SetTankValve(int tankValve, int val);
void HM_SetLightBrightness(int val);
void HM_CheckButtonPressed(void);

void HM_UpdateTemperature(void);
void HM_UpdateWaterTank(void);
void HM_UpdateGardenLight(void);

void HM_Notify(const char *msg);
void HM_SendEvent(EventType type, int payload);

#endif