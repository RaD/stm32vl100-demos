#include "ch.h"
#include "hal.h"
#include "leds.h"

#ifndef MAILBOX_SIZE
#define MAILBOX_SIZE    4
#endif

Mailbox mb;
msg_t buffers[MAILBOX_SIZE];

#define MSG_BUTTON_PRESSED  1

/**
 * This program make the following tasks:
 *  1) set the timer to blink green LED every second;
 *  2) set ISR on button, send a message to mailbox on press;
 *  3) separate thread waits a message and blink blue LED on every fetch.
 */

// BLINKER THREAD DEFINITION: BEGIN
// Simple thread for green LED blinkng.
static WORKING_AREA(waBlinker, 128);
static msg_t blinker(void *arg) {
  (void) arg;

  chRegSetThreadName("blinker");

  while (TRUE) {
    blink_led(GREEN_LED, 500);
  }
  return 0;
}
// BLINKER THREAD DEFINITION: END


// ONPRESS THREAD DEFINITION: BEGIN
static WORKING_AREA(waOnPress, 128);
static msg_t onpress(void *arg) {
  (void) arg;
  msg_t message;

  chRegSetThreadName("onpress");

  while(TRUE) {
    chMBFetch(&mb, &message, TIME_INFINITE);
    if (message == MSG_BUTTON_PRESSED) {
      blink_led(BLUE_LED, 500); // on
      blink_led(BLUE_LED, 100); // off
      blink_led(BLUE_LED, 100); // on
      blink_led(BLUE_LED, 100); // off
      blink_led(BLUE_LED, 100); // on
      blink_led(BLUE_LED, 100); // off
      blink_led(BLUE_LED, 500); // on
      blink_led(BLUE_LED, 100); // off
    }
  }
  return 0;
}
// ONPRESS THREAD DEFINITION: END


// BUTTON HANDLER DEFINITION: BEGIN
static void button_handler(EXTDriver *extp, expchannel_t channel)
{
    (void) extp;
    (void) channel;
    chSysLockFromIsr();
    chMBPostI(&mb, MSG_BUTTON_PRESSED);
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
// BUTTON HANDLER DEFINITION: END

// Program entry point.
int main(void) {

  // System initialization.
  halInit();
  chSysInit();
  chMBInit(&mb, buffers, MAILBOX_SIZE);

  // Start green LED blinking thread.
  (void)chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);

  // Start blue LED onpress thread.
  (void)chThdCreateStatic(waOnPress, sizeof(waOnPress), NORMALPRIO, onpress, NULL);

  // Activate first EXT driver.
  extStart(&EXTD1, &extcfg);

  // Do nothing in main thread.
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
