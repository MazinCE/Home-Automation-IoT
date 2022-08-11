#ifndef UI_H
#define UI_H

#include "LiquidCrystal_I2C.h"
#include "Icons.h"

#define PAGE_COUNT 4
#define UI_CLEAR_INTERVAL 500

typedef enum
{
    UI_PARAM_TEMPERATURE = 0,
    UI_PARAM_WATER_VOLUME,
    UI_PARAM_HEATER_STATUS,
    UI_PARAM_COOLER_STATUS,
    UI_PARAM_INLET_STATUS,
    UI_PARAM_OUTLET_STATUS,
    UI_PARAM_LIGHT_BRIGHTNESS,
    UI_PARAM_CONNECTION_STATUS,
    UI_PARAMTERS_MAX,
} UI_Parameter;

typedef enum 
{
    HOME_PAGE = 0,
    TEMPERATURE_PAGE,
    WATER_TANK_PAGE,
    GARDEN_LIGHT_PAGE,
}PageType;

typedef void (*DrawPageFn)(void);

typedef struct
{
    const char *icon;
    const char *label;
    DrawPageFn draw;
}Page;

typedef struct
{
    int currentPageIndex;
    Page pages[PAGE_COUNT];
    LiquidCrystal_I2C *lcd20x4;
    int uiParameters[UI_PARAMTERS_MAX];
    int uiDirty;
    unsigned long lastClearTime;
} UI;

void UI_Init(LiquidCrystal_I2C *lcd20x4);
void UI_Draw(void);
void UI_DrawIcon(const char *icon, const char *label);

void UI_SetParam(UI_Parameter param, int val);

void UI_NextPage(void);
void UI_PrevPage(void);

void UI_HomePage();
void UI_TemperaturePage();
void UI_WaterTankPage();
void UI_GardenLightPage();

#endif