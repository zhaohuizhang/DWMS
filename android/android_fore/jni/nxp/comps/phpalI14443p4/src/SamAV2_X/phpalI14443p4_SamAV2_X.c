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
* Software ISO14443-4 Component of Basic Function Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.6 $
* $Date: Fri Apr 15 09:27:49 2011 $
*
* History:
*  CHu: Generated 29. May 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p4.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I14443P4_SAMAV2_X

#include "phpalI14443p4_SamAV2_X.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phpalI14443p4_SamAV2_X_Init(
                                       phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
                                       uint16_t wSizeOfDataParams,
                                       phhalHw_SamAV2_DataParams_t * pHalDataParams
                                       )
{
    if (sizeof(phpalI14443p4_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P4);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pHalDataParams);

    /* Init private data */
    pDataParams->wId            = PH_COMP_PAL_ISO14443P4 | PHPAL_I14443P4_SAMAV2_X_ID;
    pDataParams->pHalDataParams = pHalDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4);
}

phStatus_t phpalI14443p4_SamAV2_X_SetProtocol(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint8_t   bCidEnable,
    uint8_t   bCid,
    uint8_t   bNadEnable,
    uint8_t   bNad,
    uint8_t   bFwi,
    uint8_t   bFsdi,
    uint8_t   bFsci
    )
{
    /* satisfy compiler */
    if (pDataParams || bCidEnable || bCid || bNadEnable || bNad || bFwi || bFsdi || bFsci);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_ISO14443P4);
}

phStatus_t phpalI14443p4_SamAV2_X_ResetProtocol(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams
    )
{
    /* satisfy compiler */
    if (pDataParams);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_ISO14443P4);
}

phStatus_t phpalI14443p4_SamAV2_X_Deselect(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams
    )
{
    return phhalHw_SamAV2_Cmd_X_ISO14443_4_Deselect(pDataParams->pHalDataParams ,PHHAL_HW_SAMAV2_CMD_EMPTY_CID);
}

phStatus_t phpalI14443p4_SamAV2_X_PresCheck(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams
    )
{
    return phhalHw_SamAV2_Cmd_X_ISO14443_4_PresenceCheck(pDataParams->pHalDataParams);
}

phStatus_t phpalI14443p4_SamAV2_X_Exchange(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pRxBufferTmp;
    uint16_t    PH_MEMLOC_REM wRxLengthTmp;
    uint16_t    PH_MEMLOC_REM wTxLengthTmp;
    uint16_t    PH_MEMLOC_REM wOptionSam;
    uint16_t    PH_MEMLOC_REM wRxBufferSize;
    uint16_t    PH_MEMLOC_REM wRxBufferStartPos;

	uint16_t	PH_MEMLOC_REM RxLength;
	uint8_t *	PH_MEMLOC_REM pRxBuffer;

	/* Check if caller has provided valid RxBuffer */
	if (ppRxBuffer == NULL)
    {
		ppRxBuffer = &pRxBuffer;
	}
	if (pRxLength == NULL)
	{
		pRxLength = &RxLength;
    }

    /* Reset receive length */
    *pRxLength = 0;

    /* Tx-Chaining loop */
    if (!(wOption & PH_EXCHANGE_RXCHAINING))
    {
        do
        {
            /* Check if chaining is neccessary */
            if (wOption & PH_EXCHANGE_TXCHAINING)
            {
                wTxLengthTmp = wTxLength;
                wOptionSam = PH_EXCHANGE_TXCHAINING;
            }
            else if (wTxLength > PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_APP_DATA_MAX)
            {
                wTxLengthTmp = PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_APP_DATA_MAX;
                wOptionSam = PH_EXCHANGE_TXCHAINING;
            }
            else
            {
                wTxLengthTmp = wTxLength;
                wOptionSam = PH_EXCHANGE_DEFAULT;
            }

            /* Perform actual exchange */
            status = phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange(
                pDataParams->pHalDataParams,
                wOptionSam,
                pTxBuffer,
                (uint8_t)wTxLength,
                &pRxBufferTmp,
                &wRxLengthTmp);

            /* Remove already transmitted bytes */
            pTxBuffer += wTxLengthTmp;
            wTxLength = wTxLength - wTxLengthTmp;
        }
        while (wTxLength > 0);

        /* Tx-Chaining was intended -> return successful */
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            *ppRxBuffer = NULL;
            *pRxLength = 0;
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4); 
        }
        /* Bail out if there's nothing to chain */
        else
        {
            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                /* Return received contents */
                *ppRxBuffer = pRxBufferTmp;
                *pRxLength = wRxLengthTmp;
                return status;
            }
        }
    }

    /* Retrieve receive buffer size */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
        &wRxBufferSize));

    /* Retrieve receive buffer start position */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        &wRxBufferStartPos));

    /* Rx-Chaining Loop */
    do
    {
        /* Rx-Chaining: This is the first chain, so do not check buffer size */
        if (wOption & PH_EXCHANGE_RXCHAINING)
        {
            wOption &= (uint16_t)~(uint16_t)PH_EXCHANGE_RXCHAINING;
        }
        else
        {
            /* Calculate free space within the receive buffer */
            wRxBufferSize = wRxBufferSize - wRxBufferStartPos;

            /* Bail out if we do not have enough buffer space for next exchange */
            if (wRxBufferSize < PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_RESPONSE_MAX)
            {
                return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_PAL_ISO14443P4); 
            }

            /* Update receive buffer start position */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wRxBufferStartPos));
        }

        /* Exchange with lower layer */
        status = phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            NULL,
            0,
            ppRxBuffer,
            pRxLength);

        /* Update receive buffer start position */
        wRxBufferStartPos = *pRxLength;
    }
    while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING);

    return status;
}

phStatus_t phpalI14443p4_SamAV2_X_SetConfig(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    )
{
    /* satisfy compiler */
    if (pDataParams || wConfig || wValue);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_ISO14443P4);
}

phStatus_t phpalI14443p4_SamAV2_X_GetConfig(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
    )
{
    /* satisfy compiler */
    if (pDataParams || wConfig || pValue);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_ISO14443P4);
}

#endif /* NXPBUILD__PHPAL_I14443P4_SAMAV2_X */
