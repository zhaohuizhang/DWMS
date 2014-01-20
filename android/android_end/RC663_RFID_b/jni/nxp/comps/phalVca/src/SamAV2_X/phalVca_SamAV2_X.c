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
* $Date: Fri Apr 15 09:26:54 2011 $
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

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X

#include "../phalVca_Int.h"
#include "phalVca_SamAV2_X.h"
#include "phalVca_SamAV2_X_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalVca_SamAV2_X_Init(
                                 phalVca_SamAV2_X_DataParams_t * pDataParams,  
                                 uint16_t wSizeOfDataParams, 
                                 phhalHw_SamAV2_DataParams_t * pSamHal,     
                                 phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams,
                                 phalVca_SamAV2_X_IidTable_t * pIidTable,
                                 phalVca_SamAV2_X_KeysTable_t * pKeysTable,
                                 uint16_t wNumIidKeysTableEntries,          
                                 phalVca_SamAV2_X_CardTableEntry_t * pCardTableStorage,
                                 uint16_t wNumCardTableStorageEntries
                                 )
{
    if (sizeof(phalVca_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);
	PH_ASSERT_NULL (pSamHal);
	PH_ASSERT_NULL (pIidTable);
	PH_ASSERT_NULL (pKeysTable);
	if (wNumCardTableStorageEntries) PH_ASSERT_NULL (pCardTableStorage);

    /* init private data */
    pDataParams->wId                        = PH_COMP_AL_VCA | PHAL_VCA_SAMAV2_X_ID;
    pDataParams->pPalMifareDataParams       = pPalMifareDataParams;
    pDataParams->pSamHal                    = pSamHal;
    pDataParams->pIidTable                  = pIidTable;
    pDataParams->pKeysTable                 = pKeysTable;
    pDataParams->wNumIidKeysTableEntries    = wNumIidKeysTableEntries;
    pDataParams->wCurrentIidKeysTablePos    = 0;
    pDataParams->wCurrentCardTablePos       = 0;
    pDataParams->pCardTable                 = pCardTableStorage;
    pDataParams->wNumCardTableEntries       = wNumCardTableStorageEntries;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_X_VcSupport(
                                      phalVca_SamAV2_X_DataParams_t * pDataParams, 
                                      uint8_t*    pIid,
                                      uint16_t    wKeyEncNumber,                           
                                      uint16_t    wKeyEncVersion,
                                      uint16_t    wKeyMacNumber,                     
                                      uint16_t    wKeyMacVersion
                                      )
{
    /* Check available space in key duos list */
    if (pDataParams->wCurrentIidKeysTablePos >= pDataParams->wNumIidKeysTableEntries)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Add keys to key list */
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[0] = (uint8_t)wKeyEncNumber;
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[1] = (uint8_t)wKeyEncVersion;
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[2] = (uint8_t)wKeyMacNumber;
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[3] = (uint8_t)wKeyMacVersion;

    memcpy(pDataParams->pIidTable[pDataParams->wCurrentIidKeysTablePos].pIid, pIid, 16);  /* PRQA S 3200 */
    ++pDataParams->wCurrentIidKeysTablePos;
    ++pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_X_VcSupportLast(
    phalVca_SamAV2_X_DataParams_t * pDataParams,
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
    uint8_t     PH_MEMLOC_REM bIndex;
    uint8_t *   PH_MEMLOC_REM pKeyDuosPointer;
    uint8_t *   PH_MEMLOC_REM pIidPointer;

    /* Copy PCD Caps */
    memset(pDataParams->pPcdCaps, 0x00, 4);  /* PRQA S 3200 */
    if (bLenCap)
    {
        if (bLenCap > 3)
        {
            bLenCap = 3;
        }
        pDataParams->pPcdCaps[0] = bLenCap;
        memcpy(&pDataParams->pPcdCaps[1], pPcdCapabilities, bLenCap);  /* PRQA S 3200 */
    }

    /* Check available space in iid list */
    if (pDataParams->wCurrentIidKeysTablePos >= pDataParams->wNumIidKeysTableEntries)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Check available space in card table list */
    if (pDataParams->wCurrentCardTablePos >= pDataParams->wNumCardTableEntries)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Add keys to key list */
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[0] = (uint8_t)wKeyEncNumber;
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[1] = (uint8_t)wKeyEncVersion;
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[2] = (uint8_t)wKeyMacNumber;
    pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos].pKeys[3] = (uint8_t)wKeyMacVersion;
    memcpy(pDataParams->pIidTable[pDataParams->wCurrentIidKeysTablePos].pIid, pIid, 16);  /* PRQA S 3200 */
    ++pDataParams->wCurrentIidKeysTablePos;
    ++pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos;

    /* Safety check */
    if (pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos > pDataParams->wCurrentIidKeysTablePos)
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_VCA);
    }

    /* Add Num key Duos */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
        pDataParams->pSamHal,
        PH_EXCHANGE_BUFFER_CONT,
        &pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos,
        1,
        NULL,
        0,
        NULL,
        NULL,
        NULL));

    pKeyDuosPointer = pDataParams->pKeysTable[pDataParams->wCurrentIidKeysTablePos - pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos].pKeys;
    pIidPointer = pDataParams->pIidTable[pDataParams->wCurrentIidKeysTablePos - pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos].pIid;

    for (bIndex = 0; bIndex < pDataParams->pCardTable[pDataParams->wCurrentCardTablePos].bNumKeyDuos; ++bIndex)
    {
        /* Add key Duos */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
            pDataParams->pSamHal,
            PH_EXCHANGE_BUFFER_CONT,
            pKeyDuosPointer,
            4,
            NULL,
            0,
            NULL,
            NULL,
            NULL));

        pKeyDuosPointer = pKeyDuosPointer + 4;

        /* Add Iid */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
            pDataParams->pSamHal,
            PH_EXCHANGE_BUFFER_CONT,
            pIidPointer,
            16,
            NULL,
            0,
            NULL,
            NULL,
            NULL));

        pIidPointer = pIidPointer + 16;
    }

    pDataParams->wCurrentCardTablePos++;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_X_StartCardSelection (
    phalVca_SamAV2_X_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIndex;

    /* Send the complete stream to the SAM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_FIRST,
        NULL,
        0,
        NULL,
        0,
        NULL,
        NULL,
        NULL));

    pDataParams->wCurrentCardTablePos = 0;
    pDataParams->wCurrentIidKeysTablePos = 0;

    for (bIndex = 0; bIndex <  pDataParams->wNumCardTableEntries; ++bIndex)
    {
        pDataParams->pCardTable[bIndex].bNumKeyDuos = 0;
        pDataParams->pCardTable[bIndex].wIidIndex = 0xFF;
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_X_FinalizeCardSelection(
                                       phalVca_SamAV2_X_DataParams_t * pDataParams, 
                                       uint16_t* pNumValidIids
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wIidIndex;
    uint8_t     PH_MEMLOC_REM bValidIids = 0;
    uint8_t     PH_MEMLOC_REM bDummyValidIids = 0;
    uint8_t     PH_MEMLOC_REM bDummyIidIndex = 0;
    uint8_t     PH_MEMLOC_REM bPiccReturnCode;

    /* Send the complete stream to the SAM */
    statusTmp = phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
        pDataParams->pSamHal, 
        PH_EXCHANGE_BUFFER_LAST,
        NULL,
        0,
        &pDataParams->pPcdCaps[1],
        pDataParams->pPcdCaps[0],
        &pResponse,
        &wResponseLength,
        &bPiccReturnCode);

    if (((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        (statusTmp != PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN, PH_COMP_HAL)))
    {
        return statusTmp;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(1, bPiccReturnCode));

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
        else
        {
            /* Invalidate the found IID index */
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

phStatus_t phalVca_SamAV2_X_GetIidInfo(
                                       phalVca_SamAV2_X_DataParams_t * pDataParams,
                                       uint16_t wValidIidIndex,
                                       uint16_t * pIidIndex,
                                       uint8_t * pVcUidSize,
                                       uint8_t * pVcUid,
                                       uint8_t * pInfo,
                                       uint8_t * pPdCapabilities
                                       )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_SamAV2_X_Int_ResolveValidIndex(
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

phStatus_t phalVca_SamAV2_X_SelectVc(
                                     phalVca_SamAV2_X_DataParams_t * pDataParams,
                                     uint16_t wValidIidIndex,
                                     uint16_t wKeyNumber,                      
                                     uint16_t wKeyVersion
                                     )
{
    /* local variables */
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bPiccReturnCode;

    status = phalVca_SamAV2_X_Int_ResolveValidIndex(
        pDataParams,
        wValidIidIndex,
        &wValidIidIndex);

    /* for the case of an overflow we always send the first VCIID number to the SAM */
    if ((status  & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
    {
        wValidIidIndex = 0;
    }
    else
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Prepare Buffer */
    status = phhalHw_SamAV2_Cmd_MfpSelectVirtualCard(
        pDataParams->pSamHal,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        pDataParams->pIidTable[pDataParams->pCardTable[wValidIidIndex].wIidIndex].pIid,
        NULL,
        0,
        &bPiccReturnCode);

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN))
    {
        return status;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(1, bPiccReturnCode));


    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_X_DeselectVc(
                                       phalVca_SamAV2_X_DataParams_t * pDataParams
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

phStatus_t phalVca_SamAV2_X_ProximityCheck(
    phalVca_SamAV2_X_DataParams_t * pDataParams,
    uint8_t     bGenerateRndC,
    uint8_t*    pRndC, 
    uint8_t     bPps1,
    uint8_t     bNumSteps,
    uint16_t    wKeyNumber,        
    uint16_t    wKeyVersion, 
    uint8_t*    pUsedRndC
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bReturnCode;

    /* Satisfy Compiler */
    if (bPps1 ||pUsedRndC || bGenerateRndC || pRndC);

    /* Recalculate bNumSteps to X-config */
    switch(bNumSteps)
    {
    case 1:
        bNumSteps = 0;
        break;
    case 2:
        bNumSteps = 4;
        break;
    case 3:
        bNumSteps = 3;
        break;
    case 4:
    case 5: /* Note - not correct translation */
        bNumSteps = 2;
        break;
    case 6: /* Note - not correct translation */
    case 7:
        bNumSteps = 1;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
    }

    status =  phhalHw_SamAV2_Cmd_MfpProximityCheck(
        pDataParams->pSamHal,
        PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK_DEFAULT,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        bNumSteps,
        NULL,
        0, 
        &bReturnCode);

    if ((status != PH_ERR_SUCCESS) &&( status != (PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN)))
    {
        /* Remap crypto error */
        if (status == (PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_CRYPTO))
        {   
            return PH_ADD_COMPCODE(PHAL_VCA_ERR_AUTH, PH_COMP_AL_VCA);
        }

        return status;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(1, bReturnCode));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_SamAV2_X_Int_ResolveValidIndex(
    phalVca_SamAV2_X_DataParams_t * pDataParams,
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

#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */
