/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "platform/mbed_thread.h"
#include "psa/client.h"

#define BLINKING_RATE_MS 1000

int main(void)
{
    uint32_t count = 0;

    DigitalOut led(LED1);

    printf("PSA Framework Version : %ld\r\n", psa_framework_version());

    while (true)
    {
        led = !led;

        thread_sleep_for(BLINKING_RATE_MS);

        // Print only after (BLINKING_RATE_MS * 10) milliseconds.
        if ((count++ % 10) == 0)
        {
            printf("Executing from NS Core... LED Blink rate : %d ms\r\n", BLINKING_RATE_MS);
        }
    }
}
