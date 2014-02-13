/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic Tools Component of phSecMsg component.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:58 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_SAMAV2

#include <phhalHw_SamAV2_Cmd.h>
#include "phhalHw_SamAV2_HcUtils.h"

phStatus_t phhalHw_SamAV2_HcUtils_GetCheckLcLe(
    uint8_t * pCmd,
    uint16_t wCmdLength,
    uint8_t * isLcPresent,
    uint8_t * LcLength,
    uint8_t * isLePresent
    )
{
    /* There are four cases: */
    /*1 CLA INS P1 P2 */
    /*2 CLA INS P1 P2 LE */
    /*3 CLA INS P1 P2 LC DATA */
    /*4 CLA INS P1 P2 LC DATA LE */

    if (wCmdLength < PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_FRAMING_ERROR, PH_COMP_HAL);
    }
    /* @1*/
    if (wCmdLength == PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH)
    {
        *LcLength = 0;
        *isLcPresent = PH_OFF;
        *isLePresent = PH_OFF;
    }
    /* @2*/
    else if (wCmdLength == PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH)
    {
        *LcLength = 0;
        *isLcPresent = PH_OFF;
        *isLePresent = PH_ON;
    }
    /* @3*/
    else if (wCmdLength == (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + pCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]))
    {
        *LcLength = pCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS];
        *isLcPresent = PH_ON;
        *isLePresent = PH_OFF;
    }
    /* @4*/
    else if (wCmdLength == (PHHAL_HW_SAMAV2_ISO7816_HEADER_LE_LENGTH + pCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]))
    {
        *LcLength = pCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS];
        *isLcPresent = PH_ON;
        *isLePresent = PH_ON;
    }else
    {
        return PH_ADD_COMPCODE(PH_ERR_FRAMING_ERROR, PH_COMP_HAL);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);


}

phStatus_t phhalHw_SamAV2_HcUtils_TruncateMacBuffer(
    uint8_t * pIoBuffer,
    uint8_t * MacLength
    )
{

    uint8_t count, truncateCount=0;
    for (count=1; count<(*MacLength); count+=2)
    {
        pIoBuffer[truncateCount++] = pIoBuffer[count] ;
    }
    *MacLength = truncateCount;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_HcUtils_Cmd_ResolveErrorCode(
    uint8_t * pSw1Sw2,
    uint16_t wComponentID
    )
{   
    uint16_t status = (((uint16_t)pSw1Sw2[0]) << 8);
    status |= pSw1Sw2[1];

    switch(status)
    {
        /**< Success status codes */
    case PHHAL_HW_SAMAV2_RET_CODE_OK:
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_OK_1BIT:
    case PHHAL_HW_SAMAV2_RET_CODE_OK_2BIT:
    case PHHAL_HW_SAMAV2_RET_CODE_OK_3BIT:
    case PHHAL_HW_SAMAV2_RET_CODE_OK_4BIT:
    case PHHAL_HW_SAMAV2_RET_CODE_OK_5BIT:
    case PHHAL_HW_SAMAV2_RET_CODE_OK_6BIT:
    case PHHAL_HW_SAMAV2_RET_CODE_OK_7BIT:
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS_INCOMPLETE_BYTE, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_OK_CHAINING_ACTIVE:
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE, PH_COMP_HAL);

        /**< Communication Error Codes codes */    
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_IO_TIMEOUT: 
        return PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_BITCNT_PROTOCOL: 
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_PARITY: 
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_CRC_FAILURE: 
        return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_FIFO_BUF_OVERFLOW: 
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_RF_FAILURE: 
        return PH_ADD_COMPCODE(PH_ERR_RF_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_TEMP_FAILURE: 
        return PH_ADD_COMPCODE(PH_ERR_TEMPERATURE_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_FIFO_WRITE: 
        return PH_ADD_COMPCODE(PH_ERR_READ_WRITE_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_COLLISION: 
        return PH_ADD_COMPCODE(PH_ERR_COLLISION_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_COMM_INTERNAL_BUF_OVERFLOW: 
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);

        /**< ISO Error Codes codes */
    case PHHAL_HW_SAMAV2_RET_CODE_ISO_UID_BCC_INCORRECT: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO_UID_BCC_INCORRECT, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO_WRONG_BNR: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO_WRONG_BNR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO_INVALID_FORMAT: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO_INVALID_FORMAT, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO_INVALID_PARAMETER: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO_INVALID_PARAMETER, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO_UID_INCOMPLETE: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO_UID_INCOMPLETE, PH_COMP_HAL);

        /**< Application protocol error codes */
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_ERROR: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_GEN, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_PLUS_ERROR:
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_CRYPTO, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_INS_MIFARE_PLUS_ERROR: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_DESFIRE_ERROR: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_DESFIRE_GEN, PH_COMP_HAL);    
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_0: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_NAK0, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_1: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_NAK1, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_4: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_NAK4, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_5: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_NAK5, PH_COMP_HAL);

        /**< Crypto error codes */
    case PHHAL_HW_SAMAV2_RET_CODE_CRYPTO_FAILURE: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);

        /**< ISO7816 Error Codes codes */
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_LENGTH_LC: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_LENGTH_LC, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_P1P2: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_P1P2, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_LE: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_LE, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_UNKNOWN_INS: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_UNKNOWN_INS, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_CLASS: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_UNKNOWN_CLASS, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_COMMAND_NOT_ALLOWED: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_COMMAND_NOT_ALLOWED, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_PARAMS_FOR_INS: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_PARAMS_FOR_INS, PH_COMP_HAL);

        /**< SAM Hardware related Error Codes codes */
    case PHHAL_HW_SAMAV2_RET_CODE_HW_EEPROM: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_HW_EEPROM, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_HW_RC5XX: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_HW_RC5XX, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_HW_EE_HIGH_VOLTAGE: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_HW_EE_HIGH_VOLTAGE, PH_COMP_HAL);

        /**< Key management error codes */
    case PHHAL_HW_SAMAV2_RET_CODE_KEY_CREATE_FAILED: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_KEY_CREATE_FAILED, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_KEY_REF_NO_INVALID: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_KEY_REF_NO_INVALID, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_KEY_KUC_NO_INVALID: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_KEY_KUC_NO_INVALID, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_KEY_INTEGRITY_ERROR: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_KEY_INTEGRITY_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_KEY_VERSION_INVALID: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_KEY_VERSION_INVALID, PH_COMP_HAL);

        /**< Other error codes */
    case PHHAL_HW_SAMAV2_RET_CODE_COND_USE_NOT_SATISFIED: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_COND_USE_NOT_SATISFIED, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_INTEGRITY_ERROR: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_INTEGRITY_ERROR, PH_COMP_HAL);
    case PHHAL_HW_SAMAV2_RET_CODE_INCOMPLETE_CHAINING: 
        return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_INCOMPLETE_CHAINING, PH_COMP_HAL);
    default:
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, wComponentID);
    }
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
