#include "ch.h"
#include "hal.h"

#define GREEN_LED   GPIOC_LED3
#define BLUE_LED    GPIOC_LED4

// Changes a state of appropriate LED.
void blink_led(int led, int delay) {
    palTogglePad(GPIOC, led);
    chThdSleepMilliseconds(delay);
}

// Simple thread for green LED blinkng.
static WORKING_AREA(waBlinker, 128);
static msg_t blinker(void *arg) {
  chRegSetThreadName("blinker");
  while (TRUE) {
    blink_led(GREEN_LED, 500);
  }
}

// Program entry point.
int main(void) {

  // System initialization.
  halInit();
  chSysInit();

  // Start green LED blinking thread.
  (void)chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);

  // Start main thread, which is wating for button press and blinks blue LED 
  // after a second.
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON)) {
      blink_led(BLUE_LED, 500);
      blink_led(BLUE_LED, 500);
    }
    chThdSleepMilliseconds(500);
  }
}
