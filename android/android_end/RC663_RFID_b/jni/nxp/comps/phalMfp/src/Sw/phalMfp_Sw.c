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
* $Date: Fri Apr 15 09:27:39 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phalMfp.h>
#include <phpalMifare.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <phKeyStore.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFP_SW

#include "../phalMfp_Int.h"
#include "phalMfp_Sw.h"
#include "phalMfp_Sw_Int.h"

/*
* Private constants
*/
static const uint8_t PH_MEMLOC_CONST_ROM phalMfp_Sw_FirstIv[PH_CRYPTOSYM_AES_BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

phStatus_t phalMfp_Sw_Init(
                           phalMfp_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams, 
                           void * pPalMifareDataParams,
                           void * pKeyStoreDataParams,
                           void * pCryptoDataParamsEnc,
                           void * pCryptoDataParamsMac,
                           void * pCryptoRngDataParams,
                           void * pCryptoDiversifyDataParams 
                           )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* data param check */
    if (sizeof(phalMfp_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalMifareDataParams);
    PH_ASSERT_NULL (pCryptoDataParamsEnc);
    PH_ASSERT_NULL (pCryptoDataParamsMac);
    PH_ASSERT_NULL (pCryptoRngDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFP | PHAL_MFP_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;
    pDataParams->pCryptoDataParamsEnc   = pCryptoDataParamsEnc;
    pDataParams->pCryptoDataParamsMac   = pCryptoDataParamsMac;
    pDataParams->pCryptoRngDataParams   = pCryptoRngDataParams;
    pDataParams->pCryptoDiversifyDataParams  = pCryptoDiversifyDataParams;
    memset(pDataParams->bKeyModifier, 0x00, PHAL_MFP_SIZE_KEYMODIFIER); /* PRQA S 3200 */

    /* clear the secure messaging state */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_ResetSecMsgState(pDataParams));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_WritePerso(
                                 phalMfp_Sw_DataParams_t * pDataParams,
                                 uint8_t bLayer4Comm,
                                 uint16_t wBlockNr,
                                 uint8_t * pValue
                                 )
{
    return phalMfp_Int_WritePerso(pDataParams->pPalMifareDataParams, bLayer4Comm, wBlockNr, pValue);
}

phStatus_t phalMfp_Sw_CommitPerso(
                                  phalMfp_Sw_DataParams_t * pDataParams,
                                  uint8_t bLayer4Comm
                                  )
{
    return phalMfp_Int_CommitPerso(pDataParams->pPalMifareDataParams, bLayer4Comm);
}

phStatus_t phalMfp_Sw_AuthenticateSL0(
                                      phalMfp_Sw_DataParams_t * pDataParams,
                                      uint8_t bLayer4Comm,
                                      uint8_t bFirstAuth,
                                      uint16_t wBlockNr,
                                      uint16_t wKeyNumber,
                                      uint16_t wKeyVersion,
                                      uint8_t bLenDivInput,
                                      uint8_t * pDivInput,
                                      uint8_t bLenPcdCap2,
                                      uint8_t * pPcdCap2In,
                                      uint8_t * pPcdCap2Out,
                                      uint8_t * pPdCap2
                                      )
{
    return phalMfp_Sw_AuthenticateGeneral(
        pDataParams,
        bLayer4Comm,
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion, 
        bLenDivInput,
        pDivInput,
        PH_OFF, /* No SL2 KDF */
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out,  
        pPdCap2);
}

phStatus_t phalMfp_Sw_AuthenticateSL1(
                                      phalMfp_Sw_DataParams_t * pDataParams,
                                      uint8_t bLayer4Comm,
                                      uint8_t bFirstAuth,
                                      uint16_t wBlockNr,
                                      uint16_t wKeyNumber,
                                      uint16_t wKeyVersion,
                                      uint8_t bLenDivInput,
                                      uint8_t * pDivInput,
                                      uint8_t bLenPcdCap2,
                                      uint8_t * pPcdCap2In,
                                      uint8_t * pPcdCap2Out,
                                      uint8_t * pPdCap2
                                      )
{
    return phalMfp_Sw_AuthenticateGeneral(
        pDataParams,
        bLayer4Comm,
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion,
        bLenDivInput,
        pDivInput,
        PH_OFF, /* No SL2 KDF */
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out, 
        pPdCap2);
}

phStatus_t phalMfp_Sw_AuthenticateSL2(
                                      phalMfp_Sw_DataParams_t * pDataParams,
                                      uint8_t bLayer4Comm,
                                      uint8_t bFirstAuth,
                                      uint16_t wBlockNr,
                                      uint16_t wKeyNumber,
                                      uint16_t wKeyVersion,
                                      uint8_t bLenDivInput,
                                      uint8_t * pDivInput,
                                      uint8_t bLenPcdCap2,
                                      uint8_t * pPcdCap2In,
                                      uint8_t * pPcdCap2Out,
                                      uint8_t * pPdCap2,
                                      uint8_t * pKmf
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bUseKdfSl2;

    /* Evaluate usage of KdfSl2 */
    if (bLayer4Comm)
    {
        bUseKdfSl2 = PH_OFF;
    }
    else
    {
        bUseKdfSl2 = PH_ON;
    }

    /* Perform actual authentication */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_AuthenticateGeneral(
        pDataParams,
        bLayer4Comm,
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion,
        bLenDivInput,
        pDivInput,
        bUseKdfSl2,
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out, 
        pPdCap2));

    /* Successful Authentication: Perform key derivation and return SL2 key modifier */
    memcpy(pKmf, pDataParams->bKeyModifier, PHAL_MFP_SIZE_KEYMODIFIER); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_AuthenticateClassicSL2(
    phalMfp_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aKey[PHHAL_HW_MFC_KEY_LENGTH * 2];
    uint8_t *   PH_MEMLOC_REM pKey;
    uint16_t    PH_MEMLOC_REM bKeystoreKeyType;
    uint8_t     PH_MEMLOC_REM bValue;

    /* Bail out if we haven't got a keystore */
    if (pDataParams->pKeyStoreDataParams == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_HAL);
    }

    /* retrieve KeyA & KeyB from keystore */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &bKeystoreKeyType));

    /* check key type */
    if (bKeystoreKeyType != PH_KEYSTORE_KEY_TYPE_MIFARE)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Evaluate which key to use */
    if ((bKeyType & 0x7F) == PHHAL_HW_MFC_KEYA)
    {
        /* Use KeyA */
        pKey = aKey;
    }
    else if ((bKeyType & 0x7F) == PHHAL_HW_MFC_KEYB)
    {
        /* Use KeyB */
        pKey = &aKey[PHHAL_HW_MFC_KEY_LENGTH];
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Apply the key modifier if requested */
    for (bValue = 0; bValue < PHHAL_HW_MFC_KEY_LENGTH; bValue++)
    {
        pKey[bValue] ^= pDataParams->bKeyModifier[bValue];
    }

    return phpalMifare_MfcAuthenticate(
        pDataParams->pPalMifareDataParams,
        bBlockNo,
        bKeyType,
        pKey,
        &pUid[bUidLength - 4]);
}

phStatus_t phalMfp_Sw_MultiBlockRead(
                                     phalMfp_Sw_DataParams_t * pDataParams,
                                     uint8_t bBlockNr,
                                     uint8_t bNumBlocks,
                                     uint8_t * pBlocks
                                     )
{
    return phalMfp_Int_MultiBlockRead(pDataParams->pPalMifareDataParams, bBlockNr, bNumBlocks, pBlocks);
}

phStatus_t phalMfp_Sw_MultiBlockWrite(
                                      phalMfp_Sw_DataParams_t * pDataParams,
                                      uint8_t bBlockNr,
                                      uint8_t bNumBlocks,
                                      uint8_t * pBlocks
                                      )
{
    return phalMfp_Int_MultiBlockWrite(pDataParams->pPalMifareDataParams, bBlockNr, bNumBlocks, pBlocks);
}

phStatus_t phalMfp_Sw_Write(
                            phalMfp_Sw_DataParams_t * pDataParams,
                            uint8_t bEncrypted,
                            uint8_t bWriteMaced,
                            uint16_t wBlockNr,
                            uint8_t bNumBlocks,
                            uint8_t * pBlocks
                            )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bWriteMaced)
    {
        if (bEncrypted)
        {
            aCmd = PHAL_MFP_CMD_WRITE_EM;
        }
        else
        {
            aCmd = PHAL_MFP_CMD_WRITE_PM;
        }
    }
    else
    {
        if (bEncrypted)
        {
            aCmd = PHAL_MFP_CMD_WRITE_EN;
        }
        else
        {
            aCmd = PHAL_MFP_CMD_WRITE_PN;
        }
    }

    /* Perform actual write operation*/
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wBlockNr,
        0x00,
        pBlocks,
        (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE),
        bEncrypted);
}


phStatus_t phalMfp_Sw_WriteValue(
                                 phalMfp_Sw_DataParams_t * pDataParams,
                                 uint8_t bEncrypted,
                                 uint8_t bWriteMaced,
                                 uint16_t wBlockNr,
                                 uint8_t * pValue,
                                 uint8_t bAddrData
                                 )
{
    phStatus_t statusTmp;
    uint8_t pBlock[16];
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_CreateValueBlock(pValue, bAddrData, pBlock));
    return phalMfp_Sw_Write(pDataParams, bEncrypted, bWriteMaced, wBlockNr, 1, pBlock);
}

phStatus_t phalMfp_Sw_ChangeKey(
                                phalMfp_Sw_DataParams_t * pDataParams,  
                                uint8_t bChangeKeyMaced, 
                                uint16_t wBlockNr,   
                                uint16_t wKeyNumber, 
                                uint16_t wKeyVersion,
                                uint8_t bLenDivInput,
                                uint8_t * pDivInput  
                                )
{
    phStatus_t statusTmp;
    uint8_t aKey[16];
    uint16_t wKeyType;
    uint8_t aCmd;

    if (bChangeKeyMaced)
    {
        aCmd = PHAL_MFP_CMD_WRITE_EM;
    }else
    {
        aCmd = PHAL_MFP_CMD_WRITE_EN;
    }

    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNumber,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &wKeyType));

    /* Key type check */
    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }


    if (bLenDivInput)
    {
        if (pDataParams->pCryptoDiversifyDataParams == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFP);
        }
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(pDataParams->pCryptoDiversifyDataParams, 
            PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS,
            aKey, 
            wKeyType, 
            pDivInput, 
            bLenDivInput, 
            aKey));  
    }


    /* Perform actual write operation*/
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wBlockNr,
        0x00,
        aKey,
        PHAL_MFP_MIFARE_BLOCK_SIZE,
        0x01);
}

phStatus_t phalMfp_Sw_AuthenticateSL3(
                                      phalMfp_Sw_DataParams_t * pDataParams,
                                      uint8_t bFirstAuth,
                                      uint16_t wBlockNr,
                                      uint16_t wKeyNumber,
                                      uint16_t wKeyVersion,
                                      uint8_t bLenDivInput,
                                      uint8_t * pDivInput,
                                      uint8_t bLenPcdCap2,
                                      uint8_t * pPcdCap2In,
                                      uint8_t * pPcdCap2Out,
                                      uint8_t * pPdCap2
                                      )
{
    return phalMfp_Sw_AuthenticateGeneral(
        pDataParams,
        PH_ON, /* Layer 4 */
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion,
        bLenDivInput,
        pDivInput,
        PH_OFF, /* No SL2 KDF */
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out, 
        pPdCap2);
}

phStatus_t phalMfp_Sw_Read(
                           phalMfp_Sw_DataParams_t * pDataParams,
                           uint8_t bEncrypted,
                           uint8_t bReadMaced,
                           uint8_t bMacOnCmd,
                           uint16_t wBlockNr,
                           uint8_t bNumBlocks,
                           uint8_t * pBlocks
                           )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd;
    uint8_t     PH_MEMLOC_REM bTxBuffer[10];
    uint16_t    PH_MEMLOC_REM wTxLength;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM aMac[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bMacLength;
    uint8_t *   PH_MEMLOC_REM pResponseTmp;
    uint16_t    PH_MEMLOC_REM wRxLengthTmp;
    uint16_t    PH_MEMLOC_REM wTotalRxLength;
    uint8_t     PH_MEMLOC_REM aIv[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM pUnprocessedEncData[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bNumUnprocessedEncData = 0;
    uint16_t    PH_MEMLOC_REM wNumBlocksStartPos = 0;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint8_t     PH_MEMLOC_REM bFinished;

    /* Evaluate command code */
    if (bReadMaced)
    {
        if (bEncrypted)
        {
            if (bMacOnCmd)
            {
                aCmd = PHAL_MFP_CMD_READ_EMM;
            }
            else
            {
                aCmd = PHAL_MFP_CMD_READ_EMU;
            }
        }
        else
        {
            if (bMacOnCmd)
            {
                aCmd = PHAL_MFP_CMD_READ_PMM;
            }
            else
            {
                aCmd = PHAL_MFP_CMD_READ_PMU;
            }
        }
    }
    else
    {
        if (bEncrypted)
        {
            if (bMacOnCmd)
            {
                aCmd = PHAL_MFP_CMD_READ_ENM;
            }
            else
            {
                aCmd = PHAL_MFP_CMD_READ_ENU;
            }
        }
        else
        {
            if (bMacOnCmd)
            {
                aCmd = PHAL_MFP_CMD_READ_PNM;
            }
            else
            {
                aCmd = PHAL_MFP_CMD_READ_PNU;
            }
        }
    }

    /* Build the command frame */
    wTxLength = 0;
    bTxBuffer[6 + wTxLength++] = aCmd;
    bTxBuffer[6 + wTxLength++] = (uint8_t)(wBlockNr & 0xFF); /* LSB */
    bTxBuffer[6 + wTxLength++] = (uint8_t)(wBlockNr >> 8);   /* MSB */
    bTxBuffer[6 + wTxLength++] = bNumBlocks;

    /* Prepare for MAC on cmd */
    if (bMacOnCmd)
    {
        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            &bTxBuffer[6],
            wTxLength,
            &pResponse,
            &wRxLength));

        /* Prepare MAC calculation */
        bTxBuffer[0] = aCmd;
        bTxBuffer[1] = (uint8_t)(pDataParams->wRCtr);
        bTxBuffer[2] = (uint8_t)(pDataParams->wRCtr >> 8);
        memcpy(&bTxBuffer[3], pDataParams->bTi, PHAL_MFP_SIZE_TI); /* PRQA S 3200 */
        wTxLength = wTxLength + 2 /* RCtr */ + PHAL_MFP_SIZE_TI /* TI*/;

        /* Now calculate the MAC */
        /* CMAC with Padding */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            PH_CRYPTOSYM_MAC_MODE_CMAC,
            bTxBuffer,
            wTxLength,
            aMac,
            &bMacLength));

        /* Perform MAC truncation */
        phalMfp_Sw_Int_TruncateMac(aMac, aMac);

        /* Exchange the command including the MAC */
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            aMac,
            PHAL_MFP_TRUNCATED_MAC_SIZE,
            &pResponse,
            &wRxLength);
    }
    else
    {
        /* command exchange */
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            &bTxBuffer[6],
            wTxLength,
            &pResponse,
            &wRxLength);
    }

    /* Ignore success chaining status */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], PH_ON));

    /* Increment Read Counter */
    pDataParams->wRCtr++;

    /* we also have to decrypt the response */
    if (bEncrypted)
    {   
        /* Lets load the ENC IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_ComputeIv(
            PH_ON,
            pDataParams->bTi,
            pDataParams->wRCtr,
            pDataParams->wWCtr,
            aIv));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            aIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Reset TxLength */
    wTxLength = 0;

    /* In case of FIRST Read, we need to start loading TI and RCtr */
    if (pDataParams->bFirstRead)
    {
        pDataParams->bFirstRead = 0;

        /* Recopy the status code */
        bTxBuffer[wTxLength++] = pResponse[0];

        /* Rearrange the MAC header according to spec */
        bTxBuffer[wTxLength++] = (uint8_t)pDataParams->wRCtr;
        bTxBuffer[wTxLength++] = (uint8_t)(pDataParams->wRCtr >> 8);

        /* Lets recopy TI */
        memcpy(&bTxBuffer[wTxLength], pDataParams->bTi, PHAL_MFP_SIZE_TI); /* PRQA S 3200 */
        wTxLength = wTxLength + PHAL_MFP_SIZE_TI /* TI*/;

        /* Load first IV*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            phalMfp_Sw_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }
    else
    {
        /* Load current IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            pDataParams->pIntermediateMac,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Now we need to copy the BNR and the Ext byte */
    bTxBuffer[wTxLength++] = (uint8_t)(wBlockNr & 0xFF); /* LSB */
    bTxBuffer[wTxLength++] = (uint8_t)(wBlockNr >> 8);   /* MSB */
    bTxBuffer[wTxLength++] = bNumBlocks;

    /* Update the response buffer */
    ++pResponse;
    --wRxLength;

    /* Recopy the header into the pending data */
    /* In case the whole header fits into the unprocessed ReadMac Buffer, we can simply recopy. */
    /* Else an intermediate MAC calculation is necessary...*/
    wIndex = PH_CRYPTOSYM_AES_BLOCK_SIZE - pDataParams->bNumUnprocessedReadMacBytes;

    if (wIndex >= wTxLength)
    {
        /* Just recopy the data */
        memcpy(&pDataParams->pUnprocessedReadMacBuffer[pDataParams->bNumUnprocessedReadMacBytes], bTxBuffer, wTxLength); /* PRQA S 3200 */
        pDataParams->bNumUnprocessedReadMacBytes = (uint8_t)(pDataParams->bNumUnprocessedReadMacBytes + wTxLength);
    }
    else
    {
        /* Perform intermediate MAC calculation and update pUnprocessedReadMacBuffer */

        /* Recopy as many bytes as possible */
        memcpy(&pDataParams->pUnprocessedReadMacBuffer[pDataParams->bNumUnprocessedReadMacBytes], bTxBuffer, wIndex); /* PRQA S 3200 */

        /* Update the unprocessed Read MAC bytes */
        pDataParams->bNumUnprocessedReadMacBytes = (uint8_t)(pDataParams->bNumUnprocessedReadMacBytes + wIndex);

        /* Perform the MAC calculation inside of the Unprocessed Read Mac Buffer */

        /* CMAC without Padding */
        /* Perform the MAC calculation */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_MAC_MODE_CMAC,
            pDataParams->pUnprocessedReadMacBuffer,
            pDataParams->bNumUnprocessedReadMacBytes,
            pDataParams->pIntermediateMac,
            &bMacLength));

        /* Recopy the rest of the data into the pUnprocessedReadMacBuffer*/
        memcpy(pDataParams->pUnprocessedReadMacBuffer, &bTxBuffer[wIndex], wTxLength - wIndex); /* PRQA S 3200 */

        /* Update unprocessed Read Mac length */
        pDataParams->bNumUnprocessedReadMacBytes = (uint8_t)(wTxLength - wIndex);
    }

    /* chaining loop */
    wTotalRxLength = 0;
    bFinished = 0;
    do
    {
        wTotalRxLength = wTotalRxLength + wRxLength;

        /* Length Check */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Verify the Length - Status Code has already been consumed! */
            if (bReadMaced)
            {
                /* check response length */
                if (wTotalRxLength != (PHAL_MFP_TRUNCATED_MAC_SIZE /* MAC */ + (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE) /* Data */))
                {
                    return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
                }

                /* Remove the temporarily received MAC length */
                wRxLength = wRxLength - PHAL_MFP_TRUNCATED_MAC_SIZE;
            }
            else
            {
                /* check response length */
                if (wTotalRxLength != (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE) /* Data */)
                {
                    return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
                }
            }
        }

        /* Initialize helpers */
        pResponseTmp = pResponse;
        wRxLengthTmp = wRxLength;

        /* In case of unprocessed read MAC bytes, we first need to empty the unprocessed read buffer */
        wIndex = PH_CRYPTOSYM_AES_BLOCK_SIZE - pDataParams->bNumUnprocessedReadMacBytes;

        while (wIndex < wRxLengthTmp)
        {
            /* Recopy as many bytes as possible */
            memcpy(&pDataParams->pUnprocessedReadMacBuffer[pDataParams->bNumUnprocessedReadMacBytes], pResponseTmp, wIndex); /* PRQA S 3200 */

            wRxLengthTmp = wRxLengthTmp - wIndex;
            pResponseTmp += wIndex;
            pDataParams->bNumUnprocessedReadMacBytes = (uint8_t)(pDataParams->bNumUnprocessedReadMacBytes + wIndex);

            /* Perform the MAC calculation */
            /* CMAC without Padding */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_MAC_MODE_CMAC,
                pDataParams->pUnprocessedReadMacBuffer,
                pDataParams->bNumUnprocessedReadMacBytes,
                pDataParams->pIntermediateMac,
                &bMacLength));

            /* Reset unprocessed MAC bytes */
            pDataParams->bNumUnprocessedReadMacBytes = 0;

            /* In case of unprocessed read MAC bytes, we first need to empty the unprocessed read buffer */
            wIndex = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        }

        /* No complete block to be MACED is available. */
        /* Just recopy the data */
        memcpy(&pDataParams->pUnprocessedReadMacBuffer[pDataParams->bNumUnprocessedReadMacBytes], pResponseTmp, wRxLengthTmp); /* PRQA S 3200 */
        pDataParams->bNumUnprocessedReadMacBytes = (uint8_t)(pDataParams->bNumUnprocessedReadMacBytes + wRxLengthTmp);    
        pResponseTmp += wRxLengthTmp;

        /* now perform the decryption */
        if (bEncrypted)
        {
            wIndex = 0;
            if (bNumUnprocessedEncData)
            {
                /* Recopy the data */
                wIndex = PH_CRYPTOSYM_AES_BLOCK_SIZE - bNumUnprocessedEncData;
                memcpy(&pUnprocessedEncData[bNumUnprocessedEncData], pResponse, wIndex); /* PRQA S 3200 */
                bNumUnprocessedEncData = 0;

                /* Decrypt the data */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
                    pDataParams->pCryptoDataParamsEnc,
                    PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
                    pResponse,
                    PH_CRYPTOSYM_AES_BLOCK_SIZE,
                    &pBlocks[wNumBlocksStartPos]
                ));

                wNumBlocksStartPos += PH_CRYPTOSYM_AES_BLOCK_SIZE;
            }

            wRxLengthTmp = wRxLength - wIndex;
            bNumUnprocessedEncData = (uint8_t)(wRxLengthTmp % PH_CRYPTOSYM_AES_BLOCK_SIZE);
            if (bNumUnprocessedEncData)
            {
                memcpy(pUnprocessedEncData, &pResponse[wRxLength - bNumUnprocessedEncData],bNumUnprocessedEncData); /* PRQA S 3200 */
                wRxLengthTmp = wRxLengthTmp - bNumUnprocessedEncData;
            }
            if ((wNumBlocksStartPos + wRxLengthTmp) > (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE))
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
            }

            /* Decrypt the data */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
                &pResponse[wIndex],
                wRxLengthTmp,
                &pBlocks[wNumBlocksStartPos]
            ));

            wNumBlocksStartPos = wRxLengthTmp + wNumBlocksStartPos;
        }
        else
        {
            if ((wNumBlocksStartPos + wRxLength) > (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE))
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
            }
            memcpy(&pBlocks[wNumBlocksStartPos], pResponse, wRxLength); /* PRQA S 3200 */
            wNumBlocksStartPos = wRxLength + wNumBlocksStartPos;
        }

        /* Finally let's verify the MAC */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Did we receive a MAC? Then let's check else remember the part of the response */
            if (bReadMaced)
            {
                /* Calculate the MAC*/
                /* CMAC with Padding */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    PH_EXCHANGE_BUFFER_LAST | PH_CRYPTOSYM_MAC_MODE_CMAC,
                    pDataParams->pUnprocessedReadMacBuffer,
                    pDataParams->bNumUnprocessedReadMacBytes,
                    pDataParams->pIntermediateMac,
                    &bMacLength));

                /* Reset to first read */
                pDataParams->bFirstRead = 1;
                pDataParams->bNumUnprocessedReadMacBytes = 0;

                /* Truncate the MAC */
                phalMfp_Sw_Int_TruncateMac(pDataParams->pIntermediateMac,pDataParams->pIntermediateMac);

                /* Compare the result - note that wRxLength has been decremented upfront*/
                if (memcmp(pDataParams->pIntermediateMac, &pResponse[wRxLength], PHAL_MFP_TRUNCATED_MAC_SIZE) != 0x00)
                {
                    return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFP);
                }
            }
            bFinished = 1;
        }
        else
        {
            /* Continue with next Data block */
            status = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_RXCHAINING,
                NULL,
                0,
                &pResponse,
                &wRxLength);

            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                PH_CHECK_SUCCESS(status);
            }
        }
    }
    while (!bFinished);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_ReadValue(
                                phalMfp_Sw_DataParams_t * pDataParams,
                                uint8_t bEncrypted,
                                uint8_t bReadMaced,
                                uint8_t bMacOnCmd,
                                uint16_t wBlockNr,
                                uint8_t * pValue,
                                uint8_t * pAddrData
                                )
{
    phStatus_t statusTmp;
    uint8_t pData[16];
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Read(pDataParams, bEncrypted, bReadMaced, bMacOnCmd, wBlockNr, 1, pData));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_CheckValueBlockFormat(pData));
    *pAddrData = pData[12];
    memcpy(pValue, pData, 4); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_ResetAuth(
                                phalMfp_Sw_DataParams_t * pDataParams
                                )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1 /* command code */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    aCmd[0] = PHAL_MFP_CMD_RAUTH;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResponse,
        &wRxLength));

    /* check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 1));

    /* Reset the crypto layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_ResetSecMsgState(pDataParams));

    /* return exchange status code */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_Increment(
                                phalMfp_Sw_DataParams_t * pDataParams,
                                uint8_t bIncrementMaced,
                                uint16_t wBlockNr,
                                uint8_t * pValue
                                )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bIncrementMaced)
    {
        aCmd = PHAL_MFP_CMD_INCR_M;
    }
    else
    {
        aCmd = PHAL_MFP_CMD_INCR;
    }

    /* Perform actual write operation */
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wBlockNr,
        0x00,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE,
        PH_ON); /* Encrypted */
}

phStatus_t phalMfp_Sw_Decrement(
                                phalMfp_Sw_DataParams_t * pDataParams,
                                uint8_t bDecrementMaced,
                                uint16_t wBlockNr,
                                uint8_t * pValue
                                )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bDecrementMaced)
    {
        aCmd = PHAL_MFP_CMD_DECR_M;
    }
    else
    {
        aCmd = PHAL_MFP_CMD_DECR;
    }

    /* Perform actual write operation */
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wBlockNr,
        0x00,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE,
        PH_ON); /* Encrypted */
}

phStatus_t phalMfp_Sw_IncrementTransfer(
                                        phalMfp_Sw_DataParams_t * pDataParams,
                                        uint8_t bIncrementTransferMaced,
                                        uint16_t wSourceBlockNr,
                                        uint16_t wDestinationBlockNr,
                                        uint8_t * pValue
                                        )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bIncrementTransferMaced)
    {
        aCmd = PHAL_MFP_CMD_INCRTR_M;
    }
    else
    {
        aCmd = PHAL_MFP_CMD_INCRTR;
    }

    /* Perform actual write operation */
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wSourceBlockNr,
        wDestinationBlockNr,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE,
        PH_ON); /* Encrypted */
}

phStatus_t phalMfp_Sw_DecrementTransfer(
                                        phalMfp_Sw_DataParams_t * pDataParams,
                                        uint8_t bDecrementTransferMaced,
                                        uint16_t wSourceBlockNr,
                                        uint16_t wDestinationBlockNr,
                                        uint8_t * pValue
                                        )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bDecrementTransferMaced)
    {
        aCmd = PHAL_MFP_CMD_DECRTR_M;
    }
    else
    {
        aCmd = PHAL_MFP_CMD_DECRTR;
    }

    /* Perform actual write operation */
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wSourceBlockNr,
        wDestinationBlockNr,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE,
        PH_ON); /* Encrypted */
}

phStatus_t phalMfp_Sw_Transfer(
                               phalMfp_Sw_DataParams_t * pDataParams,
                               uint8_t bTransferMaced,
                               uint16_t wBlockNr
                               )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bTransferMaced)
    {
        aCmd = PHAL_MFP_CMD_TRANS_M;
    }
    else
    {
        aCmd = PHAL_MFP_CMD_TRANS;
    }

    /* Perform actual write operation */
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wBlockNr,
        0x00,
        NULL,
        0,
        PH_OFF); /* Plain */
}

phStatus_t phalMfp_Sw_Restore(
                              phalMfp_Sw_DataParams_t * pDataParams,
                              uint8_t bRestoreMaced,
                              uint16_t wBlockNr
                              )
{
    uint8_t PH_MEMLOC_REM aCmd;

    /* Evaluate command code */
    if (bRestoreMaced)
    {
        aCmd = PHAL_MFP_CMD_REST_M;
    }
    else
    {
        aCmd = PHAL_MFP_CMD_REST;
    }

    /* Perform actual write operation */
    return phalMfp_Sw_WriteExt(
        pDataParams,
        aCmd,
        wBlockNr,
        0x00,
        NULL,
        0,
        PH_OFF); /* Plain */
}

phStatus_t phalMfp_Sw_ProximityCheck(
                                     phalMfp_Sw_DataParams_t * pDataParams,
                                     uint8_t bGenerateRndC,
                                     uint8_t * pRndC,
                                     uint8_t bPps1,
                                     uint8_t bNumSteps,
                                     uint8_t * pUsedRndRC
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Cmd_PrepareProximityCheck(pDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Cmd_ProximityCheck(pDataParams, bGenerateRndC, pRndC, bNumSteps, pUsedRndRC));

    return (phalMfp_Sw_Cmd_VerifyProximityCheck(pDataParams, pUsedRndRC, bPps1));
}

phStatus_t phalMfp_Sw_ResetSecMsgState(
                                       phalMfp_Sw_DataParams_t * pDataParams
                                       )
{
    pDataParams->wRCtr                          = 0;
    pDataParams->wWCtr                          = 0;
    pDataParams->bNumUnprocessedReadMacBytes    = 0;
    pDataParams->bFirstRead                     = 1;
    memset(pDataParams->bTi, 0x00, PHAL_MFP_SIZE_TI); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_AuthenticateGeneral(
    phalMfp_Sw_DataParams_t * pDataParams,
    uint8_t bLayer4Comm,
    uint8_t bFirstAuth,
    uint16_t wBlockNr,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t bLenDivInput,
    uint8_t * pDivInput,
    uint8_t bUseKdfSl2,
    uint8_t bLenPcdCap2,
    uint8_t * pPcdCap2In,
    uint8_t * pPcdCap2Out,
    uint8_t * pPdCap2
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM pHeader[33];
    uint8_t     PH_MEMLOC_REM bHeaderLength = 0;
    uint8_t     PH_MEMLOC_REM aKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bRndA[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE+1];
    uint8_t     PH_MEMLOC_REM aIv[PH_CRYPTOSYM_AES_BLOCK_SIZE];

    /* parameter checking */
    if (((bLenPcdCap2 > 0) && (pPcdCap2In == NULL)) || (bLenPcdCap2 > 6))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* In case of first auth or layer 3 communication we need to reset the secure messaging layer */
    if ((bFirstAuth) || (!bLayer4Comm))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_ResetSecMsgState(pDataParams));
    }

    /* Create First Part of the Message */
    if (bFirstAuth)
    {
        pHeader[bHeaderLength++] = PHAL_MFP_CMD_AUTH1_FIRST;
    }
    else
    {
        pHeader[bHeaderLength++] = PHAL_MFP_CMD_AUTH1;
    }

    /* wBlockNr */
    pHeader[bHeaderLength++] = (uint8_t)(wBlockNr & 0xFF); /* LSB */
    pHeader[bHeaderLength++] = (uint8_t)(wBlockNr >> 8);   /* MSB */

    /* exchange command/response */
    if (bLayer4Comm)
    {
        /* Add PCDcap length in case of auth first */
        if (bFirstAuth == 1)
        {
            pHeader[bHeaderLength++] = bLenPcdCap2;
        }
        /* No PCDcaps in Non-First authenticate */
        else
        {
            bLenPcdCap2 = 0;
        }

        /* buffer the header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            pHeader,
            bHeaderLength,
            NULL,
            NULL));

        /* Append the PcdCaps and exchange frame */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pPcdCap2In,
            bLenPcdCap2,
            &pResponse,
            &wRxLength));
    }
    else
    {
        /* Add PCDcap length (0x00) in case of auth first */
        if (bFirstAuth == 1)
        {
            pHeader[bHeaderLength++] = 0x00;
        }

        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            pHeader,
            bHeaderLength,
            &pResponse,
            &wRxLength));
    }

    /* Check the response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], bLayer4Comm));

    if (wRxLength != (1 /* status */ + PH_CRYPTOSYM_AES_BLOCK_SIZE /* ENC(RNDB) */))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    /* Set first Read Flag to 1 */
    pDataParams->bFirstRead = 1;

    /* We also need to reset the unprocessed read MAC bytes according to specification */
    pDataParams->bNumUnprocessedReadMacBytes = 0; 

    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNumber,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &wKeyType));

    /* Key type check */
    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* Do we need to diversify the key? */
    if (bLenDivInput)
    {
        if (pDataParams->pCryptoDiversifyDataParams == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFP);
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(pDataParams->pCryptoDiversifyDataParams,
            PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS,
            aKey,
            wKeyType,
            pDivInput,
            bLenDivInput,
            aKey));
    }

    /* Load the key*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        aKey,
        wKeyType));

    /* Calculate the IV, in case of auth Next, already used parameters need to be considered */
    if (bFirstAuth == 0)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_ComputeIv(
            PH_ON, /* Response */
            pDataParams->bTi,
            pDataParams->wRCtr,
            pDataParams->wWCtr,
            aIv));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            aIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            phalMfp_Sw_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Decrypt the data key is already loaded. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        &pResponse[1],
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        bRndB
        ));

    /* Also perform the shift. */
    bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE] = bRndB[0];

    /* Start with part 2 of Authenticate MFP */

    /* Compute second part of the Auth sequence */
    bHeaderLength = 0;
    pHeader[bHeaderLength++] = PHAL_MFP_CMD_AUTH2;

    /* Generate RND A */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(
        pDataParams->pCryptoRngDataParams,
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        bRndA
        ));

    /* Load IV */
    if (bFirstAuth == 0)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_ComputeIv(
            PH_OFF, /* No Response */
            pDataParams->bTi,
            pDataParams->wRCtr,
            pDataParams->wWCtr,
            aIv));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            aIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            phalMfp_Sw_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Encrypt RndA */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        bRndA,
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        &pHeader[bHeaderLength]
    ));

    bHeaderLength += PH_CRYPTOSYM_AES_BLOCK_SIZE;

    /* Encrypt shifted RndB */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_EXCHANGE_BUFFER_LAST | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        &bRndB[1],
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        &pHeader[bHeaderLength]
    ));

    bHeaderLength += PH_CRYPTOSYM_AES_BLOCK_SIZE;

    /* command exchange */
    if (bLayer4Comm)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            pHeader,
            bHeaderLength,
            &pResponse,
            &wRxLength));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            pHeader,
            bHeaderLength,
            &pResponse,
            &wRxLength));
    }

    /* Check the status Code */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], bLayer4Comm));

    /* Check response */
    if (bFirstAuth == 0)
    {
        /* Check response length */
        if (wRxLength != (1 /* status */ + PH_CRYPTOSYM_AES_BLOCK_SIZE /* ENC(RNDA') */))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }

        /* Now start decrypting the response */

        /* Decrypt the data key is already loaded. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_ComputeIv(
            PH_ON, /* Response */
            pDataParams->bTi,
            pDataParams->wRCtr,
            pDataParams->wWCtr,
            aIv));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            aIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        PH_CHECK_SUCCESS_FCT(statusTmp ,phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
            &pResponse[1],
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            &pResponse[1]
        ));

        /* Shift RNDA' */
        pResponse[0] = pResponse[PH_CRYPTOSYM_AES_BLOCK_SIZE];

        /* Now perform the comparison */
        if (memcmp(bRndA, pResponse, PH_CRYPTOSYM_AES_BLOCK_SIZE) != 0)
        {
            /* RndA and RndA' don't match */
            return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFP);
        }
    }
    else
    {
        if (wRxLength != (1 /* status */ + (4 + PH_CRYPTOSYM_AES_BLOCK_SIZE + 12) /* ENC(TI | RNDA' | Capabilities) */))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }

        /* */
        /* Now start decrypting the response */
        /* */

        /* Load default init vector */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            phalMfp_Sw_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* Decrypt the data key is already loaded, pMac is used as temporary buffer. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC,
            &pResponse[1],
            PH_CRYPTOSYM_AES_BLOCK_SIZE << 1,
            &pResponse[1]
        ));

        /* First save the TI as we are then going to overwrite */
        memcpy(pDataParams->bTi, &pResponse[1], PHAL_MFP_SIZE_TI); /* PRQA S 3200 */

        /* Shift of RND A */
        pResponse[4] = pResponse[4 + PH_CRYPTOSYM_AES_BLOCK_SIZE];

        /* Now perform the comparison */
        if (memcmp(bRndA, &pResponse[4], PH_CRYPTOSYM_AES_BLOCK_SIZE) != 0)
        {
            /* RndA and RndA' don't match */
            return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFP);
        }

        memmove(pPdCap2, &pResponse[21], 6); /* PRQA S 3200 */

        memmove(pPcdCap2Out, &pResponse[27], 6); /* PRQA S 3200 */
    }

    if (bUseKdfSl2)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_KdfSl2(pDataParams, bRndA, bRndB));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_KdfSl3(pDataParams, bRndA, bRndB));
    }

    /* In case of Originality Key - reset secure messaging */
    if ((wBlockNr >= PHAL_MFP_ORIGINALITY_KEY_FIRST) && (wBlockNr <= PHAL_MFP_ORIGINALITY_KEY_LAST))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_ResetSecMsgState(pDataParams));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_WriteExt(
                               phalMfp_Sw_DataParams_t * pDataParams,
                               uint8_t bCmdCode,
                               uint16_t wSrcBnr,
                               uint16_t wDstBnr,
                               uint8_t * pData,
                               uint16_t wDataLength,
                               uint8_t bEncrypted
                               )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmdBuf[11 + (PH_CRYPTOSYM_AES_BLOCK_SIZE * 3)];
    uint16_t    PH_MEMLOC_REM wCmdLength;
    uint16_t    PH_MEMLOC_REM wPartLength;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM aIv[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM aMac[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bMacLength;

    if (wDataLength == 4)
    {
        memcpy(&aCmdBuf[11], pData, 4); /* PRQA S 3200 */
        pData = &aCmdBuf[11];

        /* Apply Padding */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
            PH_CRYPTOSYM_PADDING_MODE_2,
            &aCmdBuf[11],
            wDataLength,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            sizeof(aCmdBuf) - 11,
            &aCmdBuf[11],
            &wDataLength));
    }

    /* Build command frame */
    wCmdLength = 0;
    aCmdBuf[6 + wCmdLength++] = bCmdCode;
    aCmdBuf[6 + wCmdLength++] = (uint8_t)(wSrcBnr & 0xFF); /* LSB */
    aCmdBuf[6 + wCmdLength++] = (uint8_t)(wSrcBnr >> 8);   /* MSB */

    if ((bCmdCode == PHAL_MFP_CMD_INCRTR) ||
        (bCmdCode == PHAL_MFP_CMD_INCRTR_M) |
        (bCmdCode == PHAL_MFP_CMD_DECRTR) |
        (bCmdCode == PHAL_MFP_CMD_DECRTR_M))
    {
        aCmdBuf[6 + wCmdLength++] = (uint8_t)(wDstBnr & 0xFF); /* LSB */
        aCmdBuf[6 + wCmdLength++] = (uint8_t)(wDstBnr >> 8);   /* MSB */
    }

    /* Do we need encryption? */
    if ((bEncrypted != PH_OFF) && (pData != NULL))
    {
        /* Lets load the ENC IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Sw_Int_ComputeIv(
            PH_OFF, /* No Response*/
            pDataParams->bTi,
            pDataParams->wRCtr,
            pDataParams->wWCtr,
            aIv));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            aIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* No padding should be necessary */
        if (wDataLength % PH_CRYPTOSYM_AES_BLOCK_SIZE)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
        }

        /* Perform the Encryption */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
            pData,
            wDataLength,
            &aCmdBuf[11]
        ));

        pData = &aCmdBuf[11];
    }

    /* Buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        &aCmdBuf[6],
        wCmdLength,
        &pResponse,
        &wRxLength));

    /* Append and send the data */
    if ((wDataLength)&&(pData != NULL))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pData,
            wDataLength,
            &pResponse,
            &wRxLength));
    }

    /* Load the default IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsMac,
        phalMfp_Sw_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));  

    /* Calculate the MAC over the already available data and the current data. */

    /* First the mac consists of CMD || W_CTR || TI || PARAMS || DATA */
    aCmdBuf[0] = bCmdCode;
    aCmdBuf[1] = (uint8_t)(pDataParams->wWCtr);
    aCmdBuf[2] = (uint8_t)(pDataParams->wWCtr >> 8);
    memcpy(&aCmdBuf[3], pDataParams->bTi, PHAL_MFP_SIZE_TI); /* PRQA S 3200 */
    wCmdLength = wCmdLength + 2 /* WCtr */ + PHAL_MFP_SIZE_TI /* TI */;

    if ((wDataLength) && (pData != NULL))
    {
        /* Recopy part of the Data */
        wPartLength = PHAL_MFP_MIFARE_BLOCK_SIZE - wCmdLength;
        memcpy(&aCmdBuf[wCmdLength], pData, wPartLength); /* PRQA S 3200 */
        pData = &pData[wPartLength];
        wDataLength = wDataLength - wPartLength;

        /* Start with MAC calculation */

        /* CMAC without padding */
        /* Perform the MAC calculation for first block*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_MAC_MODE_CMAC,
            aCmdBuf,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            aMac,
            &bMacLength));

        /* Perform MAC calculation for all but the last Block */
        /* Calculate the amount of complete blocks in the final data buffer */
        wPartLength = wDataLength - (wDataLength % PH_CRYPTOSYM_AES_BLOCK_SIZE);

        if (wPartLength)
        {
            /* Perform the MAC calculation */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_MAC_MODE_CMAC,
                pData,
                wPartLength,
                aMac,
                &bMacLength));

            /* Adapt the sizes and also the pointer */
            pData = &pData[wPartLength];
            wDataLength = wDataLength - wPartLength;
        }
    }
    else
    {
        /* Set the final data to the cmd buffer */
        pData = aCmdBuf;
        wDataLength = wCmdLength;
    }

    /* Now calculate the MAC */

    /* CMAC Padding*/
    /* Calculate the MAC for the last block */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParamsMac,
        PH_EXCHANGE_BUFFER_LAST | PH_CRYPTOSYM_MAC_MODE_CMAC,
        pData,
        wDataLength,
        aMac,
        &bMacLength));

    /* Truncate MAC */
    phalMfp_Sw_Int_TruncateMac(aMac, aMac);

    /* Finally exchange the MAC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aMac,
        PHAL_MFP_TRUNCATED_MAC_SIZE,
        &pResponse,
        &wRxLength));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], PH_ON));

    /* Increment Write Counter */
    ++pDataParams->wWCtr;

    /* Check for correctly received lenght */
    if (bCmdCode & 0x01)
    {
        /* check response length */
        if (wRxLength != (1 /* Status */ + PHAL_MFP_TRUNCATED_MAC_SIZE /* MAC */))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }

        /* Let's Verify the MAC */
        aCmdBuf[0] = pResponse[0];
        aCmdBuf[1] = (uint8_t)pDataParams->wWCtr;
        aCmdBuf[2] = (uint8_t)(pDataParams->wWCtr >> 8);
        memcpy(&aCmdBuf[3], pDataParams->bTi, PHAL_MFP_SIZE_TI); /* PRQA S 3200 */

        /* Load default IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            phalMfp_Sw_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* CMAC with Padding */
        /* Calculate the MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            PH_CRYPTOSYM_MAC_MODE_CMAC,
            aCmdBuf,
            3 + PHAL_MFP_SIZE_TI,
            aMac,
            &bMacLength));

        /* Truncate the MAC */
        phalMfp_Sw_Int_TruncateMac(aMac, aMac);

        /* Compare the result */
        if (memcmp(aMac, &pResponse[1], PHAL_MFP_TRUNCATED_MAC_SIZE) != 0x00)
        {
            return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFP);
        }
    }
    else
    {
        /* check response length */
        if (wRxLength != 1 /* Status */)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_Int_ComputeIv(
                                    uint8_t bIsResponse,
                                    uint8_t * pTi,
                                    uint16_t wRCtr,
                                    uint16_t wWCtr,
                                    uint8_t * pIv
                                    )
{
    uint8_t PH_MEMLOC_REM bIndex = 0;
    uint8_t PH_MEMLOC_REM bRCtrMsb = (uint8_t)(wRCtr >> 8);
    uint8_t PH_MEMLOC_REM bRCtrLsb = (uint8_t)(wRCtr & 0x00ff);
    uint8_t PH_MEMLOC_REM bWCtrMsb = (uint8_t)(wWCtr >> 8);
    uint8_t PH_MEMLOC_REM bWCtrLsb = (uint8_t)(wWCtr & 0x00ff);

    /* parameter checking */
    if ((pTi == NULL) || (pIv == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_MFP);
    }

    /* IV = [Transaction Identifier[0-1] | Transaction Identifier[2-3] | wRCtr | wWCtr | wRCtr | wWCtr | wRCtr | wWCtr] */

#if PHAL_MFP_TAPEOUT_VERSION >= 30
    if (bIsResponse == 0)
    {
        pIv[bIndex++] = pTi[0];
        pIv[bIndex++] = pTi[1];
        pIv[bIndex++] = pTi[2];
        pIv[bIndex++] = pTi[3];
    }
#else

    pIv[bIndex++] = pTi[0];
    pIv[bIndex++] = pTi[1];
    pIv[bIndex++] = pTi[2];
    pIv[bIndex++] = pTi[3];

#endif

    pIv[bIndex++] = bRCtrLsb;
    pIv[bIndex++] = bRCtrMsb;
    pIv[bIndex++] = bWCtrLsb;
    pIv[bIndex++] = bWCtrMsb;

    pIv[bIndex++] = bRCtrLsb;
    pIv[bIndex++] = bRCtrMsb;
    pIv[bIndex++] = bWCtrLsb;
    pIv[bIndex++] = bWCtrMsb;

    pIv[bIndex++] = bRCtrLsb;
    pIv[bIndex++] = bRCtrMsb;
    pIv[bIndex++] = bWCtrLsb;
    pIv[bIndex++] = bWCtrMsb;

#if PHAL_MFP_TAPEOUT_VERSION >= 30
    if (bIsResponse != 0)
    {
        pIv[bIndex++] = pTi[0];
        pIv[bIndex++] = pTi[1];
        pIv[bIndex++] = pTi[2];
        pIv[bIndex++] = pTi[3];
    }
#else
    /* satisfy compiler*/
    if (bIsResponse);
#endif

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_Int_KdfSl2(
                                 phalMfp_Sw_DataParams_t * pDataParams,
                                 uint8_t * pRndA,
                                 uint8_t * pRndB
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bSv1[PH_CRYPTOSYM_AES_BLOCK_SIZE];

    /*
    * K_ENC = (PRF[K_SEC, RndA[39..0] || RndB[39..0] || RndA[95..56]   <+> RndB[95..56]  || 0x33])[127..80]
    */

    bSv1[0] = pRndA[11];
    bSv1[1] = pRndA[12];
    bSv1[2] = pRndA[13];
    bSv1[3] = pRndA[14];
    bSv1[4] = pRndA[15];

    bSv1[5] = pRndB[11];
    bSv1[6] = pRndB[12];
    bSv1[7] = pRndB[13];
    bSv1[8] = pRndB[14];
    bSv1[9] = pRndB[15];

    bSv1[10] = pRndA[4] ^ pRndB[4];
    bSv1[11] = pRndA[5] ^ pRndB[5];
    bSv1[12] = pRndA[6] ^ pRndB[6];
    bSv1[13] = pRndA[7] ^ pRndB[7];
    bSv1[14] = pRndA[8] ^ pRndB[8];

    bSv1[15] = 0x33;

    /* Load default init vector */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        phalMfp_Sw_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* Encrypt SV1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC,
        bSv1,
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        bSv1
        ));

    /* return the key modifier */
    memcpy(pDataParams->bKeyModifier, bSv1, 6); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_Int_KdfSl3(
                                 phalMfp_Sw_DataParams_t * pDataParams,
                                 uint8_t * pRndA,
                                 uint8_t * pRndB
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bSv1[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bSv2[PH_CRYPTOSYM_AES_BLOCK_SIZE];

    /*
    * K_ENC = PRF[K_SEC, RndA[39..0] || RndB[39..0] || RndA[95..56]  <+> RndB[95..56] || 0x11]
    * K_MAC = PRF[K_SEC, RndA[71..32] || RndB[71..32] || RndA[127..88]  <+> RndB[127..88] || 0x22]
    */

    bSv1[0] = pRndA[11];
    bSv2[0] = pRndA[7];

    bSv1[1] = pRndA[12];
    bSv2[1] = pRndA[8];

    bSv1[2] = pRndA[13];
    bSv2[2] = pRndA[9];

    bSv1[3] = pRndA[14];
    bSv2[3] = pRndA[10];

    bSv1[4] = pRndA[15];
    bSv2[4] = pRndA[11];


    bSv1[5] = pRndB[11];
    bSv2[5] = pRndB[7];

    bSv1[6] = pRndB[12];
    bSv2[6] = pRndB[8];

    bSv1[7] = pRndB[13];
    bSv2[7] = pRndB[9];

    bSv1[8] = pRndB[14];
    bSv2[8] = pRndB[10];

    bSv1[9] = pRndB[15];
    bSv2[9] = pRndB[11];


    bSv1[10] = pRndA[4] ^ pRndB[4];
    bSv2[10] = pRndA[0] ^ pRndB[0];

    bSv1[11] = pRndA[5] ^ pRndB[5];
    bSv2[11] = pRndA[1] ^ pRndB[1];

    bSv1[12] = pRndA[6] ^ pRndB[6];
    bSv2[12] = pRndA[2] ^ pRndB[2];

    bSv1[13] = pRndA[7] ^ pRndB[7];
    bSv2[13] = pRndA[3] ^ pRndB[3];

    bSv1[14] = pRndA[8] ^ pRndB[8];
    bSv2[14] = pRndA[4] ^ pRndB[4];

    bSv1[15] = 0x11;
    bSv2[15] = 0x22;

    /* Encrypt SV1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_ECB,
        bSv1,
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        bSv1
        ));

    /* Encrypt SV2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_ECB,
        bSv2,
        PH_CRYPTOSYM_AES_BLOCK_SIZE,
        bSv2
        ));

    /* Load Key SV1 ENC key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bSv1,
        PH_CRYPTOSYM_KEY_TYPE_AES128));

    /* Load Key SV2 as MAC key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsMac,
        bSv2,
        PH_CRYPTOSYM_KEY_TYPE_AES128));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

void phalMfp_Sw_Int_TruncateMac(
                                uint8_t * pMac,
                                uint8_t * pTruncatedMac
                                )
{
#if PHAL_MFP_TAPEOUT_VERSION >= 20

    uint8_t PH_MEMLOC_REM bIndex;

    /* truncated MAC = [1, 3, 5, 7, 9, 11, 13, 15] of the input Mac */
    for (bIndex = 0; bIndex < PHAL_MFP_TRUNCATED_MAC_SIZE; ++bIndex)
    {
        pTruncatedMac[bIndex] = pMac[1 + (bIndex << 1)];
    }

#else

    /* truncated MAC = 8 MSB of the input Mac */
    memcpy(pTruncatedMac, pMac, PHAL_MFP_TRUNCATED_MAC_SIZE); /* PRQA S 3200 */

#endif
}

phStatus_t phalMfp_Sw_Cmd_PrepareProximityCheck(
    phalMfp_Sw_DataParams_t * pDataParams
    )
{
    return(phalMfp_Int_PrepareProximityCheck(pDataParams->pPalMifareDataParams));
}

phStatus_t phalMfp_Sw_Cmd_ProximityCheck(
    phalMfp_Sw_DataParams_t * pDataParams,
    uint8_t bGenerateRndC,
    uint8_t * pRndC,
    uint8_t bNumSteps,
    uint8_t * pUsedRndRC
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRndC[PHAL_MFP_PC_RND_LEN];

    /* parameter checking */
    if (((bGenerateRndC == 0) && (pRndC == NULL)) || (pUsedRndRC == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* check whether to generate RndC or not */
    if (bGenerateRndC)
    {
        pRndC = bRndC;
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(
            pDataParams->pCryptoRngDataParams,
            PHAL_MFP_PC_RND_LEN,
            pRndC
            ));
    }

    /* send "Proximity Check" command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ProximityCheck(
        pDataParams->pPalMifareDataParams,
        bNumSteps,
        pRndC,
        pUsedRndRC));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Sw_Cmd_VerifyProximityCheck(
    phalMfp_Sw_DataParams_t * pDataParams,
    uint8_t * pRndRC,
    uint8_t bPps1
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM aTmpBuf[2 + PHAL_MFP_PC_RND_LEN * 2];
    uint8_t     PH_MEMLOC_REM aMac[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bMacLength;

    /* prepare "Verify Proximity Check" command */
    aTmpBuf[0] = PHAL_MFP_CMD_VPC;
    aTmpBuf[1] = bPps1;
    memcpy(&aTmpBuf[2], pRndRC, PHAL_MFP_PC_RND_LEN * 2); /* PRQA S 3200 */

    /* CMAC with padding */
    /* mac calculation: CMAC(CMD || PPS1 || (RndRC1 || ... || RndRC14)) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParamsMac,
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aTmpBuf,
        (2 + PHAL_MFP_PC_RND_LEN * 2),
        aMac,
        &bMacLength));    

    /* perform MAC truncation */
    phalMfp_Sw_Int_TruncateMac(aMac, aMac);

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
        aMac,
        PHAL_MFP_TRUNCATED_MAC_SIZE,
        &pResponse,
        &wRxLength));

    /* check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 1));

    /* check response length */
    if (wRxLength != 1 /* Status */ + 8 /* MAC */)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    /* Calculate MAC */
    aTmpBuf[0] = pResponse[0];
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParamsMac,
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aTmpBuf,
        (2 + PHAL_MFP_PC_RND_LEN * 2),
        aMac,
        &bMacLength));    

    /* perform MAC truncation */
    phalMfp_Sw_Int_TruncateMac(aMac, aMac);

    /* Compare MAC */
    if (memcmp(aMac, &pResponse[1], PHAL_MFP_TRUNCATED_MAC_SIZE) != 0)
    {
        return PH_ADD_COMPCODE(PHAL_MFP_ERR_AUTH, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

#endif /* NXPBUILD__PHAL_MFP_SW */
