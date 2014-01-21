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
* Software ISO14443-4A Component of Basic Function Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:48 2011 $
*
* History:
*  CHu: Generated 29. May 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p4a.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I14443P4A_SAMAV2_X

#include "phpalI14443p4a_SamAV2_X.h"
#include "phpalI14443p4a_SamAV2_X_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phpalI14443p4a_SamAV2_X_Init(
                                        phpalI14443p4a_SamAV2_X_DataParams_t * pDataParams,
                                        uint16_t wSizeOfDataParams,
                                        phhalHw_SamAV2_DataParams_t * pHalDataParams
                                        )
{
    if (sizeof(phpalI14443p4a_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P4A);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId            = PH_COMP_PAL_ISO14443P4A | PHPAL_I14443P4A_SAMAV2_X_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->bCidSupported  = PH_OFF;
    pDataParams->bNadSupported  = PH_OFF;
    pDataParams->bCid           = 0x00;
    pDataParams->bBitRateCaps   = 0x00;
    pDataParams->bFwi           = PHPAL_I14443P4A_SAMAV2_X_FWI_DEFAULT;
    pDataParams->bFsci          = PHPAL_I14443P4A_SAMAV2_X_FSCI_DEFAULT;
    pDataParams->bDri           = 0x00;
    pDataParams->bDsi           = 0x00;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4A);
}


phStatus_t phpalI14443p4a_SamAV2_X_Rats(
                                        phpalI14443p4a_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t bFsdi,
                                        uint8_t bCid,
                                        uint8_t * pAts
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Parameter check */
    if ((bFsdi > 8) || (bCid > 14))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Set default protocol settings */
    pDataParams->bCidSupported  = PH_ON;
    pDataParams->bCid           = bCid;
    pDataParams->bNadSupported  = PH_OFF;
    pDataParams->bBitRateCaps   = 0x00;
    pDataParams->bFwi           = 0x04;
    pDataParams->bDri           = 0x00;
    pDataParams->bDsi           = 0x00;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_4_RATS_PPS(
        pDataParams->pHalDataParams,
        pDataParams->bCid,
        pDataParams->bDsi,
        pDataParams->bDri,
        &pDataParams->bCid,
        &pDataParams->bDsi,
        &pDataParams->bDri,
        pAts));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4A);
}

phStatus_t phpalI14443p4a_SamAV2_X_Pps(
                                       phpalI14443p4a_SamAV2_X_DataParams_t * pDataParams,
                                       uint8_t bDri,
                                       uint8_t bDsi
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM cmd[3];
    uint16_t    PH_MEMLOC_REM wRespLength;
    uint16_t    PH_MEMLOC_REM wTxDataRate;
    uint16_t    PH_MEMLOC_REM wRxDataRate;
    uint8_t     PH_MEMLOC_REM * pResponse;

    wRespLength = 0;

    /* Check and convert Dri value */
    switch (bDri)
    {
    case PHPAL_I14443P4A_DATARATE_106:
        wTxDataRate = PHHAL_HW_RF_DATARATE_106;
        break;
    case PHPAL_I14443P4A_DATARATE_212:
        wTxDataRate = PHHAL_HW_RF_DATARATE_212;
        break;
    case PHPAL_I14443P4A_DATARATE_424:
        wTxDataRate = PHHAL_HW_RF_DATARATE_424;
        break;
    case PHPAL_I14443P4A_DATARATE_848:
        wTxDataRate = PHHAL_HW_RF_DATARATE_848;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Check and convert Dsi value */
    switch (bDsi)
    {
    case PHPAL_I14443P4A_DATARATE_106:
        wRxDataRate = PHHAL_HW_RF_DATARATE_106;
        break;
    case PHPAL_I14443P4A_DATARATE_212:
        wRxDataRate = PHHAL_HW_RF_DATARATE_212;
        break;
    case PHPAL_I14443P4A_DATARATE_424:
        wRxDataRate = PHHAL_HW_RF_DATARATE_424;
        break;
    case PHPAL_I14443P4A_DATARATE_848:
        wRxDataRate = PHHAL_HW_RF_DATARATE_848;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Send Pps command */
    cmd[0] = PHPAL_I14443P4A_SAMAV2_X_PPSS | pDataParams->bCid;
    cmd[1] = PHPAL_I14443P4A_SAMAV2_X_PPS0;
    cmd[2] = ((bDsi << 2) & 0x0C) | (bDri & 0x03);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        cmd,
        3,
        &pResponse,
        &wRespLength));

    /* Check length and start byte of PPS */
    if ((wRespLength != 1) || (pResponse[0] != (PHPAL_I14443P4A_SAMAV2_X_PPSS | pDataParams->bCid)))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
    }

    /* Store data rates */
    pDataParams->bDri = bDri;
    pDataParams->bDsi = bDsi;

    /* Apply new data rates to Pcd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXDATARATE, wTxDataRate));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXDATARATE, wRxDataRate));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4A);
}

phStatus_t phpalI14443p4a_SamAV2_X_ActivateCard(
    phpalI14443p4a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bFsdi,
    uint8_t bCid,
    uint8_t bDri,
    uint8_t bDsi,
    uint8_t * pAts
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bAtsIndex;
    uint8_t     PH_MEMLOC_REM bRespLength;
    
    /* Parameter check */
    if ((bFsdi != 8) || (bCid > 14))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Check Dri value */
    switch (bDri)
    {
    case PHPAL_I14443P4A_DATARATE_106:
    case PHPAL_I14443P4A_DATARATE_212:
    case PHPAL_I14443P4A_DATARATE_424:
    case PHPAL_I14443P4A_DATARATE_848:
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Check Dri value */
    switch (bDsi)
    {
    case PHPAL_I14443P4A_DATARATE_106:
    case PHPAL_I14443P4A_DATARATE_212:
    case PHPAL_I14443P4A_DATARATE_424:
    case PHPAL_I14443P4A_DATARATE_848:
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Set default values */
    /* */
    pDataParams->bCidSupported  = PH_ON;
    pDataParams->bCid           = bCid;
    pDataParams->bNadSupported  = PH_OFF;
    pDataParams->bBitRateCaps   = 0x00;
    pDataParams->bFwi           = PHPAL_I14443P4A_SAMAV2_X_FWI_DEFAULT;
    pDataParams->bFsci          = PHPAL_I14443P4A_SAMAV2_X_FSCI_DEFAULT;
    pDataParams->bDri           = 0x00;
    pDataParams->bDsi           = 0x00;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_4_RATS_PPS(
        pDataParams->pHalDataParams,
        bCid,
        bDsi,
        bDri,
        &pDataParams->bCid,
        &pDataParams->bDsi,
        &pDataParams->bDri,
        pAts));

    /* Retrieve ATS information */
    /* Start parsing with T0 byte */
    bRespLength = pAts[PHPAL_I14443P4A_SAMAV2_X_ATS_LEN];
    bAtsIndex   = PHPAL_I14443P4A_SAMAV2_X_ATS_T0;

    /* Parse T0 */
    pDataParams->bFsci = pAts[bAtsIndex] & 0x0F;
    if (pDataParams->bFsci > 8)
    {
        pDataParams->bFsci = 8;
    }
    ++bAtsIndex;

    /* Parse TA(1) */
    if (pAts[PHPAL_I14443P4A_SAMAV2_X_ATS_T0] & PHPAL_I14443P4A_SAMAV2_X_ATS_TA1_PRESENT)
    {
        /* Check for protocol error */
        if (bRespLength <= bAtsIndex)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
        }
        else
        {
            /* Store Bitrate capabilities */
            pDataParams->bBitRateCaps = pAts[bAtsIndex];

            ++bAtsIndex;
        }
    }

    /* Parse TB(1) */
    if (pAts[PHPAL_I14443P4A_SAMAV2_X_ATS_T0] & PHPAL_I14443P4A_SAMAV2_X_ATS_TB1_PRESENT)
    {
        /* Check for protocol error */
        if (bRespLength <= bAtsIndex)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
        }
        else
        {
            /* Store Fwi */
            pDataParams->bFwi = (pAts[bAtsIndex] >> 4) & 0x0F;
            if (pDataParams->bFwi == 0x0F)
            {
                pDataParams->bFwi = PHPAL_I14443P4A_SAMAV2_X_FWI_DEFAULT;
            }

            ++bAtsIndex;
        }
    }

    /* Parse TC(1) */
    if (pAts[PHPAL_I14443P4A_SAMAV2_X_ATS_T0] & PHPAL_I14443P4A_SAMAV2_X_ATS_TC1_PRESENT)
    {
        /* Check for protocol error */
        if (bRespLength <= bAtsIndex)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
        }
        else
        {
            /* Check NAD Support */
            if (pAts[bAtsIndex] & PHPAL_I14443P4A_SAMAV2_X_ATS_TC1_NAD_SUPPORT)
            {
                pDataParams->bNadSupported = 1;
            }

            /* Check CID Support */
            if (!(pAts[bAtsIndex] & PHPAL_I14443P4A_SAMAV2_X_ATS_TC1_CID_SUPPORT))
            {
                pDataParams->bCidSupported = 0;
                pDataParams->bCid = 0;
            }
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4A);
}

phStatus_t phpalI14443p4a_SamAV2_X_GetProtocolParams(
    phpalI14443p4a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pCidEnabled,
    uint8_t * pCid,
    uint8_t * pNadSupported,
    uint8_t * pFwi,
    uint8_t * pFsdi,
    uint8_t * pFsci
    )
{
    *pCidEnabled    = pDataParams->bCidSupported;
    *pCid           = pDataParams->bCid;
    *pNadSupported  = pDataParams->bNadSupported;
    *pFwi           = pDataParams->bFwi;
    *pFsdi          = 8;
    *pFsci          = pDataParams->bFsci;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P4A);
}

#endif /* NXPBUILD__PHPAL_I14443P4A_SAMAV2_X */
