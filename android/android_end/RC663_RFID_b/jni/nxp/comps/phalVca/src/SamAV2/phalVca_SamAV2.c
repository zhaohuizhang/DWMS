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
* SamAV2 Virtual Card Architecture(R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:03 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phalVca.h>
#include <phpalMifare.h>
#include <phKeyStore.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_VCA_SAMAV2

#include "../phalVca_Int.h"
#include "phalVca_SamAV2.h"
#include "phalVca_SamAV2_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalVca_SamAV2_Init(
                               phalVca_SamAV2_DataParams_t * pDataParams,  
                               uint16_t wSizeOfDataParams, 
                               phhalHw_SamAV2_DataParams_t * pSamHal,     
                               void * pPalMifareDataParams,
                               uint8_t * pRndq,
                               phalVca_SamAV2_KeyDuos_t * pKeyDuos,
                               uint8_t bNumKeyDuos,
                               phalVca_SamAV2_CardTableEntry_t * pCardTableStorage,
                               uint16_t wNumCardTableStorageEntries
                               )
{
    if (sizeof(phalVca_SamAV2_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);
	PH_ASSERT_NULL (pSamHal);
	PH_ASSERT_NULL (pRndq);
	PH_ASSERT_NULL (pKeyDuos);
	if (wNumCardTableStorageEntries) PH_ASSERT_NULL (pCardTableStorage);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_VCA | PHAL_VCA_SAMAV2_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pSamHal                = pSamHal;
    pDataParams->pRndq                  = pRndq;
    pDataParams->pKeyDuos               = pKeyDuos;
    pDataParams->bNumKeyDuos            = bNumKeyDuos;
    pDataParams->wCurrentCardTablePos   = 0;
    pDataParams->pCardTable             = pCardTableStorage;
    pDataParams->wNumCardTableEntries   = wNumCardTableStorageEntries;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_VcSupport(
                                    phalVca_SamAV2_DataParams_t * pDataParams, 
                                    uint8_t*    pIid,
                                    uint16_t    wKeyEncNumber,                           
                                    uint16_t    wKeyEncVersion,
                                    uint16_t    wKeyMacNumber,                     
                                    uint16_t    wKeyMacVersion
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1 /* command code */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint16_t    PH_MEMLOC_REM wIndex;

    /* send buffer */
    aCommand[0] = PHAL_VCA_CMD_VCS;

    /*Check available space in key duos list */
    if (pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos >= pDataParams->bNumKeyDuos)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Add keys to key list */
    wIndex = pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos;
    pDataParams->pKeyDuos[wIndex].bKeyEncNumber  = (uint8_t)wKeyEncNumber;
    pDataParams->pKeyDuos[wIndex].bKeyEncVersion = (uint8_t)wKeyEncVersion;
    pDataParams->pKeyDuos[wIndex].bKeyMacNumber  = (uint8_t)wKeyMacNumber;
    pDataParams->pKeyDuos[wIndex].bKeyMacVersion = (uint8_t)wKeyMacVersion;
    ++pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos;

    /* buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCommand,
        1,
        &pResponse,
        &wRxLength));

    /* Append IID and exchange the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pIid,
        PHAL_VCA_IID_SIZE,
        &pResponse,
        &wRxLength));

    /* check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));

    /* check response length */
    if (wRxLength != 1 /* STATUS */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_VcSupportLast(
                                        phalVca_SamAV2_DataParams_t * pDataParams,
                                        uint8_t*    pIid,
                                        uint8_t     bLenCap,
                                        uint8_t*    pPcdCapabilities, 
                                        uint16_t    wKeyEncNumber,               
                                        uint16_t    wKeyEncVersion, 
                                        uint16_t    wKeyMacNumber,                           
                                        uint16_t    wKeyMacVersion  
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint8_t     PH_MEMLOC_REM aCommand[1 /* CMD */];
    uint16_t    PH_MEMLOC_REM wIndex;

    /* Now we create the message to be sent */
    aCommand[0] = PHAL_VCA_CMD_VCSL;
    memset(pDataParams->pPcdCaps, 0x00, 4);  /* PRQA S 3200 */

    /* Copy PCD Caps */
    if (bLenCap)
    {
        if (bLenCap > 3)
        {
            bLenCap = 3;
        }
        pDataParams->pPcdCaps[0] = bLenCap;

        memcpy(&pDataParams->pPcdCaps[1], pPcdCapabilities, bLenCap);  /* PRQA S 3200 */
    }

    /* Check available space in key duos list */
    if (pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos >= pDataParams->bNumKeyDuos)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Add keys to key list */
    wIndex = pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos;
    pDataParams->pKeyDuos[wIndex].bKeyEncNumber  = (uint8_t)wKeyEncNumber;
    pDataParams->pKeyDuos[wIndex].bKeyEncVersion = (uint8_t)wKeyEncVersion;
    pDataParams->pKeyDuos[wIndex].bKeyMacNumber  = (uint8_t)wKeyMacNumber;
    pDataParams->pKeyDuos[wIndex].bKeyMacVersion = (uint8_t)wKeyMacVersion;
    ++pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos;

    /* buffer command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCommand,
        1,
        &pResponse,
        &wRxLength));

    /* buffer installation identifier */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pIid,
        PHAL_VCA_IID_SIZE,
        &pResponse,
        &wRxLength));

    /* buffer RNDQ identifier */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        &pDataParams->pRndq[pDataParams->wCurrentCardTablePos * 12],
        12,
        &pResponse,
        &wRxLength));

    /* append PCDCaps and transmit the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pDataParams->pPcdCaps,
        1+ bLenCap,
        &pResponse,
        &wRxLength));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));

    /* check response length */
    if (wRxLength != 1 /*Status */ + 16 /* Cryptogram */ + 8 /*MAC */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    /* Add Num key Duos */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_CONT,
        &pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos,
        1,
        NULL,
        0,
        NULL,
        NULL));

    /* Add key Duos */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_CONT,
        (uint8_t*)pDataParams->pKeyDuos,
        pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos << 2,
        NULL,
        0,
        NULL,
        NULL));

    /* Add RNDQ */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_CONT,
        &pDataParams->pRndq[pDataParams->wCurrentCardTablePos * 12],
        12,
        NULL,
        0,
        NULL,
        NULL));

    /* Add Card Data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_CONT,
        &pResponse[1],
        24,
        NULL,
        0,
        NULL,
        NULL));

    pDataParams->wCurrentCardTablePos++;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_StartCardSelection (
    phalVca_SamAV2_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM wIndex;

    /* Obtain Random Numbers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetChallenge(pDataParams->pSamHal, (uint8_t)(12 * pDataParams->wNumCardTableEntries), pDataParams->pRndq));

    /* Send the complete stream to the SAM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_FIRST,
        NULL,
        0,
        NULL,
        0,
        NULL,
        NULL));

    pDataParams->wCurrentCardTablePos = 0;
    for (wIndex = 0; wIndex <  pDataParams->wNumCardTableEntries; ++wIndex)
    {
        pDataParams->pCardTable[wIndex].bNumKeyDuos = 0;
        pDataParams->pCardTable[wIndex].wIidIndex = 0xFF;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_FinalizeCardSelection (
    phalVca_SamAV2_DataParams_t * pDataParams, 
    uint16_t* pNumValidIids
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wIidIndex;
    uint8_t     PH_MEMLOC_REM bValidIids = 0;
    uint8_t     PH_MEMLOC_REM bDummyIidIndex = 0;
    uint8_t     PH_MEMLOC_REM bDummyValidIids = 0;

    /* Check if we have something in the table */
    if (!pDataParams->wCurrentCardTablePos)
    {
        *pNumValidIids = 0;
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
    }

    /* Send the complete stream to the SAM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_LAST,
        NULL,
        0,
        &pDataParams->pPcdCaps[1],
        pDataParams->pPcdCaps[0],
        &pResponse,
        &wResponseLength));

    if (wResponseLength != pDataParams->wCurrentCardTablePos * 11)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_VCA);
    }

    wIidIndex = 0;

    /* Now we need to store the card data */
    for (wIndex = 0; wIndex < pDataParams->wCurrentCardTablePos; ++wIndex)
    {
        /* We always copy the data into the card table */
        memcpy(pDataParams->pCardTable[wIndex].pCardData, &pResponse[1], 10);  /* PRQA S 3200 */

        /* Was it a valid entry? */
        if (pResponse[0] != 0xFF)
        {
            /* Copy the found IID index */
            ++bValidIids;
            pDataParams->pCardTable[wIndex].wIidIndex = wIidIndex + pResponse[0];
        }
        /* Invalidate the found IID index */
        else
        {
            ++bDummyValidIids;
            pDataParams->pCardTable[wIndex].wIidIndex = bDummyIidIndex + pResponse[0];
        }

        /* Update the IidIndex */
        wIidIndex = wIidIndex + pDataParams->pCardTable[wIndex].bNumKeyDuos;
        pResponse += (10 /* Card Data */ + 1 /* Success index */);
    }

    *pNumValidIids = bValidIids;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_GetIidInfo(
                                     phalVca_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wValidIidIndex,
                                     uint16_t * pIidIndex,
                                     uint8_t * pVcUidSize,
                                     uint8_t * pVcUid,
                                     uint8_t * pInfo,
                                     uint8_t * pPdCapabilities
                                     )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_SamAV2_Int_ResolveValidIndex(
        pDataParams,
        wValidIidIndex,
        &wValidIidIndex));

    *pIidIndex = pDataParams->pCardTable[wValidIidIndex].wIidIndex;
    *pInfo = pDataParams->pCardTable[wValidIidIndex].pCardData[0];
    pPdCapabilities[0] = pDataParams->pCardTable[wValidIidIndex].pCardData[1];
    pPdCapabilities[1] = pDataParams->pCardTable[wValidIidIndex].pCardData[2];

    if (*pInfo & 0x80)
    {
        *pVcUidSize = 4;
        memcpy(pVcUid, &pDataParams->pCardTable[wValidIidIndex].pCardData[3], 4);  /* PRQA S 3200 */
    }
    else
    {
        *pVcUidSize = 7;
        memcpy(pVcUid, &pDataParams->pCardTable[wValidIidIndex].pCardData[3], 7);  /* PRQA S 3200 */
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_SelectVc(
                                   phalVca_SamAV2_DataParams_t * pDataParams,
                                   uint16_t    wValidIidIndex,
                                   uint16_t wKeyNumber,                      
                                   uint16_t wKeyVersion
                                   )
{
    /* local variables */
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[9];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    status = phalVca_SamAV2_Int_ResolveValidIndex(
        pDataParams,
        wValidIidIndex,
        &wValidIidIndex);

    /* for the case of an overflow we always send the first random number to the SAM */
    if ((status  & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
    {
        wValidIidIndex = 0;
    }
    else
    {
        PH_CHECK_SUCCESS(status);
    }

    aCommand[0] = PHAL_VCA_CMD_SVC;

    /* Prepare Buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_SelectVirtualCardMfp(
        pDataParams->pSamHal,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        &pDataParams->pRndq[wValidIidIndex * 12],
        NULL,
        0,
        &aCommand[1]));

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        9,
        &pResponse,
        &wRxLength));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));

    /* check response length */
    if (wRxLength != 1 /* STATUS */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_DeselectVc(
                                     phalVca_SamAV2_DataParams_t * pDataParams
                                     )
{
    /* local variables */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1 /* command code */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    aCommand[0] = PHAL_VCA_CMD_DVC;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        1,
        &pResponse,
        &wRxLength));

    /* check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));

    /* check response length */
    if (wRxLength != 1 /* STATUS */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_ProximityCheck(
    phalVca_SamAV2_DataParams_t * pDataParams,
    uint8_t     bGenerateRndC,
    uint8_t*    pRndC, 
    uint8_t     bPps1,
    uint8_t     bNumSteps,
    uint16_t    wKeyNumber,        
    uint16_t    wKeyVersion, 
    uint8_t*    pUsedRndC
    )
{
    /* local variables */
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[9];
    uint8_t     PH_MEMLOC_REM bRndRC[14];
    uint8_t     PH_MEMLOC_REM bRndC[7];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* parameter checking */
    if ((bGenerateRndC == 0) && (pRndC == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_VCA);
    }

    /* send "Prepare Proximity Check" command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_PrepareProximityCheck(pDataParams->pPalMifareDataParams));

    /* check whether to generate RndC or not */
    if (bGenerateRndC)
    {
        pRndC = bRndC;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetChallenge(pDataParams->pSamHal, PHAL_VCA_PC_RND_LEN, pRndC));
    }

    /* send "Proximity Check" command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ProximityCheck(
        pDataParams->pPalMifareDataParams,
        bNumSteps,
        pRndC,
        bRndRC));

    /* send "Verify Proximity Check" command */
    status = phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part1( pDataParams->pSamHal,
        (uint8_t) wKeyNumber,
        (uint8_t) wKeyVersion,
        bPps1,
        bRndRC,
        NULL,
        0,
        &aCommand[1]);

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        return status;
    }

    aCommand[0] = PHAL_VCA_CMD_VPC;

    /* append the MAC and exchange frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        9,
        &pResponse,
        &wRxLength));

    /* Check the status Code */
    status = phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]);
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* finish SAM chaining with KillAuthenticate command */
        /* Kill only card Auth */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pSamHal, 0x01);
        return status;
    }

    /* check response length */
    if (wRxLength != 1 /* Status */ + 8 /* MAC */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    /* send "Verify Proximity Check" command */
    status = phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part2( pDataParams->pSamHal, &pResponse[1]);

    if ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_CRYPTO)
    {
        return PH_ADD_COMPCODE(PHAL_VCA_ERR_AUTH, PH_COMP_AL_VCA);
    }

    PH_CHECK_SUCCESS(status);
    /* Copy RndC if requested */
    if (pUsedRndC != NULL)
    {
        memcpy(pUsedRndC, pRndC, PHAL_VCA_PC_RND_LEN);  /* PRQA S 3200 */
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_Int_ResolveValidIndex(
    phalVca_SamAV2_DataParams_t * pDataParams,
    uint16_t wIidIndex,
    uint16_t * pValidIndex
    )
{
    uint8_t     PH_MEMLOC_REM bCurrentValidIndex = 0;
    uint8_t     PH_MEMLOC_REM bDummyValidIndex = 0;
    uint8_t     PH_MEMLOC_REM bDummyFoundIndex;
    uint8_t     PH_MEMLOC_REM bIndex;
    uint16_t    PH_MEMLOC_REM bFoundIndex = pDataParams->wCurrentCardTablePos;

    for (bIndex = 0; bIndex < pDataParams->wCurrentCardTablePos; ++bIndex)
    {
        if (pDataParams->pCardTable[bIndex].wIidIndex == 0xFF)
        {
            if (bCurrentValidIndex == wIidIndex)
            {
                bDummyFoundIndex = bIndex;
            }
            else
            {
                bDummyFoundIndex = bIndex;
            }
            bDummyValidIndex++;
        }
        else
        {
            if (bCurrentValidIndex == wIidIndex)
            {
                bFoundIndex = bIndex;
            }
            else
            {
                bDummyFoundIndex = bIndex;
            }
            bCurrentValidIndex++;
        }        
    }

    /* Check if the index is valid */
    if (bFoundIndex >= pDataParams->wCurrentCardTablePos)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
    }

    *pValidIndex = bFoundIndex;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */
