#include "ch.h"
#include "hal.h"

#define GREEN_LED   GPIOC_LED3
#define BLUE_LED    GPIOC_LED4

static void blink_led(int led, int delay);

// Changes a state of appropriate LED.
static void blink_led(int led, int delay) {
    palTogglePad(GPIOC, led);
    chThdSleepMilliseconds(delay);
}
