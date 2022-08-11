#ifndef EVENT_H
#define EVENT_H

typedef enum
{
    EVNT_NULL = 0,
    EVNT_UPDATE_TEMPERATURE,
    EVNT_UPDATE_LIGHT_BRIGHTNESS,
    EVNT_UPDATE_WATER_VOLUME,
    EVNT_SET_COOLER,
    EVNT_SET_HEATER,
    EVNT_SET_TANK_OUTLET,
    EVNT_SET_TANK_INLET,
    EVNT_RIGHT_BTN_PRESSED,
    EVNT_LEFT_BTN_PRESSED,
    EVNT_NOTIFY,
} EventType;

typedef struct
{
    EventType type;
    int int_payload;
    const char *str_payload;
} Event;

#endif
