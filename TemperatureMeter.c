/*
htop - TemperatureMeter.c
(C) 2013 Ralf Stemmer
(C) 2014 Blair Bonnett
(C) 2018 Simon J.
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "TemperatureMeter.h"

#include "TemperatureFan.h"
#include "CRT.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*{
#include "Meter.h"
}*/

int TemperatureMeter_attributes[] = {
    TEMPERATURE_COOL,
    TEMPERATURE_MEDIUM,
    TEMPERATURE_HOT,
};

static void TemperatureMeter_updateValues(Meter* this, char* buffer, int len) {
    int temperature;
    TemperatureFan_getTemperature(&temperature);

    this->values[0] = temperature;
    xSnprintf(buffer, len, "%d°C", temperature);
}

static void TemperatureMeter_display(Object* cast, RichString* out) {
    Meter *this = (Meter *)cast;
    int temperature = (int)(this->values[0]);

    int color;
    if(temperature < 50)        color = CRT_colors[TEMPERATURE_COOL];
    else if(temperature < 70)   color = CRT_colors[TEMPERATURE_MEDIUM];
    else                        color = CRT_colors[TEMPERATURE_HOT];

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d°C", temperature);
    RichString_append(out, color, buffer);
}

MeterClass TemperatureMeter_class = {
    .super = {
        .extends = Class(Meter),
        .display = TemperatureMeter_display,
        .delete = Meter_delete,
    },
    .updateValues = TemperatureMeter_updateValues,
    .defaultMode = TEXT_METERMODE,
    .maxItems = 1,
    .total = 100.0,
    .attributes = TemperatureMeter_attributes,
    .name = "Temperature",
    .uiName = "Temperature",
    .caption = "Temperature: "
};
