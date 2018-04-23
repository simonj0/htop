/*
htop - linux/TemperatureFan.c
(C) 2018 Simon J.
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "TemperatureFan.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

static void TemperatureFan_getData(int *temperature, int *rpm) {
    FILE *p;
    p = popen("sensors", "r");
    if(!p) return;

    ssize_t read;
    size_t len;
    char *line = NULL;
    char *entry = NULL;
//    int temperatureTotal = 0, temperatureCount = 0;
    int temperature_ = 0;
    int _rpm = 0;
    while((read = getline(&line, &len, p)) != -1) {
        //if((entry = strstr(line, "Core "))) {
        if((entry = strstr(line, "Package "))) {
            // temperature
            char *tstart = strchr(entry, '+');
            if(!tstart) continue;

            // read to long
            long val = strtol(tstart, NULL, 10);
            if(val == LONG_MAX || val == LONG_MIN) continue;

            // add temperature to total
            //temperatureTotal += (int)val;
            //temperatureCount++;
            temperature_ = (int)val;
        } else if((entry = strstr(line, "fan"))) {
            // fan
            char *rstart = entry;
            while(*rstart != ' ' && *rstart != '\n') rstart++;

            // read to long
            long val = strtol(rstart, NULL, 10);
            if(val == LONG_MAX || val == LONG_MIN) continue;

            _rpm = (int)val;
        }
    }

    free(line);
    pclose(p);

    //*temperature = temperatureCount > 0 ? (temperatureTotal / temperatureCount) : 0;
    *temperature = temperature_;
    *rpm = _rpm;
}

static time_t TemperatureFan_cacheTime = 0;
static int TemperatureFan_cacheTemperature = 0;
static int TemperatureFan_cacheRPM = 0;

void TemperatureFan_getTemperature(int *temperature) {
    time_t now = time(NULL);
    // refresh if data is 3 seconds old
    if(now > TemperatureFan_cacheTime + 3) {
        TemperatureFan_getData(&TemperatureFan_cacheTemperature, &TemperatureFan_cacheRPM);
        TemperatureFan_cacheTime = now;
    }

    *temperature = TemperatureFan_cacheTemperature;
}

void TemperatureFan_getRPM(int *rpm) {
    time_t now = time(NULL);
    // refresh if data is 3 seconds old
    if(now > TemperatureFan_cacheTime + 3) {
        TemperatureFan_getData(&TemperatureFan_cacheTemperature, &TemperatureFan_cacheRPM);
        TemperatureFan_cacheTime = now;
    }

    *rpm = TemperatureFan_cacheRPM;
}
