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
* SamAV2 MIFARE(R) Ultralight Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:04:54 2011 $
*
* History:
*  CHu: Generated 25. November 2009
*
*/

#include <ph_Status.h>
#include <phalMful.h>
#include <phpalMifare.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFUL_SAMAV2

#include "phalMful_SamAV2.h"
#include "../phalMful_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalMful_SamAV2_Init(
                                phalMful_SamAV2_DataParams_t * pDataParams,
                                uint16_t wSizeOfDataParams, 
                                phhalHw_SamAV2_DataParams_t * pHalSamDataParams,
                                void * pPalMifareDataParams
                                )
{
    if (sizeof(phalMful_SamAV2_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFUL);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pHalSamDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFUL | PHAL_MFUL_SAMAV2_ID;
    pDataParams->pHalSamDataParams      = pHalSamDataParams;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFUL);
}

phStatus_t phalMful_SamAV2_UlcAuthenticate(
    phalMful_SamAV2_DataParams_t * pDataParams,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bFrame[2];
    uint8_t     PH_MEMLOC_REM bEncRndAB[PHAL_MFUL_DES_BLOCK_SIZE * 2];
    uint8_t     PH_MEMLOC_REM bEncRndABLength;
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* build the authentication request */
    bFrame[0] = PHAL_MFUL_CMD_AUTH;
    bFrame[1] = 0x00;

    /* transmit the request */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bFrame,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* check the format of the received data */
    if ((wRxLength != PHAL_MFUL_DES_BLOCK_SIZE+1) ||
        (pRxBuffer[0] != PHAL_MFUL_PREAMBLE_TX))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* Let the SAM process the response and retrieve Enc(RndAB) */
    status = phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part1(
        pDataParams->pHalSamDataParams,
        0x00,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        &pRxBuffer[1],
        PHAL_MFUL_DES_BLOCK_SIZE,
        NULL,
        0,
        bEncRndAB,
        &bEncRndABLength);

    /* status should be chaining active */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* buffer the preamble */
    bFrame[0] = PHAL_MFUL_PREAMBLE_TX;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bFrame,
        1,
        &pRxBuffer,
        &wRxLength
        ));

    /* append RndAB and send the stream */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bEncRndAB,
        bEncRndABLength,
        &pRxBuffer,
        &wRxLength
        ));

    /* check the format of the received data */
    if ((wRxLength != PHAL_MFUL_DES_BLOCK_SIZE+1) ||
        (pRxBuffer[0] != PHAL_MFUL_PREAMBLE_RX))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* Let the SAM process the response and retrieve Enc(RndAB) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part2(
        pDataParams->pHalSamDataParams,
        &pRxBuffer[1],
        PHAL_MFUL_DES_BLOCK_SIZE));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFUL);
}

phStatus_t phalMful_SamAV2_Read(
                                phalMful_SamAV2_DataParams_t * pDataParams,
                                uint8_t bAddress,
                                uint8_t * pData
                                )
{
    return phalMful_Int_Read(pDataParams->pPalMifareDataParams, bAddress, pData);
}

phStatus_t phalMful_SamAV2_Write(
                                 phalMful_SamAV2_DataParams_t * pDataParams,
                                 uint8_t bAddress,
                                 uint8_t * pData
                                 )
{
    return phalMful_Int_Write(pDataParams->pPalMifareDataParams, bAddress, pData);
}

phStatus_t phalMful_SamAV2_CompatibilityWrite(
    phalMful_SamAV2_DataParams_t * pDataParams,
    uint8_t bAddress,
    uint8_t * pData
    )
{
    return phalMful_Int_CompatibilityWrite(pDataParams->pPalMifareDataParams, bAddress, pData);
}

#endif /* NXPBUILD__PHAL_MFUL_SAMAV2 */
