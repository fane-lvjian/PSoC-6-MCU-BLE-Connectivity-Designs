/*******************************************************************************
* File Name: debug.c
*
* Version: 1.0
*
* Description:
*  This file contains functions for printf functionality.
*
* Hardware Dependency:
*  CY8CKIT-062 PSoC6 BLE Pioneer Kit
* 
********************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "common.h"
    
#if (DEBUG_UART_ENABLED == ENABLED)

#if defined(__ARMCC_VERSION)
    
/* For MDK/RVDS compiler revise fputc function for printf functionality */
struct __FILE 
{
    int handle;  
};

enum 
{
    STDIN_HANDLE,
    STDOUT_HANDLE,
    STDERR_HANDLE
};

FILE __stdin = {STDIN_HANDLE};
FILE __stdout = {STDOUT_HANDLE};
FILE __stderr = {STDERR_HANDLE};

int fputc(int ch, FILE *file) 
{
    int ret = EOF;

    switch( file->handle )
    {
        case STDOUT_HANDLE:
            UART_DEB_PUT_CHAR(ch);
            ret = ch ;
            break ;

        case STDERR_HANDLE:
            ret = ch ;
            break ;

        default:
            file = file;
            break ;
    }
    return ret ;
}

#elif defined (__ICCARM__)      /* IAR */

/* For IAR compiler revise __write() function for printf functionality */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers. Since we
         * don't we just return. (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return (0);
    }

    for (/* Empty */; size != 0; --size)
    {
        UART_DEB_PUT_CHAR(*buffer);
        ++buffer;
        ++nChars;
    }

    return (nChars);
}

#else  /* (__GNUC__)  GCC */

/* For GCC compiler revise _write() function for printf functionality */
int _write(int file, char *ptr, int len)
{
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {
        UART_DEB_PUT_CHAR(*ptr);
        ++ptr;
    }
    return len;
}


#endif  /* (__ARMCC_VERSION) */   

#endif /* DEBUG_UART_ENABLED == ENABLED */


/*******************************************************************************
* Function Name: PrintStackVersion
********************************************************************************
*
* Summary:
*   Prints the BLE Stack version if the PRINT_STACK_VERSION is defined.
*
*******************************************************************************/
void PrintStackVersion(void)
{
    cy_stc_ble_stack_lib_version_t stackVersion;
    cy_en_ble_api_result_t apiResult;
    
    apiResult = Cy_BLE_GetStackLibraryVersion(&stackVersion);
    if(apiResult != CY_BLE_SUCCESS)
    {
        DBG_PRINTF("Cy_BLE_GetStackLibraryVersion API Error: ");
        PrintApiResult(apiResult);
    }
    else
    {
        DBG_PRINTF("BLE Stack Version: %d.%d.%d.%d\r\n", stackVersion.majorVersion, 
                                                         stackVersion.minorVersion,
                                                         stackVersion.patch,
                                                         stackVersion.buildNumber);
    }
}

/*******************************************************************************
* Function Name: PrintApiResult
********************************************************************************
*
* Summary:
*   Decodes and prints the apiResult global variable value.
*
*******************************************************************************/
void PrintApiResult(cy_en_ble_api_result_t apiResult)
{
    DBG_PRINTF("0x%2.2x ", apiResult);
    
    switch(apiResult)
    {
        case CY_BLE_SUCCESS:
            DBG_PRINTF("ok\r\n");
            break;
            
        case CY_BLE_ERROR_INVALID_PARAMETER:
            DBG_PRINTF("invalid parameter\r\n");
            break;
            
        case CY_BLE_ERROR_INVALID_OPERATION:
            DBG_PRINTF("invalid operation\r\n");
            break;
            
        case CY_BLE_ERROR_NO_DEVICE_ENTITY:
            DBG_PRINTF("no device entity\r\n");
            break;
            
        case CY_BLE_ERROR_NTF_DISABLED:
            DBG_PRINTF("notification is disabled\r\n");
            break;
            
        case CY_BLE_ERROR_IND_DISABLED:
            DBG_PRINTF("indication is disabled\r\n");
            break;
            
        case CY_BLE_ERROR_CHAR_IS_NOT_DISCOVERED:
            DBG_PRINTF("characteristic is not discovered\r\n");
            break;
            
        case CY_BLE_ERROR_INVALID_STATE:
            DBG_PRINTF("invalid state");
            break;
            
        case CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE:
            DBG_PRINTF("invalid attribute handle\r\n");
            break;
            
        case CY_BLE_ERROR_FLASH_WRITE_NOT_PERMITED:
            DBG_PRINTF("flash write not permitted\r\n");
            break;
            
        default:
            DBG_PRINTF("other api result\r\n");
            break;
    }
}


/* [] END OF FILE */
