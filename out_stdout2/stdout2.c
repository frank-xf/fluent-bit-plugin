/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2019      The Fluent Bit Authors
 *  Copyright (C) 2015-2018 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_output.h>
#include <fluent-bit/flb_pack.h>

#include <stdio.h>
#include <pulsar/c/client.h>

static int cb_stdout_init(struct flb_output_instance *ins,
                          struct flb_config *config, void *data)
{
    (void) ins;
    (void) config;
    (void) data;

    pulsar_client_configuration_t *conf = pulsar_client_configuration_create();
    pulsar_client_configuration_set_memory_limit(conf, 64 * 1024 * 1024);
    pulsar_client_t *client = pulsar_client_create("pulsar://localhost:6650", conf);


    if (client == NULL) {
        printf("create pulsar client failed! \n");
    } else {
        printf("create pulsar client ok: %p\n", client);
    }

    flb_output_set_context(ins, client);
    
    return FLB_OK;
}

static void cb_stdout_flush(const void *data, size_t bytes,
                            const char *tag, int tag_len,
                            struct flb_input_instance *i_ins,
                            void *out_context,
                            struct flb_config *config)
{
    printf("output context: %p\n", out_context);
    flb_pack_print(data, bytes);
    FLB_OUTPUT_RETURN(FLB_OK);
}

static int cb_stdout_exit(void *data, struct flb_config *config)
{

    printf("exit pulsar! \n");
    return 0;
}

struct flb_output_plugin out_stdout2_plugin = {
    .name         = "stdout2",
    .description  = "Prints events to STDOUT",
    .cb_init      = cb_stdout_init,
    .cb_flush     = cb_stdout_flush,
    .cb_exit      = cb_stdout_exit,
    .flags        = 0,
};
