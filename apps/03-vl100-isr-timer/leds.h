#include "ch.h"
#include "hal.h"

#define GREEN_LED   GPIOC_LED3
#define BLUE_LED    GPIOC_LED4

// Turns on the blue LED.
static void blue_led_on(void *arg) {
  (void)arg;
  palSetPad(GPIOC, BLUE_LED);
}

// Turns off the blue LED.
static void blue_led_off(void *arg) {
  (void)arg;
  palClearPad(GPIOC, BLUE_LED);
}

// Turns on the green LED.
static void green_led_on(void *arg) {
  (void)arg;
  palSetPad(GPIOC, GREEN_LED);
}

// Turns off the green LED.
static void green_led_off(void *arg) {
  (void)arg;
  palClearPad(GPIOC, GREEN_LED);
}
