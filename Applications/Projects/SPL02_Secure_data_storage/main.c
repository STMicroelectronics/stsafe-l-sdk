/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-L010 Secure data storage access example
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
 * @note   Zone IDs are aligned with STSAFE-L010 SPL02 personalization.
 *         Access parameters must be adapted for other personalizations.
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handle;
    uint32_t counter_value;

    uint8_t readBuffer[READ_BUFFER_SIZE];
    uint8_t random[RANDOM_SIZE];

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* ## Print Example instruction on terminal */
    printf(PRINT_CLEAR_SCREEN PRINT_RESET);
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                            STSAFE-L010 secure data storage zone access example                               -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- description :                                                                                                -");
    printf("\n\r- This examples illustrates how to makes use of the STSAFE-L data storage APIs by performing following         -");
    printf("\n\r- accesses/commands to the target STSAFE device                                                                -");
    printf("\n\r-          o Read STSAFE-L zone 'ZONE_INDEX'                                                                   -");
    printf("\n\r-          o Update STSAFE-L zone 'ZONE_INDEX'                                                                 -");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- Note : zone IDs used in this example are aligned with STSAFE-L010 SPL02 personalization                      -");
    printf("\n\r-        Accesses parameters must be adapted for other device personalization                                  -");
    printf("\n\r-                                                                                                              -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\n\r ( Press key to continue )");
    uart_getc();

    /* ## Initialize STSAFE-L010 device handler */
    stse_ret = stse_set_default_handler_value(&stse_handle);
    if (stse_ret != STSE_OK) {
        printf("\n\r ## stse_set_default_handler_value : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
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

    /* ## Read zone ZONE_INDEX (counter zone) */

    printf("\n\n\r ## stse_data_storage_read_counter_zone (zone : %d ; associated data length : %d) : ",
    		ZONE_INDEX,
			sizeof(readBuffer) / sizeof(readBuffer[0])
	);

    stse_ret = stse_data_storage_read_counter_zone(
        &stse_handle,      /* SE handler		*/
        ZONE_INDEX,         /* Zone index		*/
        0x0000,             /* Read Offset		*/
        readBuffer,         /* Read buffer		*/
        sizeof(readBuffer), /* Read length		*/
        04,                 /* Read chunk size	*/
        &counter_value,     /* Counter Value	*/
        STSE_NO_PROT
	);

    if (stse_ret != STSE_OK) {
        printf(""PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r",
        		stse_ret
		);
        apps_process_error(stse_ret);
    } else {
        printf(PRINT_GREEN"SUCCESS"PRINT_RESET"\n\r");
        printf("\n\n\r    - Associated Data : \n\r");
        apps_print_hex_buffer(readBuffer, sizeof(readBuffer));
        printf("\n\n\r    - Current counter value : %lu",counter_value);
    }

    /*## Generate random number */

    apps_randomize_buffer(random, sizeof(random));

    /* ## Decrement zone ZONE_INDEX counter and store Randomized Associated data */

    printf("\n\n\n\r ## stse_data_storage_decrement_counter_zone (zone = %d ; amount = 1 ; data length = %d) : ",
            		ZONE_INDEX,
    				sizeof(random) / sizeof(random[0])
    );

    stse_ret = stse_data_storage_decrement_counter_zone(
        &stse_handle,  /* SE handler 			*/
        ZONE_INDEX,     /* Zone index 			*/
        1,              /* Decrement amount		*/
        0x0000,         /* Update Offset 		*/
        random,         /* Update input buffer 	*/
        sizeof(random), /* Update Length 		*/
        &counter_value, /* Counter value		*/
        STSE_NO_PROT
	);

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r",
        		stse_ret
		);
        apps_process_error(stse_ret);
    } else {
        printf(PRINT_GREEN"SUCCESS"PRINT_RESET"\n\r");
        printf("\n\n\r    - Associated Data : \n\r");
        apps_print_hex_buffer(random, sizeof(random));
        printf("\n\n\r    - Current counter value : %lu",counter_value);
    }

    /* ## Read Zone ZONE_INDEX (counter zone) */

    printf("\n\n\n\r ## stse_data_storage_read_counter_zone (zone : %d ; associated data length : %d) : ",
    		ZONE_INDEX,
			sizeof(readBuffer) / sizeof(readBuffer[0])
	);

    stse_ret = stse_data_storage_read_counter_zone(
        &stse_handle,      /* SE handler		*/
        ZONE_INDEX,         /* Zone index		*/
        0x0000,             /* Read Offset		*/
        readBuffer,         /* Read buffer		*/
        sizeof(readBuffer), /* Read length		*/
        04,                 /* Read chunk size	*/
        &counter_value,     /* Counter Value	*/
        STSE_NO_PROT);

    if (stse_ret != STSE_OK) {
        printf(""PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r",
        		stse_ret
		);
        apps_process_error(stse_ret);
    } else {
        printf(PRINT_GREEN"SUCCESS"PRINT_RESET"\n\r");
        printf("\n\n\r    - Associated Data : \n\r");
        apps_print_hex_buffer(readBuffer, sizeof(readBuffer));
        printf("\n\n\r    - Current counter value : %lu",counter_value);
    }


    /* ## Compare Associated data buffers */

    if (apps_compare_buffers(random, readBuffer, sizeof(random))) {
    	printf("\n\n\n\r ## zone %d update : "PRINT_RED"ERROR "PRINT_RESET"\n\r",
    			ZONE_INDEX
		);
    } else {
    	printf("\n\n\r ## zone %d update : "PRINT_GREEN"SUCCESS "PRINT_RESET"\n\r",
    			ZONE_INDEX
		);
    }

    printf("\n\n\r*#*# STMICROELECTRONICS #*#*\n\r");

    while (1) {
        // infinite loop
    }

    return 0;
}
