#include "ch.h"
#include "hal.h"

#define GREEN_LED   GPIOC_LED3
#define BLUE_LED    GPIOC_LED4

// Changes a state of appropriate LED.
static void blink_led(int led, int delay) {
    palTogglePad(GPIOC, led);
    chThdSleepMilliseconds(delay);
}

// Simple thread for green LED blinkng.
static WORKING_AREA(waBlinker, 128);
static msg_t blinker(void *arg) {
  (void) arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    blink_led(GREEN_LED, 500);
  }
}

static VirtualTimer vt;
static void blue_led_on(void *arg) { (void)arg; palSetPad(GPIOC, BLUE_LED); }
static void blue_led_off(void *arg) { (void)arg; palClearPad(GPIOC, BLUE_LED); }


static void button_handler(EXTDriver *extp, expchannel_t channel)
{
    (void) extp;
    (void) channel;
    blue_led_on(NULL);
    chSysLockFromIsr();
    if (!chVTIsArmedI(&vt))
      chVTSetI(&vt, MS2ST(200), blue_led_off, NULL);
    chSysUnlockFromIsr();
}

// Channel configuration.
static const EXTConfig extcfg = {
  {
   {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, button_handler}, /* 0 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 1 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 2 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 3 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 4 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 5 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 6 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 7 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 8 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 9 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 10 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 11 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 12 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 13 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 14 */
   {EXT_CH_MODE_DISABLED, NULL}, /* 15 */
  }
};

// Program entry point.
int main(void) {

  // System initialization.
  halInit();
  chSysInit();

  // Activate first EXT driver.
  extStart(&EXTD1, &extcfg);

  // Start green LED blinking thread.
  (void)chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);

  // Start main thread, which is wating for button press and blinks blue LED 
  // after a second.
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
