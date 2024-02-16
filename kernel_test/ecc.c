// SPDX-License-Identifier: GPL-2.0
/*
 *  ecc_test.c - The simplest kernel module.
 *
 *  Written 2023 by Eric Christian
 */

#include "ecc.h"

static struct rs_control *rs = NULL;

static int __init ecc_init(void)
{
    pr_info("ECC Init\n");

    rs = init_rs_gfp(8, 0x11d, 0, 1, 32, GFP_KERNEL);
    if (rs == NULL) {
        pr_err("Failed to initialize RS control\n");
        return -1;
    }

    // Encode data
    u8 * data = kzalloc(223 * sizeof(u8), GFP_KERNEL);
    if (!data) {
        pr_err("Failed to allocate data\n");
        goto cleanup_rs;
    }

    u16 * ecc = kzalloc(32 * sizeof(u16), GFP_KERNEL);
    if (!ecc) {
        pr_err("Failed to allocate ecc\n");
        goto cleanup_data;
    }

    for (int i = 0; i < 223; i++) {
        data[i] = i;
    }

    if (encode_rs8(rs, data, 223, ecc, 0)) {
        pr_err("Failed to encode data\n");
        goto cleanup_ecc;
    }

    // Decode data
    u8 * data2 = kzalloc(223 * sizeof(u8), GFP_KERNEL);
    if (!data2) {
        pr_err("Failed to allocate data2\n");
        goto cleanup_ecc;
    }

    for (int i = 0; i < 223; i++) {
        data2[i] = data[i];
    }
    data2[1] = 0;
    data2[2] = 0;
    data2[3] = 0;
    data2[4] = 0;

    int corrected = decode_rs8(rs, data2, ecc, 223, NULL, 0, NULL, 0, NULL);

    if (corrected < 0) {
        pr_err("Failed to decode data\n");
        goto cleanup_ecc;
    } else {
        pr_info("Corrected %d errors\n", corrected);
    }

    for (int i = 0; i < 223; i++) {
        if (data[i] != data2[i]) {
            pr_err("Data mismatch at index %d\n", i);
            goto cleanup_ecc;
        }
    }

    pr_info("ECC Test Passed\n");

    return 0;

cleanup_ecc:
    kfree(ecc);

cleanup_data:
    kfree(data);

cleanup_rs:
    free_rs(rs);

    return -1;
}


static void __exit ecc_exit(void)
{
    free_rs(rs);
    pr_info("ECC Exit\n");
}


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ECC Driver");
MODULE_AUTHOR("DM");

module_init(ecc_init);
module_exit(ecc_exit);