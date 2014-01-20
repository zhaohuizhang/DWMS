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
* RD710Sam BAL Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:52 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <phbalReg.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHBAL_REG_RD710SAM

#include "phbalReg_Rd710Sam.h"
#include "phbalReg_Rd710Sam_Int.h"

/* Supported Fi/Di values */
static const uint8_t PH_MEMLOC_CONST_ROM PH_MEMLOC_CONST_ROM gaphbalReg_Rd710Sam_SupportedFiDiValues[] =
{
    0x11,
    0x12,
    0x13,
    0x18,
    0x95,
    0x96
};

phStatus_t phbalReg_Rd710Sam_Init(
                                  phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  void * pLowerBalDataParams,
                                  uint8_t * pAtrBuffer,
                                  uint16_t wAtrBufSize,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxBufSize,
                                  uint8_t * pRxBuffer,
                                  uint16_t wRxBufSize
                                  )
{
    if (sizeof(phbalReg_Rd710Sam_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    }
    /* Verify buffer sizes */
    if ((wAtrBufSize == 0) || (wTxBufSize == 0) || (wRxBufSize == 0))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_BAL);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pLowerBalDataParams);
    PH_ASSERT_NULL (pAtrBuffer);
    PH_ASSERT_NULL (pTxBuffer);
    PH_ASSERT_NULL (pRxBuffer);

    /* initialize the data parameters to default values */
    pDataParams->wId                = PH_COMP_BAL | PHBAL_REG_RD710_SAM_ID;
    pDataParams->pLowerBalDataParams   = pLowerBalDataParams;
    pDataParams->pAtrBuffer         = pAtrBuffer;
    pDataParams->wAtrBufSize        = wAtrBufSize;
    pDataParams->pTxBuffer          = pTxBuffer;
    pDataParams->wTxBufSize         = wTxBufSize;
    pDataParams->pRxBuffer          = pRxBuffer;
    pDataParams->wRxBufSize         = wRxBufSize;
    pDataParams->bCardOpened        = PH_OFF;

    /* Reset ATR length */
    pDataParams->pAtrBuffer[0] = 0x00;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_GetPortList(
    phbalReg_Rd710Sam_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    )
{
    /* satisfy compiler */
    if (pDataParams || wPortBufSize || pPortNames || pNumOfPorts);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_SetPort(
                                     phbalReg_Rd710Sam_DataParams_t * pDataParams, 
                                     uint8_t * pPortName
                                     )
{
    /* satisfy compiler */
    if (pDataParams || pPortName);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_Exchange(
                                      phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength,
                                      uint16_t wRxBufSize,
                                      uint8_t * pRxBuffer,
                                      uint16_t * pRxLength
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wCmd;
    uint16_t    PH_MEMLOC_REM wOffTxBuf;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t *   PH_MEMLOC_REM pPayload;

    /* Check options */
    if (wOption != PH_EXCHANGE_DEFAULT)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_BAL);
    }

    /* Port should be open */
    if (pDataParams->bCardOpened == PH_OFF)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_BAL);
    }

    /* Check if TxBuffer is big enough */
    if (pDataParams->wTxBufSize < (2 + wTxLength))
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_BAL);
    }

    /* Reset receive length */
    *pRxLength = 0;

    /* Init parameters of Exchange */
    wCmd = PHBAL_RD710SAM_CMD_CC_TRANSMIT_DATA;
    wOffTxBuf = 0;
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd >> 8);
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd & 0xFF);
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wTxLength >> 8);
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wTxLength & 0xFF);
    memcpy(&pDataParams->pTxBuffer[wOffTxBuf], pTxBuffer, wTxLength);  /* PRQA S 3200 */

    /* Perform command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
        pDataParams->pLowerBalDataParams,
        PH_EXCHANGE_DEFAULT,
        pDataParams->pTxBuffer,
        wOffTxBuf + wTxLength,
        pDataParams->wRxBufSize,
        pDataParams->pRxBuffer,
        &wRxLength
        ));

    /* Check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Rd710Sam_CheckResponse(
        wCmd,
        pDataParams->pRxBuffer,
        wRxLength,
        &pPayload,
        &wRxLength));

    /* Check if RxBuffer is big enough */
    if (wRxBufSize < wRxLength)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_BAL);
    }

    /* Copy received contents */
    memcpy(pRxBuffer, pPayload, wRxLength);  /* PRQA S 3200 */
    *pRxLength = wRxLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_OpenPort(
                                      phbalReg_Rd710Sam_DataParams_t * pDataParams
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wCmd;
    uint16_t    PH_MEMLOC_REM wOffTxBuf;
    uint8_t *   PH_MEMLOC_REM pAtr;
    uint16_t    PH_MEMLOC_REM wAtrLength;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM bPps1;
    uint8_t     PH_MEMLOC_REM bSpecificMode;
    uint8_t     PH_MEMLOC_REM bDoPps;

    /* Port should be closed */
    if (pDataParams->bCardOpened != PH_OFF)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_BAL);
    }

    /* Reset ATR length */
    pDataParams->pAtrBuffer[0] = 0x00;

    /* Init parameters of Set PCSC Mode */
    wCmd = PHBAL_RD710SAM_CMD_RD_OPS_SET_PCSC_MODE;
    wOffTxBuf = 0;
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd >> 8);
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd & 0xFF);
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x01;
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x01;

    /* Perform Set PCSC Mode */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
        pDataParams->pLowerBalDataParams,
        PH_EXCHANGE_DEFAULT,
        pDataParams->pTxBuffer,
        wOffTxBuf,
        pDataParams->wRxBufSize,
        pDataParams->pRxBuffer,
        &wRxLength));

    /* Check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Rd710Sam_CheckResponse(
        wCmd,
        pDataParams->pRxBuffer,
        wRxLength,
        NULL,
        &wRxLength));

    /* Payload length should be 0 */
    if (wRxLength != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_BAL);
    }

    /* Init parameters of Activate Contact Card */
    wCmd = PHBAL_RD710SAM_CMD_ACTIVATE_CONTACT_CARD;
    wOffTxBuf = 0;
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd >> 8);
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd & 0xFF);
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;

    /* Perform Activate Contact Card */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
        pDataParams->pLowerBalDataParams,
        PH_EXCHANGE_DEFAULT,
        pDataParams->pTxBuffer,
        wOffTxBuf,
        pDataParams->wRxBufSize,
        pDataParams->pRxBuffer,
        &wRxLength));

    /* Check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Rd710Sam_CheckResponse(
        wCmd,
        pDataParams->pRxBuffer,
        wRxLength,
        &pAtr,
        &wAtrLength));

    /* Parse ATR */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Rd710Sam_ParseAtr(pAtr, wAtrLength, &bPps1, &bSpecificMode));

    /* Store ATR */
    if (wAtrLength < pDataParams->wAtrBufSize)
    {
        memcpy(pDataParams->pAtrBuffer, pAtr, wAtrLength); /* PRQA S 3200 */
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_BAL);
    }

    /* Do PPS only if not in specific mode and only if non-default values are used*/
    bDoPps = 0;
    if ((!bSpecificMode) && (bPps1 != 0x11))
    {
        /* Check if Fi/Di values are supported */
        for (wCmd = 0; wCmd < sizeof(gaphbalReg_Rd710Sam_SupportedFiDiValues); ++wCmd)
        {
            if (gaphbalReg_Rd710Sam_SupportedFiDiValues[wCmd] == bPps1)
            {
                bDoPps = 1;
                break;
            }
        }
    }

    /* Perform PPS if neccessary */
    if (bDoPps)
    {
        /* Init parameters of PPS */
        wCmd = PHBAL_RD710SAM_CMD_CC_SEND_PPS;
        wOffTxBuf = 0;
        pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd >> 8);
        pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd & 0xFF);
        pDataParams->pTxBuffer[wOffTxBuf++] = 0x03;
        pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;
        pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;
        pDataParams->pTxBuffer[wOffTxBuf++] = 0x11;
        pDataParams->pTxBuffer[wOffTxBuf++] = bPps1;

        /* Perform PPS */
        PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
            pDataParams->pLowerBalDataParams,
            PH_EXCHANGE_DEFAULT,
            pDataParams->pTxBuffer,
            wOffTxBuf,
            pDataParams->wRxBufSize,
            pDataParams->pRxBuffer,
            &wRxLength));

        /* Check response */
        PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Rd710Sam_CheckResponse(
            wCmd,
            pDataParams->pRxBuffer,
            wRxLength,
            NULL,
            &wRxLength));
    }

    /* Card is now open */
    pDataParams->bCardOpened = PH_ON;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_ClosePort(
                                       phbalReg_Rd710Sam_DataParams_t * pDataParams
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wCmd;
    uint16_t    PH_MEMLOC_REM wOffTxBuf;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* Port should be open */
    if (pDataParams->bCardOpened == PH_OFF)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_BAL);
    }

    /* Reset ATR length */
    pDataParams->pAtrBuffer[0] = 0x00;

    /* Init parameters of Activate Contact Card */
    wCmd = PHBAL_RD710SAM_CMD_DEACTIVATE_CONTACT_CARD;
    wOffTxBuf = 0;
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd >> 8);
    pDataParams->pTxBuffer[wOffTxBuf++] = (uint8_t)(wCmd & 0xFF);
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;
    pDataParams->pTxBuffer[wOffTxBuf++] = 0x00;

    /* Perform Activate Contact Card */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
        pDataParams->pLowerBalDataParams,
        PH_EXCHANGE_DEFAULT,
        pDataParams->pTxBuffer,
        wOffTxBuf,
        pDataParams->wRxBufSize,
        pDataParams->pRxBuffer,
        &wRxLength
        ));

    /* Check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Rd710Sam_CheckResponse(
        wCmd,
        pDataParams->pRxBuffer,
        wRxLength,
        NULL,
        &wRxLength));

    /* Payload length should be 0 */
    if (wRxLength != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_BAL);
    }

    /* Card is now closed */
    pDataParams->bCardOpened = PH_OFF;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_SetConfig(
                                       phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t wValue
                                       )
{
    /* satisfy compiler */
    if (pDataParams || wConfig || wValue);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Rd710Sam_GetConfig(
                                       phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t * pValue
                                       )
{
    /* satisfy compiler */
    if (pDataParams || wConfig || pValue);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_RD710SAM */
