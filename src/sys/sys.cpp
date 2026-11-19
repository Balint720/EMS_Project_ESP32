#include "sys/sys.h"
#include <sys/timer.h>
#include <hal/dac.h>

void SYS_Setup() {
    pinMode(PIN_FGENOUTPUT_LED, OUTPUT);
    SYS_SetupTimers();
}