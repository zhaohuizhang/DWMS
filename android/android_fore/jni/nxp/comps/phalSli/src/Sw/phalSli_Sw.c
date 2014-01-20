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
* Software ICode SLI Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:40 2011 $
*
* History:
*  CHu: Generated 10. November 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phalSli.h>
#include <phpalSli15693.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_SLI_SW

#include "../phalSli_Int.h"
#include "phalSli_Sw.h"
#include "phalSli_Sw_Int.h"

phStatus_t phalSli_Sw_Init(
                           phalSli_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams,
                           void * pPalSli15693DataParams
                           )
{
    if (sizeof(phalSli_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_SLI);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalSli15693DataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_SLI | PHAL_SLI_SW_ID;
    pDataParams->pPalSli15693DataParams = pPalSli15693DataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_GetRandomNumber(
                                      phalSli_Sw_DataParams_t * pDataParams,
                                      uint8_t * pRnd
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;

    aCommand[0] = PHAL_SLI_CMD_GET_RANDOM_NUMBER;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        &pRxBuffer,
        &wRxLength));

    /* Check number of data received */
    if (wRxLength != 2)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_SLI);
    }

    /* copy received data block */
    memcpy(pRnd, pRxBuffer, wRxLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_SetPWD(
                             phalSli_Sw_DataParams_t * pDataParams,
                             uint8_t bPwdIdentifier,
                             uint8_t * pXorPwd
                             )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[6];

    aCommand[0] = PHAL_SLI_CMD_SET_PASSWORD;
    aCommand[1] = bPwdIdentifier;
    memcpy(&aCommand[2], pXorPwd, 4); /* PRQA S 3200 */

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        6,
        NULL,
        NULL));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_WritePWD(
                               phalSli_Sw_DataParams_t * pDataParams,
                               uint8_t bPwdIdentifier,
                               uint8_t * pPwd
                               )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[6];

    aCommand[0] = PHAL_SLI_CMD_WRITE_PASSWORD;
    aCommand[1] = bPwdIdentifier;
    memcpy(&aCommand[2], pPwd, 4); /* PRQA S 3200 */

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        6,
        NULL,
        NULL));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_LockPWD(
                              phalSli_Sw_DataParams_t * pDataParams,
                              uint8_t bPwdIdentifier
                              )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[2];

    aCommand[0] = PHAL_SLI_CMD_LOCK_PASSWORD;
    aCommand[1] = bPwdIdentifier;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        NULL,
        NULL));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_64BitPWDProtection(
    phalSli_Sw_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1];

    aCommand[0] = PHAL_SLI_CMD_64BIT_PASSWORD_PROTECT;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_ProtectPage(
                                  phalSli_Sw_DataParams_t * pDataParams,
                                  uint8_t bPageNo,
                                  uint8_t bProtectionStatus
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[3];

    aCommand[0] = PHAL_SLI_CMD_PROTECT_PAGE;
    aCommand[1] = bPageNo;
    aCommand[2] = bProtectionStatus;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        3,
        NULL,
        NULL));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_LockPageProtectionCondition(
    phalSli_Sw_DataParams_t * pDataParams,
    uint8_t bPageNo
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[2];

    aCommand[0] = PHAL_SLI_CMD_LOCK_PAGE_PROTECT_COND;
    aCommand[1] = bPageNo;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        NULL,
        NULL));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_GetMultipleBlockProtStatus(
    phalSli_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t * pProtectionStates,
    uint8_t * pNumReceivedStates
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[3];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;

    *pNumReceivedStates = 0;

    /* Number of wNumBlocks can't be zero */
    if (wNumBlocks == 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_SLI);
    }

    /* adjust number of blocks. */
    --wNumBlocks;

    /* Check number of blocks doesn't exceed 256 */
    if ((uint16_t)bBlockNo + wNumBlocks >= 0x100)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_SLI);
    }

    aCommand[0] = PHAL_SLI_CMD_GET_MULT_BLOCK_PROT_ST;
    aCommand[1] = bBlockNo;
    aCommand[2] = (uint8_t)wNumBlocks;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        3,
        &pRxBuffer,
        &wRxLength));

    /* Return number of received states */
    *pNumReceivedStates = (uint8_t)wRxLength;

    /* Copy received states */
    memcpy(pProtectionStates, pRxBuffer, wRxLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_Destroy(
                              phalSli_Sw_DataParams_t * pDataParams
                              )
{
    uint8_t     PH_MEMLOC_REM aCommand[1];
    phStatus_t  PH_MEMLOC_REM statusTmp;

    aCommand[0] = PHAL_SLI_CMD_DESTROY_SLI;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_EnablePrivacy(
                                    phalSli_Sw_DataParams_t * pDataParams
                                    )
{
    uint8_t     PH_MEMLOC_REM aCommand[1];
    phStatus_t  PH_MEMLOC_REM statusTmp;

    aCommand[0] = PHAL_SLI_CMD_ENABLE_PRIVACY;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_SetEAS(
                             phalSli_Sw_DataParams_t * pDataParams
                             )
{
    uint8_t     PH_MEMLOC_REM aCommand[1];
    phStatus_t  PH_MEMLOC_REM statusTmp;

    aCommand[0] = PHAL_SLI_CMD_SET_EAS;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_ResetEAS(
                               phalSli_Sw_DataParams_t * pDataParams
                               )
{
    uint8_t     PH_MEMLOC_REM aCommand[1];
    phStatus_t  PH_MEMLOC_REM statusTmp;

    aCommand[0] = PHAL_SLI_CMD_RESET_EAS;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_LockEAS(
                              phalSli_Sw_DataParams_t * pDataParams
                              )
{
    uint8_t     PH_MEMLOC_REM aCommand[1];
    phStatus_t  PH_MEMLOC_REM statusTmp;

    aCommand[0] = PHAL_SLI_CMD_LOCK_EAS;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_EASAlarm(
                               phalSli_Sw_DataParams_t * pDataParams,
                               uint8_t bOption,
                               uint8_t * pEasIdValue,
                               uint8_t bEasIdMaskLength,
                               uint8_t * pEas
                               )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[4];
    uint16_t    PH_MEMLOC_REM wTxLength;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t *   PH_MEMLOC_REM pRxBuffer;

    aCommand[0] = PHAL_SLI_CMD_EAS_ALARM;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, bOption));

    if (bOption != PHAL_SLI_OPTION_OFF)
    {
        wTxLength = 2 + (uint16_t)(bEasIdMaskLength >> 3); 
        aCommand[1] = bEasIdMaskLength;
        memcpy(&aCommand[2], pEasIdValue, wTxLength - 2); /* PRQA S 3200 */
    }
    else
    {
        wTxLength = 1;
    }

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        wTxLength,
        &pRxBuffer,
        &wRxLength));

    if ((bOption != PHAL_SLI_OPTION_OFF) && (bEasIdMaskLength == 0))
    {
        if (wRxLength != 2)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_SLI);
        }
    }
    else
    {
        if (wRxLength != 32)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_SLI);
        }
    }
    /* copy received data block */
    memcpy(pEas, pRxBuffer, wRxLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_PwdProtectEAS(
                                    phalSli_Sw_DataParams_t * pDataParams
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1];

    aCommand[0] = PHAL_SLI_CMD_PASSWORD_PROTECT_EAS;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_WriteEAS_ID(
                                  phalSli_Sw_DataParams_t * pDataParams,
                                  uint8_t * pEasIdValue
                                  )
{
    uint8_t     PH_MEMLOC_REM aCommand[3];
    phStatus_t  PH_MEMLOC_REM statusTmp;

    aCommand[0] = PHAL_SLI_CMD_WRITE_EAS_ID;
    aCommand[1] = pEasIdValue[0];
    aCommand[2] = pEasIdValue[1];

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    return (phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        3,
        NULL,
        NULL));
}

phStatus_t phalSli_Sw_ReadEPC(
                              phalSli_Sw_DataParams_t * pDataParams,
                              uint8_t * pEpc
                              )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;

    aCommand[0] = PHAL_SLI_CMD_READ_EPC;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalSli_Sw_Int_SetOptionBit(pDataParams, PHAL_SLI_OPTION_OFF));

    /* Set long timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        &pRxBuffer,
        &wRxLength));

    if (wRxLength != 12)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_SLI);
    }

    /* copy received data block */
    memcpy(pEpc, pRxBuffer, wRxLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

phStatus_t phalSli_Sw_Int_SetOptionBit(
                                       phalSli_Sw_DataParams_t * pDataParams,
                                       uint8_t bOption
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wFlagsCurrent;
    uint16_t    PH_MEMLOC_REM wFlagsNew;

    /* Next check if the Option Byte has a valid value */
    if ((bOption != PHAL_SLI_OPTION_OFF) && (bOption != PHAL_SLI_OPTION_ON))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_SLI);
    }

    /* retrieve the flags byte */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_GetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_FLAGS,
        &wFlagsCurrent));

    /* Set Option bit */
    if (bOption != PHAL_SLI_OPTION_OFF)
    {
        wFlagsNew = wFlagsCurrent | PHPAL_SLI15693_FLAG_OPTION;
    }
    /* Clear option bit */
    else
    {
        wFlagsNew = wFlagsCurrent & (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;
    }

    /* update the flags byte */
    if (wFlagsNew != wFlagsCurrent)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
            pDataParams->pPalSli15693DataParams,
            PHPAL_SLI15693_CONFIG_FLAGS,
            wFlagsNew));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_SLI);
}

#endif /* NXPBUILD__PHAL_SLI_SW */
