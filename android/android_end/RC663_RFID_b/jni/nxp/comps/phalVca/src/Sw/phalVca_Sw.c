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
* Software MIFARE(R) Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.8 $
* $Date: Fri Apr 15 09:27:26 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <phhalHw.h>
#include <phalVca.h>
#include <phpalMifare.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <phKeyStore.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_VCA_SW

#include "../phalVca_Int.h"
#include "phalVca_Sw_Int.h"
#include "phalVca_Sw.h"

/*
* Private constants
*/
static const uint8_t PH_MEMLOC_CONST_ROM phalVca_Sw_FirstIv[PH_CRYPTOSYM_AES_BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

phStatus_t phalVca_Sw_Init(
                           phalVca_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams, 
                           void * pPalMifareDataParams,
                           void * pKeyStoreDataParams,
                           void * pCryptoDataParams,
                           void * pCryptoRngDataParams,
                           phalVca_Sw_IidTableEntry_t * pIidTableStorage,
                           uint16_t wNumIidTableStorageEntries,
                           phalVca_Sw_CardTableEntry_t * pCardTableStorage,
                           uint16_t wNumCardTableStorageEntries
                           )
{
    if (sizeof(phalVca_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);
	PH_ASSERT_NULL (pKeyStoreDataParams);
	PH_ASSERT_NULL (pCryptoDataParams);
	PH_ASSERT_NULL (pCryptoRngDataParams);
	if (wNumIidTableStorageEntries) PH_ASSERT_NULL (pIidTableStorage);
	if (wNumCardTableStorageEntries) PH_ASSERT_NULL (pCardTableStorage);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_VCA | PHAL_VCA_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;
    pDataParams->pCryptoDataParams      = pCryptoDataParams;
    pDataParams->wCurrentCardTablePos   = 0;
    pDataParams->pCardTable             = pCardTableStorage;
    pDataParams->wNumCardTableEntries   = wNumCardTableStorageEntries;
    pDataParams->pIidTable              = pIidTableStorage;
    pDataParams->wCurrentIidTablePos    = 0;
    pDataParams->wNumIidTableEntries    = wNumIidTableStorageEntries;
    pDataParams->pCryptoRngDataParams   = pCryptoRngDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Sw_VcSupport(
                                phalVca_Sw_DataParams_t * pDataParams,
                                uint8_t * pIid,
                                uint16_t wKeyEncNumber,
                                uint16_t wKeyEncVersion,
                                uint16_t wKeyMacNumber,
                                uint16_t wKeyMacVersion
                                )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aTxBuffer[1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* Check available space in key duos list */
    if (pDataParams->wCurrentIidTablePos >= pDataParams->wNumIidTableEntries )
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Add keys and iids to the iid table list */
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wIidIndex = pDataParams->wCurrentIidIndex;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyEncNumber = wKeyEncNumber;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyEncVersion = wKeyEncVersion;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyMacNumber = wKeyMacNumber;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyMacVersion = wKeyMacVersion;
    pDataParams->wCurrentIidTablePos++;
    pDataParams->wCurrentIidIndex++;

    /* command code */
    aTxBuffer[0] = PHAL_VCA_CMD_VCS;

    /* buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aTxBuffer,
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

phStatus_t phalVca_Sw_VcSupportLast(
                                    phalVca_Sw_DataParams_t * pDataParams,
                                    uint8_t * pIid,
                                    uint8_t bLenCap,
                                    uint8_t * pPcdCapabilities,
                                    uint16_t wKeyEncNumber,
                                    uint16_t wKeyEncVersion,
                                    uint16_t wKeyMacNumber,
                                    uint16_t wKeyMacVersion
                                    )
{
    phStatus_t                      PH_MEMLOC_REM statusTmp;
    uint8_t                         PH_MEMLOC_REM aTmpBuf[PH_CRYPTOSYM_AES_BLOCK_SIZE * 2];
    uint8_t                         PH_MEMLOC_REM aTxBuffer[1];
    uint8_t *                       PH_MEMLOC_REM pResponse;
    uint16_t                        PH_MEMLOC_REM wRxLength;
    uint8_t                         PH_MEMLOC_REM aKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint16_t                        PH_MEMLOC_REM wKeyType;
    uint8_t                         PH_MEMLOC_REM bMac[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t                         PH_MEMLOC_REM bMacLength;
    phalVca_Sw_CardTableEntry_t     PH_MEMLOC_REM pDummyCardData;
    phalVca_Sw_CardTableEntry_t *   PH_MEMLOC_REM pCardDataStorage;
    uint16_t                        PH_MEMLOC_REM wIndex;

    /* Build the command frame */
    aTxBuffer[0] = PHAL_VCA_CMD_VCSL;

    /* Copy PCD Caps */
    memset(aTmpBuf, 0x00, 4);  /* PRQA S 3200 */
    if (bLenCap)
    {
        if (bLenCap > 3)
        {
            bLenCap = 3;
        }
        aTmpBuf[0] = bLenCap;
        memcpy(&aTmpBuf[1], pPcdCapabilities, bLenCap);  /* PRQA S 3200 */
    }

    /* Check available space in key duos list */
    if (pDataParams->wCurrentIidTablePos >= pDataParams->wNumIidTableEntries)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Check available space in card table */
    if (pDataParams->wCurrentCardTablePos >= pDataParams->wNumCardTableEntries)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_VCA);
    }

    /* Add keys and iids to the iid table list */
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wIidIndex = pDataParams->wCurrentIidIndex;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyEncNumber = wKeyEncNumber;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyEncVersion = wKeyEncVersion;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyMacNumber = wKeyMacNumber;
    pDataParams->pIidTable[pDataParams->wCurrentIidTablePos].wKeyMacVersion = wKeyMacVersion;
    pDataParams->wCurrentIidTablePos++;
    pDataParams->wCurrentIidIndex++;

    /* Generate RNDQ */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, 12, &aTmpBuf[PHAL_VCA_POS_RNDQ]));

    /* buffer command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aTxBuffer,
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
        &aTmpBuf[PHAL_VCA_POS_RNDQ],
        12,
        &pResponse,
        &wRxLength));

    /* append PCDCaps and transmit the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        &aTmpBuf[0],
        1+ bLenCap,
        &pResponse,
        &wRxLength));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));

    /* check response length */
    if (wRxLength != 1 /*Status */ + 16 /* Cryptogram */ + 8 /*MAC */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    /* Prepare MAC data */
    aTmpBuf[0] = pResponse[0];
    memcpy(&aTmpBuf[PHAL_VCA_POS_PAYLOAD], &pResponse[1], 16);  /* PRQA S 3200 */

    /* Iterate over all available key pairs and try to find a match */
    for (wIndex = pDataParams->wCurrentIidTablePos; wIndex > 0; --wIndex)
    {
        /* Retrieve MAC key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
            pDataParams->pKeyStoreDataParams,
            pDataParams->pIidTable[(wIndex-1)].wKeyMacNumber,
            pDataParams->pIidTable[(wIndex-1)].wKeyMacVersion,
            sizeof(aKey),
            aKey,
            &wKeyType));

        /* Check key type */
        if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
        }

        /* Load the key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParams,
            aKey,
            PH_CRYPTOSYM_KEY_TYPE_AES128));

        /* Set the correct MAC calculation mode */
        /* CMAC with Padding */
        /* Calculate the MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParams,
            PH_CRYPTOSYM_MAC_MODE_CMAC,
            aTmpBuf,
            32,
            bMac,
            &bMacLength));

        /* Truncate the MAC */
        phalVca_Sw_Int_TruncateMac(bMac, bMac);

        /* Compare the MACs */
        if (memcmp(&pResponse[17], bMac, PHAL_VCA_TRUNCATED_MAC_SIZE) == 0)
        {
            pCardDataStorage = &pDataParams->pCardTable[pDataParams->wCurrentCardTablePos];            
        }
        else
        {
            pCardDataStorage = &pDummyCardData;
        }

        /* In any case, we need to decrypt */
        pCardDataStorage->bValid = PHAL_VCA_CARD_TABLE_ENTRY_VALID;

        /* Retrieve ENC key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
            pDataParams->pKeyStoreDataParams,
            pDataParams->pIidTable[(wIndex-1)].wKeyEncNumber,
            pDataParams->pIidTable[(wIndex-1)].wKeyEncVersion,
            sizeof(aKey),
            aKey,
            &wKeyType));

        /* Check key type */
        if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
        }

        /* Load the key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParams,
            aKey,
            PH_CRYPTOSYM_KEY_TYPE_AES128));

        /* Load first IV*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParams,
            phalVca_Sw_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* Perform decryption */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParams,
            PH_CRYPTOSYM_CIPHER_MODE_CBC,
            &pResponse[1],
            PHAL_VCA_IID_SIZE,
            pCardDataStorage->pCardData
            ));

        /* Copy the found IID Index */
        pCardDataStorage->wIidIndex = pDataParams->pIidTable[(wIndex-1)].wIidIndex;
    }

    pDataParams->wCurrentIidTablePos = 0;
    ++pDataParams->wCurrentCardTablePos;    

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Sw_StartCardSelection (
    phalVca_Sw_DataParams_t * pDataParams
    )
{
    uint16_t PH_MEMLOC_REM wIndex;

    for (wIndex=0; wIndex < pDataParams->wNumCardTableEntries; ++wIndex)
    {
        pDataParams->pCardTable[wIndex].bValid = PHAL_VCA_CARD_TABLE_ENTRY_INVALID;
    }

    pDataParams->wCurrentIidIndex       = 0;
    pDataParams->wCurrentCardTablePos   = 0;
    pDataParams->wCurrentIidTablePos    = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Sw_FinalizeCardSelection (
    phalVca_Sw_DataParams_t * pDataParams,
    uint16_t * pNumValidIids
    )
{
    uint16_t PH_MEMLOC_REM wDummyValidIids = 0;
    uint16_t PH_MEMLOC_REM wValidIids = 0;
    uint16_t PH_MEMLOC_REM wIndex;

    for (wIndex = 0; wIndex < pDataParams->wNumCardTableEntries; ++wIndex)
    {
        if (pDataParams->pCardTable[wIndex].bValid == PHAL_VCA_CARD_TABLE_ENTRY_INVALID)
        {
            ++wDummyValidIids;
        }
        else
        {
            ++wValidIids;
        }
    }

    *pNumValidIids = wValidIids;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Sw_GetIidInfo(
                                 phalVca_Sw_DataParams_t * pDataParams,
                                 uint16_t wValidIidIndex,
                                 uint16_t * pIidIndex,
                                 uint8_t * pVcUidSize,
                                 uint8_t * pVcUid,
                                 uint8_t * pInfo,
                                 uint8_t * pPdCapabilities
                                 )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Sw_Int_ResolveValidIndex(
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

phStatus_t phalVca_Sw_SelectVc(
                               phalVca_Sw_DataParams_t * pDataParams,
                               uint16_t wValidIidIndex,
                               uint16_t wKeyNumber,
                               uint16_t wKeyVersion
                               )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aTxBuffer[PHAL_VCA_IID_SIZE + 1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM aKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bMacLength;

    /* Prepare command header */
    aTxBuffer[0] = PHAL_VCA_CMD_SVC;
    aTxBuffer[1] = PHAL_VCA_CMD_SVC;

    /* Resolve Iid index */
    status = phalVca_Sw_Int_ResolveValidIndex(
        pDataParams,
        wValidIidIndex,
        &wValidIidIndex);

    /* for the case of an overflow we generate random data */
    /* Prepare MAC data */
    if ((status  & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
    {
        wValidIidIndex = 0;
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, PHAL_VCA_IID_SIZE-1, &aTxBuffer[2]));
    }
    else
    {
        PH_CHECK_SUCCESS(status);
        memcpy(&aTxBuffer[2], &pDataParams->pCardTable[wValidIidIndex].pCardData[1], PHAL_VCA_IID_SIZE-1);  /* PRQA S 3200 */
    }

    /* Get MAC Key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNumber,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &wKeyType));

    /* Check key type */
    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
    }

    /* Load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParams,
        aKey,
        PH_CRYPTOSYM_KEY_TYPE_AES128));

    /* Calculate MAC */
    /* CMAC with Padding */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParams,
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        &aTxBuffer[1],
        PHAL_VCA_IID_SIZE,
        &aTxBuffer[1],
        &bMacLength));

    /* Truncate MAC */
    phalVca_Sw_Int_TruncateMac(&aTxBuffer[1], &aTxBuffer[1]);

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aTxBuffer,
        1 + PHAL_VCA_TRUNCATED_MAC_SIZE,
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

phStatus_t phalVca_Sw_DeselectVc(
                                 phalVca_Sw_DataParams_t * pDataParams
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aTxBuffer[1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    aTxBuffer[0] = PHAL_VCA_CMD_DVC;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aTxBuffer,
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

phStatus_t phalVca_Sw_ProximityCheck(
                                     phalVca_Sw_DataParams_t * pDataParams,
                                     uint8_t     bGenerateRndC,
                                     uint8_t*    pRndC, 
                                     uint8_t     bPps1,
                                     uint8_t     bNumSteps,
                                     uint16_t    wKeyNumber,        
                                     uint16_t    wKeyVersion, 
                                     uint8_t * pUsedRndRC
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Sw_Cmd_PrepareProximityCheck(pDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Sw_Cmd_ProximityCheck(pDataParams, bGenerateRndC, pRndC, bNumSteps, pUsedRndRC));

    return (phalVca_Sw_Cmd_VerifyProximityCheck(pDataParams, pUsedRndRC, bPps1, wKeyNumber, wKeyVersion));
}

phStatus_t phalVca_Sw_Cmd_PrepareProximityCheck(
    phalVca_Sw_DataParams_t * pDataParams
    )
{
    /* send "Prepare Proximity Check" command */
    return(phalVca_Int_PrepareProximityCheck(pDataParams->pPalMifareDataParams));
}

phStatus_t phalVca_Sw_Cmd_ProximityCheck(
    phalVca_Sw_DataParams_t * pDataParams,
    uint8_t bGenerateRndC,
    uint8_t * pRndC,
    uint8_t bNumSteps,
    uint8_t * pUsedRndRC
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRndC[PHAL_VCA_PC_RND_LEN];

    /* parameter checking */
    if (((bGenerateRndC == 0) && (pRndC == NULL)) || (pUsedRndRC == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
    }


    /* check whether to generate RndC or not */
    if (bGenerateRndC)
    {
        pRndC = bRndC;
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(
            pDataParams->pCryptoRngDataParams,
            PHAL_VCA_PC_RND_LEN,
            pRndC
            ));
    }

    /* send "Proximity Check" command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ProximityCheck(
        pDataParams->pPalMifareDataParams,
        bNumSteps,
        pRndC,
        pUsedRndRC));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Sw_Cmd_VerifyProximityCheck(
    phalVca_Sw_DataParams_t * pDataParams,
    uint8_t*    pRndRC,
    uint8_t     bPps1,
    uint16_t    wKeyNumber,        
    uint16_t    wKeyVersion
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM aTmpBuf[2 + PHAL_VCA_PC_RND_LEN * 2];
    uint8_t     PH_MEMLOC_REM aKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bMac[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bMacLength;

    /* prepare "Verify Proximity Check" command */
    aTmpBuf[0] = PHAL_VCA_CMD_VPC;
    aTmpBuf[1] = bPps1;
    memcpy(&aTmpBuf[2], pRndRC, PHAL_VCA_PC_RND_LEN * 2);  /* PRQA S 3200 */

    /* Get Proximity Check Key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNumber,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &wKeyType));

    /* Check key type */
    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
    }

    /* Load Proximity Check Key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParams,
        aKey,
        PH_CRYPTOSYM_KEY_TYPE_AES128));

    /* CMAC with Padding */
    /* mac calculation: CMAC(CMD || PPS1 || (RndRC1 || ... || RndRC14)) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParams,
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aTmpBuf,
        (2 + PHAL_VCA_PC_RND_LEN * 2),
        bMac,
        &bMacLength));

    /* perform MAC truncation */
    phalVca_Sw_Int_TruncateMac(bMac, bMac);

    /* buffer the command code */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aTmpBuf,
        1,
        &pResponse,
        &wRxLength));

    /* append the MAC and exchange frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bMac,
        PHAL_VCA_TRUNCATED_MAC_SIZE,
        &pResponse,
        &wRxLength));

    /* check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));

    /* check response length */
    if (wRxLength != 1 /* Status */ + 8 /* MAC */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    /* Calculate MAC */
    aTmpBuf[0] = pResponse[0];
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParams,
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aTmpBuf,
        (2 + PHAL_VCA_PC_RND_LEN * 2),
        bMac,
        &bMacLength));    

    /* perform MAC truncation */
    phalVca_Sw_Int_TruncateMac(bMac, bMac);

    /* Compare MAC */
    if (memcmp(bMac, &pResponse[1], PHAL_VCA_TRUNCATED_MAC_SIZE) != 0)
    {
        return PH_ADD_COMPCODE(PHAL_VCA_ERR_AUTH, PH_COMP_AL_VCA);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Sw_Int_ResolveValidIndex(
    phalVca_Sw_DataParams_t * pDataParams,
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
        if (pDataParams->pCardTable[bIndex].bValid == 0xFF)
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

void phalVca_Sw_Int_TruncateMac(
                                uint8_t * pMac,
                                uint8_t * pTruncatedMac
                                )
{
    uint8_t PH_MEMLOC_REM bIndex;

    /* truncated MAC = [1, 3, 5, 7, 9, 11, 13, 15] of the input Mac */
    for (bIndex = 0; bIndex < PHAL_VCA_TRUNCATED_MAC_SIZE; ++bIndex)
    {
        pTruncatedMac[bIndex] = pMac[1 + (bIndex << 1)];
    }
}

#endif /* NXPBUILD__PHAL_VCA_SW */
