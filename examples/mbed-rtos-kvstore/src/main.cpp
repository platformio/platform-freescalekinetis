/* Copyright (c) 2018 Arm Limited
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "mbed.h"
#include <stdio.h>
#include <string.h>
#include "KVStore.h"
#include "kvstore_global_api.h"

using namespace mbed;

#define EXAMPLE_KV_VALUE_LENGTH 64
#define EXAMPLE_KV_KEY_LENGTH 32
#define err_code(res) MBED_GET_ERROR_CODE(res)

void kv_store_global_api_example();

int main()
{
    /* KV Store Static API Example */
    kv_store_global_api_example();
}

void kv_store_global_api_example()
{
    char kv_value_in[EXAMPLE_KV_VALUE_LENGTH] = {"kvstore_dummy_value_hello_world"};
    char kv_key_in[EXAMPLE_KV_KEY_LENGTH] = {"/kv/dummy_key1"};
    char kv_key_out[EXAMPLE_KV_KEY_LENGTH] = {0};
    size_t actual_size = 0;

    /* key information container */
    kv_info_t info;

    /* kv store iterator */
    kv_iterator_t kvstore_it;

    int i_ind = 0;

    printf("--- Mbed OS KVStore static API example ---\n");

    int res = MBED_ERROR_NOT_READY;

    /* Start By Resetting the KV Storage */
    printf("kv_reset\n");
    res = kv_reset("/kv/");
    printf("kv_reset -> %d\n", err_code(res));

    /* Set First 'Dummy' Key/Value pair with unprotected clear value data */
    printf("kv_set first dummy key\n");
    res = kv_set(kv_key_in, kv_value_in, strlen(kv_value_in), 0);
    printf("kv_set -> %d\n", err_code(res));

    /* Read the KV Pair you've just set */
    /* Start by getting key's information */
    printf("kv_get_info of first key\n");
    res = kv_get_info(kv_key_in, &info);
    printf("kv_get_info -> %d\n", err_code(res));
    printf("kv_get_info key: %s\n", kv_key_in);
    printf("kv_get_info info - size: %u, flags: %lu\n", info.size, info.flags);

    /* Now that you know the data value size of this key,
     * allocate a buffer with matching size and get the value data */
    printf("kv_get first key\n");
    char *kv_first_value_out = new char[info.size + 1];
    memset(kv_first_value_out, 0, info.size + 1);
    res = kv_get(kv_key_in, kv_first_value_out, info.size, &actual_size);
    printf("kv_get -> %d\n", err_code(res));
    printf("kv_get key: %s\n", kv_key_in);
    printf("kv_get value: %s\n", kv_first_value_out);
    delete[] kv_first_value_out;

    /* Lets set some more 'Dummy' and 'Real' KV pairs */
    /* Set 'Dummy' Key2 */
    printf("kv_set second dummy key \n");
    res = kv_set("/kv/dummy_key2", "dummy_value2", strlen("dummy_value2"), 0);
    printf("kv_set -> %d\n", err_code(res));

    /* Set an authenticated-encrypted 'Dummy' key with Replay protection */
    printf("kv_set third key with Confidentiality and Replay Protection flags\n");
    res = kv_set("/kv/dummy_auth_enc_key", "auth_enc_value", strlen("auth_enc_value"),
                 KV_REQUIRE_CONFIDENTIALITY_FLAG | KV_REQUIRE_REPLAY_PROTECTION_FLAG);
    printf("kv_set -> %d\n", err_code(res));

    /* Set 2 non-dummy 'Real' KV pairs */
    /* Set 'Real' Key 1 */
    printf("kv_set Set 'Real' Key 1\n");
    res = kv_set("/kv/real_key1", "real_value1", strlen("real_value1"), 0);
    printf("kv_set -> %d\n", err_code(res));
    /* Set 'Real' Write-Once Key2 for a key that you do not want to be removed */
    printf("kv_set Set 'Real' Key 2 with flag write-once\n");
    res = kv_set("/kv/real_wo_key", "real_wo_value", strlen("real_wo_value"), KV_WRITE_ONCE_FLAG);
    printf("kv_set -> %d\n", err_code(res));

    /* Now lets remove all of the 'Dummy' Keys and remain with the 'Real' ones */
    printf("Removing 'Dummy' Keys\n");
    /* Iterate and remove Keys that start with prefix 'dummy' */
    res = kv_iterator_open(&kvstore_it, "dummy");
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    while (kv_iterator_next(kvstore_it, kv_key_out, EXAMPLE_KV_KEY_LENGTH) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
        printf("%d) Removing %s\n", i_ind, kv_key_out);
        kv_remove(kv_key_out);
        memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    }
    res = kv_iterator_close(kvstore_it);
    printf("Remaining with 'Real' Keys:\n");
    /* Iterate on all remaining Keys */
    res = kv_iterator_open(&kvstore_it, NULL);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    i_ind = 0;
    while (kv_iterator_next(kvstore_it, kv_key_out, EXAMPLE_KV_KEY_LENGTH) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
        printf("%d) %s\n", i_ind, kv_key_out);
        memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    }
    res = kv_iterator_close(kvstore_it);

    /* Try to remove write-once Key - should fail */
    printf("kv_remove write-once file - should fail!\n");
    res = kv_remove("/kv/real_wo_key");
    printf("kv_remove -> %d\n", err_code(res));

    /* Finally, reset will format kvstore and remove All Keys (including write-once keys) */
    printf("kv_reset format kvstore (including write-once)\n");
    res = kv_reset("/kv/");
    printf("kv_reset -> %d\n", err_code(res));

    return;
}
