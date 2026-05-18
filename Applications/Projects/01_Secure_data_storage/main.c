/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-L010 Secure data storage access example
 ******************************************************************************
 *           			COPYRIGHT 2022 STMicroelectronics
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
#define READ_BUFFER_SIZE 16 /**< Size of read buffer for zone data */
#define RANDOM_SIZE 16      /**< Size of random data to write to zone */
#define ZONE_INDEX 32U      /**< Index of the zone */

/**
 * @brief  Main program entry point - STSAFE-L010 Secure data storage zone access
 * @details Demonstrates data storage partition operations:
 *          - Queries total partition count
 *          - Retrieves and displays partition configuration table
 *          - Reads data from zone 1 and displays content
 *          - Generates random data and writes to zone 1
 *          - Reads back zone 1 to verify the update
 *          - Compares read data with written data
 * @note   Zone IDs are aligned with STSAFE-L010 SPL01 personalization.
 *         Access parameters must be adapted for other personalizations.
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;
    uint32_t counter_value;

    uint8_t readBuffer[READ_BUFFER_SIZE];
    uint8_t random[RANDOM_SIZE];

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf(PRINT_CLEAR_SCREEN PRINT_RESET);
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                            STSAFE-L010 secure data storage zone access example                               -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- description :                                                                                                -");
    printf("\n\r- This examples illustrates how to makes use of the STSAFE-L data storage APIs by performing following         -");
    printf("\n\r- accesses/commands to the target STSAFE device                                                                -");
    printf("\n\r-          o Query STSAFE-L total partition count                                                              -");
    printf("\n\r-          o Query STSAFE-L partitions information                                                             -");
    printf("\n\r-          o Read STSAFE-L zone 'ZONE_INDEX'                                                                   -");
    printf("\n\r-          o Update STSAFE-L zone 'ZONE_INDEX'                                                                 -");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- Note : zone IDs used in this example are aligned with STSAFE-L010 SPL01 personalization                      -");
    printf("\n\r-        Accesses parameters must be adapted for other device personalization                                  -");
    printf("\n\r-                                                                                                              -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");

    /* ## Initialize STSAFE-L0xx device handler */
    stse_ret = stse_set_default_handler_value(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\r ## stse_set_default_handler_value ERROR : 0x%04X\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    stse_handler.device_type = STSAFE_L010;
    stse_handler.io.busID = 1;
    stse_handler.io.BusSpeed = 100;
    stse_handler.io.Devaddr = 0x0C;

    printf("\n\r - Initialize target STSAFE-L010");
    stse_ret = stse_init(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\r ## stse_init ERROR : 0x%04X\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    /* ## Read zone ZONE_INDEX (counter zone) */
    stse_ret = stse_data_storage_read_counter_zone(
        &stse_handler,      /* SE handler		*/
        ZONE_INDEX,         /* Zone index		*/
        0x0000,             /* Read Offset		*/
        readBuffer,         /* Read buffer		*/
        sizeof(readBuffer), /* Read length		*/
        04,                 /* Read chunk size	*/
        &counter_value,     /* Counter Value	*/
        STSE_NO_PROT);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ### stse_data_storage_read_data_zone : ERROR 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r - stse_data_storage_read_data_zone (zone : %d - length : %d - counter : %lu)", ZONE_INDEX, sizeof(readBuffer) / sizeof(readBuffer[0]), counter_value);
        apps_print_hex_buffer(readBuffer, sizeof(readBuffer));
    }

    /*## Generate random number */
    apps_randomize_buffer(random, sizeof(random));

    /* ## Decrement zone ZONE_INDEX counter and store Randomized Associated data */
    stse_ret = stse_data_storage_decrement_counter_zone(
        &stse_handler,  /* SE handler 			*/
        ZONE_INDEX,     /* Zone index 			*/
        1,              /* Decrement amount		*/
        0x0000,         /* Update Offset 		*/
        random,         /* Update input buffer 	*/
        sizeof(random), /* Update Length 		*/
        &counter_value, /* Counter value		*/
        STSE_NO_PROT);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ### stse_data_storage_decrement_counter_zone : ERROR 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r - stse_data_storage_decrement_counter_zone (zone = %d - length = %d - New counter : %lu)", ZONE_INDEX, sizeof(random) / sizeof(random[0]), counter_value);
        apps_print_hex_buffer(random, sizeof(random));
    }

    /* ## Read Zone ZONE_INDEX (counter zone) */
    stse_ret = stse_data_storage_read_counter_zone(
        &stse_handler,      /* SE handler		*/
        ZONE_INDEX,         /* Zone index		*/
        0x0000,             /* Read Offset		*/
        readBuffer,         /* Read buffer		*/
        sizeof(readBuffer), /* Read length		*/
        04,                 /* Read chunk size	*/
        &counter_value,     /* Counter Value	*/
        STSE_NO_PROT);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ### stse_data_storage_read_data_zone : ERROR 0x%04X", stse_ret);
    } else {
        printf(PRINT_GREEN "\n\n\r - stse_data_storage_read_data_zone (zone : %d - length : %d - counter : %lu)", ZONE_INDEX, sizeof(readBuffer) / sizeof(readBuffer[0]), counter_value);
        apps_print_hex_buffer(readBuffer, sizeof(readBuffer));
    }

    printf(PRINT_RESET "\n\r\n\r*#*# STMICROELECTRONICS #*#*\n\r");

    while (1) {
        // infinite loop
    }

    return 0;
}
