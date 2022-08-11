#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// Hardware pins

#define HW_LDR_SENSOR         A1
#define HW_GARDEN_LIGHT       3
#define HW_HEATER             5
#define HW_COOLER             4
#define HW_RIGHT_BTN          8
#define HW_LEFT_BTN           7
#define HW_TEMPERATURE_SENSOR A0
#define HW_INLET_VALVE  0x00
#define HW_OUTLET_VALVE 0x01
#define HW_WATER_VOLUME 0x30
#define HW_UPDATE_INTERVAL 100

// Virtual pins

#define VT_TEMPERATURE_GAUGE  V0
#define VT_COOLER_SW          V1
#define VT_HEATER_SW          V2
#define VT_WATER_VOL_GAUGE    V3
#define VT_INLET_SW           V4
#define VT_OUTLET_SW          V5
#define VT_TERMINAL           V6

// Value ranges

#define TEMPERATURE_MAX 35
#define WATER_VOLUME_MAX 3000
#define WATER_VOLUME_MIN 2000

#define ON  1
#define OFF 0

#endif