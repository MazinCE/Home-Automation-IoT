#include "UI.h"

UI uiInstance;

void UI_Init(LiquidCrystal_I2C *lcd20x4)
{
    uiInstance.lcd20x4 = lcd20x4;
    uiInstance.lcd20x4->init();

    uiInstance.currentPageIndex = 0;
    uiInstance.lastClearTime = 0;
    uiInstance.uiDirty = 1;

    uiInstance.pages[HOME_PAGE] = {
        .icon = HOUSE_ICON,
        .label = "Mazin's House",
        .draw = &UI_HomePage,
    };

    uiInstance.pages[TEMPERATURE_PAGE] = {
        .icon = THERMOEMTER_ICON,
        .label = "Temperature",
        .draw = &UI_TemperaturePage,
    };

    uiInstance.pages[WATER_TANK_PAGE] = {
        .icon = TANK_ICON,
        .label = "Water Tank",
        .draw = &UI_WaterTankPage,
    };

    uiInstance.pages[GARDEN_LIGHT_PAGE] = {
        .icon = BULB_ICON,
        .label = "Garden Light",
        .draw = &UI_GardenLightPage,
    };
}

void UI_Draw(void)
{
    unsigned long currentTime = millis();

    if (uiInstance.uiDirty && currentTime - uiInstance.lastClearTime >= UI_CLEAR_INTERVAL)
    {
        uiInstance.lcd20x4->clear();
        uiInstance.lastClearTime = currentTime;
    }

    if (uiInstance.uiDirty && currentTime == uiInstance.lastClearTime)
    {
        Page *currentPage = &(uiInstance.pages[uiInstance.currentPageIndex]);
        currentPage->draw();
        uiInstance.uiDirty = 0;
    }
}

void UI_DrawIcon(const char *icon, const char *label)
{
    uiInstance.lcd20x4->setCursor(3, 0);
    uiInstance.lcd20x4->print(label);

    for (int i = 0; i < 4; ++i)
    {
        uiInstance.lcd20x4->createChar(i + 1, icon + i * 8);
        uiInstance.lcd20x4->setCursor(ICON_LOCATION[i][0], ICON_LOCATION[i][1]);
        uiInstance.lcd20x4->write(i + 1);
    }
}

void UI_SetParam(UI_Parameter param, int val)
{
    uiInstance.uiParameters[param] = val;
    uiInstance.uiDirty = 1;
}

void UI_NextPage(void)
{
    uiInstance.currentPageIndex = (uiInstance.currentPageIndex + 1) % PAGE_COUNT;
    uiInstance.uiDirty = 1;
}

void UI_PrevPage(void)
{
    if (uiInstance.currentPageIndex == 0)
    {
        uiInstance.currentPageIndex = PAGE_COUNT - 1;
    }
    else
    {
        uiInstance.currentPageIndex--;
    }

    uiInstance.uiDirty = 1;
}

void UI_HomePage()
{
    Page *home = &uiInstance.pages[HOME_PAGE];
    UI_DrawIcon(home->icon, home->label);
    uiInstance.lcd20x4->setCursor(6, 2);
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_CONNECTION_STATUS] ? "Online" : "Offline");
}

void UI_TemperaturePage()
{
    Page *temperature = &uiInstance.pages[TEMPERATURE_PAGE];
    UI_DrawIcon(temperature->icon, temperature->label);
    uiInstance.lcd20x4->setCursor(5, 1);
    uiInstance.lcd20x4->print("Temp  : ");
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_TEMPERATURE]);
    uiInstance.lcd20x4->print((char)223); // ° Symbol
    uiInstance.lcd20x4->print("C");
    uiInstance.lcd20x4->setCursor(5, 2);
    uiInstance.lcd20x4->print("Heater: ");
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_HEATER_STATUS] ? "ON" : "OFF");
    uiInstance.lcd20x4->setCursor(5, 3);
    uiInstance.lcd20x4->print("Cooler: ");
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_COOLER_STATUS] ? "ON" : "OFF");
}
void UI_WaterTankPage()
{
    Page *waterTank = &uiInstance.pages[WATER_TANK_PAGE];
    UI_DrawIcon(waterTank->icon, waterTank->label);
    uiInstance.lcd20x4->setCursor(5, 1);
    uiInstance.lcd20x4->print("Volume: ");
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_WATER_VOLUME]);
    uiInstance.lcd20x4->print("L");
    uiInstance.lcd20x4->setCursor(5, 2);
    uiInstance.lcd20x4->print("Inlet : ");
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_INLET_STATUS] ? "ON" : "OFF");
    uiInstance.lcd20x4->setCursor(5, 3);
    uiInstance.lcd20x4->print("Outlet: ");
    uiInstance.lcd20x4->print(uiInstance.uiParameters[UI_PARAM_OUTLET_STATUS] ? "ON" : "OFF");
}

void UI_GardenLightPage()
{
    Page *gardenLight = &uiInstance.pages[GARDEN_LIGHT_PAGE];
    UI_DrawIcon(gardenLight->icon, gardenLight->label);
    uiInstance.lcd20x4->setCursor(4, 2);
    uiInstance.lcd20x4->print("Brightness: ");
    uiInstance.lcd20x4->print((int)(uiInstance.uiParameters[UI_PARAM_LIGHT_BRIGHTNESS] / 255.0f * 100.0f));
    uiInstance.lcd20x4->print("%");
}
