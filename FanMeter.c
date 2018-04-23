/*
htop - FanMeter.c
(C) 2018 Simon J.
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "FanMeter.h"

#include "TemperatureFan.h"
#include "CRT.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*{
#include "Meter.h"
}*/

int FanMeter_attributes[] = {
    FANSPEED
};

static void FanMeter_updateValues(Meter* this, char* buffer, int len) {
    int rpm;
    TemperatureFan_getRPM(&rpm);

    this->values[0] = rpm;
    xSnprintf(buffer, len, "%d RPM", rpm);
}

MeterClass FanMeter_class = {
    .super = {
        .extends = Class(Meter),
//        .display = FanMeter_display,
        .delete = Meter_delete,
    },
    .updateValues = FanMeter_updateValues,
    .defaultMode = TEXT_METERMODE,
    .maxItems = 1,
    .total = 8000.0,
    .attributes = FanMeter_attributes,
    .name = "FanSpeed",
    .uiName = "Fan Speed",
    .caption = "Fan Speed: "
};
