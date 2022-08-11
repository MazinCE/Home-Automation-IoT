#define BLYNK_TEMPLATE_ID "TMPLlrbBDC0j"
#define BLYNK_DEVICE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "GU_3qRw-JkpZJKHWeAnui0RE3ra-P_Ci"

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HardwareManager.h"
#include "UI.h"

LiquidCrystal_I2C lcd20x4 = LiquidCrystal_I2C(0x27, 20, 4);

void ProcessEvents(void);

void setup()
{
    HM_Init();
    UI_Init(&lcd20x4);
    Blynk.begin(BLYNK_AUTH_TOKEN);
}

void loop()
{
    ProcessEvents();
    HM_Update();
    UI_Draw();
    Blynk.run();
}

void ProcessEvents(void)
{
    Event e;

    while (EventQueue_Poll(&e))
    {
        switch (e.type)
        {
        case EVNT_UPDATE_TEMPERATURE:
            Blynk.virtualWrite(VT_TEMPERATURE_GAUGE, e.int_payload);
            UI_SetParam(UI_PARAM_TEMPERATURE, e.int_payload);
            break;
        case EVNT_UPDATE_LIGHT_BRIGHTNESS:
            HM_SetLightBrightness(e.int_payload);
            UI_SetParam(UI_PARAM_LIGHT_BRIGHTNESS, e.int_payload);
            break;
        case EVNT_UPDATE_WATER_VOLUME:
            Blynk.virtualWrite(VT_WATER_VOL_GAUGE, e.int_payload);
            UI_SetParam(UI_PARAM_WATER_VOLUME, e.int_payload);
            break;
        case EVNT_SET_COOLER:
            HM_SetCooler(e.int_payload);
            Blynk.virtualWrite(VT_COOLER_SW, e.int_payload);
            UI_SetParam(UI_PARAM_COOLER_STATUS, e.int_payload);
            break;
        case EVNT_SET_HEATER:
            HM_SetHeater(e.int_payload);
            Blynk.virtualWrite(VT_HEATER_SW, e.int_payload);
            UI_SetParam(UI_PARAM_HEATER_STATUS, e.int_payload);
            break;
        case EVNT_SET_TANK_INLET:
            HM_SetTankValve(HW_INLET_VALVE, e.int_payload);
            Blynk.virtualWrite(VT_INLET_SW, e.int_payload);
            UI_SetParam(UI_PARAM_INLET_STATUS, e.int_payload);
            break;
        case EVNT_SET_TANK_OUTLET:
            HM_SetTankValve(HW_OUTLET_VALVE, e.int_payload);
            Blynk.virtualWrite(VT_OUTLET_SW, e.int_payload);
            UI_SetParam(UI_PARAM_OUTLET_STATUS, e.int_payload);
            break;
        case EVNT_RIGHT_BTN_PRESSED:
            UI_NextPage();
            break;
        case EVNT_LEFT_BTN_PRESSED:
            UI_PrevPage();
            break;
        case EVNT_NOTIFY:
            Blynk.virtualWrite(VT_TERMINAL, e.str_payload);
            break;
        default:
            break;
        }
    }
}

BLYNK_CONNECTED()
{
    Blynk.syncAll();
    UI_SetParam(UI_PARAM_CONNECTION_STATUS, 1);
}

BLYNK_DISCONNECTED()
{
    UI_SetParam(UI_PARAM_CONNECTION_STATUS, 0);
}

BLYNK_WRITE(VT_OUTLET_SW)
{
    HM_SendEvent(EVNT_SET_TANK_OUTLET, param.asInt());
}

BLYNK_WRITE(VT_INLET_SW)
{
    HM_SendEvent(EVNT_SET_TANK_INLET, param.asInt());
}

BLYNK_WRITE(VT_COOLER_SW)
{
    HM_SendEvent(EVNT_SET_COOLER, param.asInt());
}

BLYNK_WRITE(VT_HEATER_SW)
{
    HM_SendEvent(EVNT_SET_HEATER, param.asInt());
}