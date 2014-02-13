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
* SamAV2_X MIFARE(R) Ultralight Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:28:02 2011 $
*
* History:
*  CHu: Generated 25. November 2009
*
*/

#include <ph_Status.h>
#include <phalMful.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFUL_SAMAV2_X

#include "phalMful_SamAV2_X.h"
#include "../phalMful_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalMful_SamAV2_X_Init(
                                  phalMful_SamAV2_X_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams, 
                                  phhalHw_SamAV2_DataParams_t * pHalSamDataParams,
                                  phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams
                                  )
{
    if (sizeof(phalMful_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFUL);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pHalSamDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFUL | PHAL_MFUL_SAMAV2_X_ID;
    pDataParams->pHalSamDataParams      = pHalSamDataParams;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFUL);
}

phStatus_t phalMful_SamAV2_X_UlcAuthenticate(
    phalMful_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion
    )
{
    return phhalHw_SamAV2_Cmd_ULCAuthenticatePICC(
        pDataParams->pHalSamDataParams,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        NULL,
        0);
}

phStatus_t phalMful_SamAV2_X_Read(
                                  phalMful_SamAV2_X_DataParams_t * pDataParams,
                                  uint8_t bAddress,
                                  uint8_t * pData
                                  )
{
    /* perform read operation */
    uint8_t *  PH_MEMLOC_REM pRxBuffer;
    uint16_t   PH_MEMLOC_REM wRxBufferLength;
    phStatus_t PH_MEMLOC_REM statusTmp;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfRead(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_SINGLEBLOCK,
        &bAddress, 
        1, 
        &pRxBuffer,
        &wRxBufferLength
        ));

    memcpy(pData, pRxBuffer, wRxBufferLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFUL);
}

phStatus_t phalMful_SamAV2_X_Write(
                                   phalMful_SamAV2_X_DataParams_t * pDataParams,
                                   uint8_t bAddress,
                                   uint8_t * pData
                                   )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* perform the actual write operation */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT,
        &bAddress,
        1));

    return phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_LAST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT,
        pData,
        16);
}

phStatus_t phalMful_SamAV2_X_CompatibilityWrite(
    phalMful_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bAddress,
    uint8_t * pData
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* perform the actual write operation */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MFULTRALIGTH,
        &bAddress,
        1));

    return phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_LAST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MFULTRALIGTH,
        pData,
        4);
}

#endif /* NXPBUILD__PHAL_MFUL_SAMAV2_X */
