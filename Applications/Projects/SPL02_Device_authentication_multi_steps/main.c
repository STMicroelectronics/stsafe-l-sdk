/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-L010 Device authentication multi steps example
 ******************************************************************************
 *                              COPYRIGHT 2022 STMicroelectronics
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "Apps_utils.h"

/* Defines -------------------------------------------------------------------*/

/* Root CA key used for STSAFE-L010 SPL02 */
#define CA_SELF_SIGNED_CERTIFICATE_01                                                                   \
    0x30, 0x82, 0x01, 0x67, 0x30, 0x82, 0x01, 0x19, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01, 0x01,     \
        0x30, 0x05, 0x06, 0x03, 0x2b, 0x65, 0x70, 0x30, 0x4b, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, \
        0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x1e, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, \
        0x15, 0x53, 0x54, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x65, 0x6c, 0x65, 0x63, 0x74, 0x72, 0x6f, 0x6e, \
        0x69, 0x63, 0x73, 0x20, 0x6e, 0x76, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, \
        0x13, 0x53, 0x54, 0x4d, 0x5f, 0x53, 0x54, 0x53, 0x41, 0x46, 0x45, 0x2d, 0x4c, 0x5f, 0x43, 0x41, \
        0x30, 0x30, 0x30, 0x31, 0x30, 0x20, 0x17, 0x0d, 0x32, 0x34, 0x30, 0x36, 0x30, 0x37, 0x30, 0x30, \
        0x30, 0x30, 0x30, 0x30, 0x5a, 0x18, 0x0f, 0x32, 0x30, 0x35, 0x34, 0x30, 0x36, 0x30, 0x37, 0x30, \
        0x30, 0x30, 0x30, 0x30, 0x30, 0x5a, 0x30, 0x4b, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, \
        0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x1e, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x15, \
        0x53, 0x54, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x65, 0x6c, 0x65, 0x63, 0x74, 0x72, 0x6f, 0x6e, 0x69, \
        0x63, 0x73, 0x20, 0x6e, 0x76, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x13, \
        0x53, 0x54, 0x4d, 0x5f, 0x53, 0x54, 0x53, 0x41, 0x46, 0x45, 0x2d, 0x4c, 0x5f, 0x43, 0x41, 0x30, \
        0x30, 0x30, 0x31, 0x30, 0x2a, 0x30, 0x05, 0x06, 0x03, 0x2b, 0x65, 0x70, 0x03, 0x21, 0x00, 0x84, \
        0x47, 0xf2, 0xc0, 0x98, 0xbe, 0x15, 0xf6, 0x05, 0xc6, 0x98, 0xd5, 0xfa, 0xc5, 0x7b, 0x56, 0x0f, \
        0x1c, 0xcf, 0x1f, 0x37, 0x9f, 0xe9, 0x88, 0xaa, 0x2f, 0xea, 0xa2, 0x93, 0xb5, 0xde, 0xd1, 0xa3, \
        0x20, 0x30, 0x1e, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x08, 0x30, 0x06, 0x01, 0x01, \
        0xff, 0x02, 0x01, 0x00, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x04, 0x04, 0x03, 0x02, 0x02, \
        0x04, 0x30, 0x05, 0x06, 0x03, 0x2b, 0x65, 0x70, 0x03, 0x41, 0x00, 0x29, 0x31, 0xa8, 0x6f, 0xed, \
        0x6f, 0xd7, 0x54, 0x8a, 0x03, 0x20, 0x72, 0x84, 0x5d, 0x77, 0x87, 0x97, 0xe2, 0x36, 0x4b, 0x52, \
        0x65, 0xea, 0xc1, 0xbf, 0x7b, 0x39, 0x36, 0x05, 0x75, 0x5b, 0x5d, 0x76, 0x19, 0xfd, 0x32, 0x9c, \
        0xfd, 0x90, 0x93, 0x17, 0xf9, 0x53, 0xb4, 0xb8, 0x21, 0xdf, 0x9c, 0xe2, 0x0d, 0xa7, 0xcd, 0x74, \
        0x5b, 0x9e, 0xf1, 0xab, 0xf7, 0x94, 0x9f, 0x4a, 0xb0, 0xfb, 0x05

#define STSAFE_CERTIFICATE_ZONE_0 0U
#define STSE_STATIC_PRIVATE_KEY_SLOT_0 0U

/**
 * @brief  Main program entry point - STSAFE-L010 Multi-step device authentication
 * @details Demonstrates detailed device authentication process with individual steps:
 *          - Reads device certificate from STSAFE-L010 zone 0
 *          - Parses both CA and device certificates into structured format
 *          - Extracts and displays certificate information (subject, issuer, keys)
 *          - Verifies device certificate signature using CA public key
 *          - Generates random challenge for device
 *          - Requests device signature on challenge using private key
 *          - Verifies signature using device's public key from certificate
 * @note   This multi-step approach shows each authentication phase separately,
 *         useful for understanding the authentication protocol or implementing
 *         custom authentication flows for distant server scenarios
 * @retval 0 on success (never reached due to infinite loop)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handle;
    PLAT_UI16 certificate_size;
    stse_certificate_t parsed_stse_certificate;
    stse_certificate_t parsed_ca_selfsigned_cert;
    static const uint8_t ca_selfsigned_cert[] = {CA_SELF_SIGNED_CERTIFICATE_01};

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* ## Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                       STSAFE-L010 SPL02 Multi-Steps Device Authentication Example                            -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r- This example illustrates STSAFE-L010 device authentication process using Multi-Step approach.                -");
    printf("\n\r- it can be taken as reference for building distant server authentication use cases.                           -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\n\r ( Press key to continue )");
    uart_getc();

    /* ## Initialize STSAFE-L010 device handler */
    stse_ret = stse_set_default_handler_value(&stse_handle);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r ## stse_set_default_handler_value : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    stse_handle.device_type = STSAFE_L010;
    stse_handle.io.BusType = STSE_BUS_TYPE_ST1WIRE;
    stse_handle.io.Devaddr = 0x00;

    printf("\n\n\r ## Initialize target STSAFE-L010");
    stse_ret = stse_init(&stse_handle);
    if (stse_ret != STSE_OK) {
        printf("\n\r ## stse_init : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    /* ## Parse CA self-signed certificate */
    stse_ret = stse_certificate_parse(ca_selfsigned_cert, &parsed_ca_selfsigned_cert, NULL);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## ST CA stse_certificate_parse ERROR : 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    }
    printf("\n\n\r## CA self-signed certificate : \n\r");
    stse_certificate_print_parsed_cert(&parsed_ca_selfsigned_cert);

    /* ## Get target STSAFE-L010 Certificate  */
    stse_ret = stse_get_device_certificate_size(&stse_handle, STSAFE_CERTIFICATE_ZONE_0, &certificate_size);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_get_device_certificate_size : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    PLAT_UI8 stse_certificate[certificate_size];

    stse_ret = stse_get_device_certificate(&stse_handle, STSAFE_CERTIFICATE_ZONE_0, certificate_size, stse_certificate);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_get_device_certificate : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    /* ## Parse target STSAFE-L010 certificate */
    stse_ret = stse_certificate_parse(stse_certificate, &parsed_stse_certificate, NULL);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## STSAFE-L010 stse_certificate_parse : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    }
    printf("\n\n\r## Target STSAFE-L010 certificate : \n\r");
    stse_certificate_print_parsed_cert(&parsed_stse_certificate);

    /* ## Verify STSAFE-L010 Certificate with CA self-signed Certificate */
    stse_ret = stse_certificate_is_parent(&parsed_ca_selfsigned_cert, &parsed_stse_certificate, NULL);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_certificate_is_parent : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r## Device Certificate Verified\n\r");
    }

    /* ## Extract certificate key type */
    stse_ecc_key_type_t key_type = stse_certificate_get_key_type(&parsed_stse_certificate);
    PLAT_UI16 signature_size = stse_ecc_info_table[key_type].signature_size;
    PLAT_UI8 signature[signature_size];

    /* ## Generate a challenge ( aligned with key type size requirement) */
    printf("\n\n\r## Host random challenge : \n\r");
    PLAT_UI16 challenge_size = STSAFEL_ECC_SIGNATURE_CHALLENGE_LENGTH;
    PLAT_UI8 challenge[challenge_size];
    apps_randomize_buffer(challenge, challenge_size);
    apps_print_hex_buffer(challenge, challenge_size);

    /* ##  Signature over challenge */
    stse_ret = stse_ecc_generate_signature(
        &stse_handle,                  /* STSE handler */
        STSE_STATIC_PRIVATE_KEY_SLOT_0, /* Slot number */
        key_type,                       /* Certificate key type */
        challenge,                      /* Challenge */
        challenge_size,                 /* Challenge size */
        signature);                     /* Challenge signature */

    if (stse_ret != STSE_OK) {
        printf("\n\r## stse_ecc_generate_signature : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
        apps_process_error(stse_ret);
    }
    printf("\n\n\r## Device signature over Host challenge: ");
    apps_print_hex_buffer(signature, signature_size);

    /*## Verify The Signature of the Random Number*/

    stse_ret = stse_certificate_verify_signature(
        &parsed_stse_certificate,
        challenge,
        challenge_size,
        signature, (signature_size >> 1),
        &signature[signature_size >> 1], (signature_size >> 1));
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_certificate_verify_signature : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
    } else {
        printf("\n\n\r## Device Authentication : "PRINT_GREEN"SUCCESS"PRINT_RESET" (Challenge signature verified successfully)");
    }

    printf(PRINT_RESET "\n\r\n\r*#*# STMICROELECTRONICS #*#*\n\r");

    while (1) {
        // infinite loop
    }

    return 0;
}
