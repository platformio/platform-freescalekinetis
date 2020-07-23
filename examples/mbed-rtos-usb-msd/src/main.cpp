/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "USBMSD.h"
#include "FATFileSystem.h"

#define DEFAULT_BLOCK_SIZE  512
#define HEAP_BLOCK_DEVICE_SIZE (128 * DEFAULT_BLOCK_SIZE)

FATFileSystem heap_fs("heap_fs");
HeapBlockDevice bd(HEAP_BLOCK_DEVICE_SIZE, DEFAULT_BLOCK_SIZE);

int main()
{
    bd.init();

    FATFileSystem::format(&bd);

    int err = heap_fs.mount(&bd);

    if (err) {
        printf("%s filesystem mount failed\ntry to reformat device... \r\n", heap_fs.getName());
        err = heap_fs.reformat(&bd);
    }

    // If still error, then report failure
    if (err) {
        printf("Error: Unable to format/mount the device.\r\n");
        while (1);
    }

    USBMSD usb(&bd);

    while (true) {
        usb.process();
    }

    return 0;
}