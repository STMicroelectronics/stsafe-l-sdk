/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-L010 Echo loop example
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

#define ECHO_LOOP_COUNT 5

/**
 * @brief  Main program entry point - STSAFE-L010 Echo loop example
 * @details Demonstrates the echo command functionality:
 *          - Generates random message with random length (1-500 bytes)
 *          - Sends message to STSAFE-L010 device via echo command
 *          - Receives and verifies echoed message
 *          - Repeats continuously with 1 second delay
 * @retval Not applicable (infinite loop)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handle;
    uint16_t message_length = 0;

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* ## Print Example instruction on terminal */
    printf(PRINT_CLEAR_SCREEN);
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                               STSAFE-L010 SPL02 Echo loop example                                            -");
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

    for(uint8_t i=0 ; i<ECHO_LOOP_COUNT ; i++ ) {

    	printf("\n\n\r----------------------------------------------------------------------------------------------------------------");

        /* Generate random message length (1..500) */
        message_length = (uint16_t)(apps_generate_random_number() & 0x1FF);
        if ((message_length > 500) || (message_length == 0)) {
            message_length = 1;
        }

        /* Create message and echo buffers (max 500 bytes) */
        uint8_t message[500] = {0};
        uint8_t echoed_message[500] = {0};

        /* Fill message with random content */
        apps_randomize_buffer(message, message_length);

        /* Print message */
        printf("\n\n\r ## Echo Message %d :\n\r", i);
        apps_print_hex_buffer(message, message_length);

        /* Perform echo operation */
        stse_ret = stse_device_echo(&stse_handle, message, echoed_message, message_length);
        if (stse_ret != STSE_OK) {
            printf("\n\r ## stse_device_echo : "PRINT_RED"ERROR 0x%04X"PRINT_RESET"\n\r", stse_ret);
            apps_process_error(stse_ret);
        }

        /* Compare message and echoed message */
        if (apps_compare_buffers(message, echoed_message, message_length)) {
            printf("\n\n\r ## ECHO MESSAGES COMPARE ERROR (%d)", message_length);
            printf("\n\r\t Echoed Message :\n\r");
            apps_print_hex_buffer(echoed_message, message_length);
        }
        printf("\n\n\r ## Echoed Message %d :\n\r", i);
        apps_print_hex_buffer(echoed_message, message_length);


        /* Wait for 1s */
        apps_delay_ms(500);
    }

	printf("\n\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\n\r*#*# STMICROELECTRONICS #*#*\n\r");

    while (1) {
        // infinite loop
    }

}
