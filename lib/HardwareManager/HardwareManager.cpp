#include "HardwareManager.h"
#include <Arduino.h>

HardwareManager hmInstance;

void HM_Init(void)
{
    pinMode(HW_GARDEN_LIGHT, OUTPUT);
    pinMode(HW_HEATER, OUTPUT);
    pinMode(HW_COOLER, OUTPUT);
    pinMode(HW_RIGHT_BTN, INPUT);
    pinMode(HW_LEFT_BTN, INPUT);

    // Setup Serial for water tank
    Serial.begin(19200);
}

void HM_Update(void)
{
    unsigned long currentTime = millis();

    if (currentTime - hmInstance.lastUpdateTime >= HW_UPDATE_INTERVAL)
    {
        HM_CheckButtonPressed();
        HM_UpdateTemperature();
        HM_UpdateWaterTank();
        HM_UpdateGardenLight();

        hmInstance.lastUpdateTime = currentTime;
    }
}

void HM_SetHeater(int val)
{
    digitalWrite(HW_HEATER, val);
}

void HM_SetCooler(int val)
{
    digitalWrite(HW_COOLER, val);
    hmInstance.actuators[ACT_COOLER] = val;
}

void HM_SetTankValve(int tankValve, int val)
{
    int status = val ? ON : OFF;
    Serial.write(tankValve);
    Serial.write(status);

    if (tankValve == HW_INLET_VALVE)
    {
        hmInstance.actuators[tankValve] = status;
    }
}

void HM_SetLightBrightness(int val)
{
    analogWrite(HW_GARDEN_LIGHT, val);
}

void HM_CheckButtonPressed(void)
{
    Event e;
    e.type = EVNT_NULL;

    if (digitalRead(HW_RIGHT_BTN))
    {
        e.type = EVNT_RIGHT_BTN_PRESSED;
    }
    else if (digitalRead(HW_LEFT_BTN))
    {
        e.type = EVNT_LEFT_BTN_PRESSED;
    }

    if (e.type != EVNT_NULL)
    {
        EventQueue_Push(e);
    }
}

void HM_UpdateTemperature(void)
{
    int adc_val = analogRead(HW_TEMPERATURE_SENSOR);
    int temperature = (adc_val * 4.88f) / 10;

    if (temperature != hmInstance.sensors[SEN_TEMPERATURE])
    {
        HM_SendEvent(EVNT_UPDATE_TEMPERATURE, temperature);

        if (temperature >= TEMPERATURE_MAX)
        {
            HM_SendEvent(EVNT_SET_HEATER, OFF);
            HM_Notify("Heater off: Temperature exceeded 35°C");
        }

        hmInstance.sensors[SEN_TEMPERATURE] = temperature;
    }
}

void HM_UpdateWaterTank(void)
{
    Serial.write(HW_WATER_VOLUME);
    int hiByte = Serial.read();
    int loByte = Serial.read();

    int waterVolume = (hiByte << 8) | loByte;

    if (waterVolume != hmInstance.sensors[SEN_WATER_VOLUME] || waterVolume == 0)
    {
        HM_SendEvent(EVNT_UPDATE_WATER_VOLUME, waterVolume);

        if (waterVolume >= WATER_VOLUME_MAX)
        {
            HM_Notify("Inlet off: Tank is full");
            hmInstance.actuators[ACT_TANK_INLET] = OFF;
            HM_SendEvent(EVNT_SET_TANK_INLET, OFF);
        }
        else if (waterVolume <= WATER_VOLUME_MIN)
        {
            if (hmInstance.actuators[ACT_TANK_INLET] == OFF)
            {
                HM_Notify("Inlet on: Water volume less than 2000L");
                HM_SendEvent(EVNT_SET_TANK_INLET, ON);
            }
        }

        hmInstance.sensors[SEN_WATER_VOLUME] = waterVolume;
    }
}

void HM_UpdateGardenLight(void)
{
    int adc_val = analogRead(HW_LDR_SENSOR);
    int brightness = map(adc_val, 0, 1023, 0, 255);

    if (brightness != hmInstance.sensors[SEN_LIGHT_BRIGHTNESS])
    {
        HM_SendEvent(EVNT_UPDATE_LIGHT_BRIGHTNESS, brightness);
        hmInstance.sensors[SEN_LIGHT_BRIGHTNESS] = brightness;
    }
}

void HM_Notify(const char *msg)
{
    Event e;
    e.type = EVNT_NOTIFY;
    e.str_payload = msg;

    EventQueue_Push(e);
}

void HM_SendEvent(EventType type, int payload)
{
    Event e = {
        .type = type,
        .int_payload = payload,
    };

    EventQueue_Push(e);
}
