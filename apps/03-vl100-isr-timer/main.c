#include "ch.h"
#include "hal.h"
#include "leds.h"

/**
 * This program make the following tasks:
 *  1) set the timer to blink green LED every second;
 *  2) set ISR on button, blink blue LED on release.
 */

#define freq_step 5000
#define initial_freq 10000

static int current_freq = initial_freq;

// Virtual timers for the LEDs.
static VirtualTimer vt_green;
static VirtualTimer vt_blue;

// Timer interrupt handler for green LED.
static void gpt1cb(GPTDriver *gptp) {
  (void) gptp;
  green_led_on(NULL);
  chSysLockFromIsr();
  if (!chVTIsArmedI(&vt_green))
    chVTSetI(&vt_green, MS2ST(200), green_led_off, NULL);
  chSysUnlockFromIsr();
}

// Green LED's timer configuration (10 kHz).
static const GPTConfig gpt1cfg = {initial_freq, gpt1cb};

// Release buttom interrupt handler.
static void button_handler(EXTDriver *extp, expchannel_t channel)
{
  CH_IRQ_PROLOGUE();
  (void) extp;
  (void) channel;
  blue_led_on(NULL);

  //gptStopTimer(&GPTD1);
  current_freq += freq_step;
  if (20000 == current_freq) current_freq = initial_freq;
  //gptStartContinuous(&GPTD1, current_freq);

  chSysLockFromIsr();
  if (!chVTIsArmedI(&vt_blue))
    chVTSetI(&vt_blue, MS2ST(200), blue_led_off, NULL);
  chSysUnlockFromIsr();
  CH_IRQ_EPILOGUE();
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

  // Activate first timer.
  gptStart(&GPTD1, &gpt1cfg);
  // Start the timer.
  gptStartContinuous(&GPTD1, initial_freq);

  // Do nothing in main thread.
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
