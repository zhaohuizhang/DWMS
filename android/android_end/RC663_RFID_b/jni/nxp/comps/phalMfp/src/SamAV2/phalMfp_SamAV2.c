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
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:26:50 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phalMfp.h>
#include <phpalMifare.h>
#include <phKeyStore.h>
#include <phpalI14443p3a.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFP_SAMAV2

#include "phalMfp_SamAV2.h"
#include "phalMfp_SamAV2_Int.h"
#include "../phalMfp_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalMfp_SamAV2_Init(
                               phalMfp_SamAV2_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams, 
                               phhalHw_SamAV2_DataParams_t * pHalSamDataParams,
                               void * pPalMifareDataParams
                               )
{
    if (sizeof(phalMfp_SamAV2_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);
	PH_ASSERT_NULL (pHalSamDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFP | PHAL_MFP_SAMAV2_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pHalSamDataParams      = pHalSamDataParams;
    memset(pDataParams->bKeyModifier, 0x00, PHAL_MFP_SIZE_KEYMODIFIER);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_WritePerso(
                                     phalMfp_SamAV2_DataParams_t * pDataParams,
                                     uint8_t bLayer4Comm,
                                     uint16_t wBlockNr,
                                     uint8_t * pValue
                                     )
{
    return phalMfp_Int_WritePerso(pDataParams->pPalMifareDataParams, bLayer4Comm, wBlockNr, pValue);
}

phStatus_t phalMfp_SamAV2_CommitPerso(
                                      phalMfp_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bLayer4Comm
                                      )
{
    return phalMfp_Int_CommitPerso(pDataParams->pPalMifareDataParams, bLayer4Comm);
}

phStatus_t phalMfp_SamAV2_AuthenticateSL0(
    phalMfp_SamAV2_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_AuthenticateGeneral(
        pDataParams,
        bLayer4Comm,
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion,
        bLenDivInput,
        pDivInput,
        0x00, /* No SL2 KDF */
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out, 
        pPdCap2);
}

phStatus_t phalMfp_SamAV2_AuthenticateSL1(
    phalMfp_SamAV2_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_AuthenticateGeneral(
        pDataParams,
        bLayer4Comm,
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion, 
        bLenDivInput,
        pDivInput,
        0x00, /* No SL2 KDF */
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out, 
        pPdCap2);
}

phStatus_t phalMfp_SamAV2_AuthenticateSL2(
    phalMfp_SamAV2_DataParams_t * pDataParams,
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
        bUseKdfSl2 = 0;
    }
    else
    {
        bUseKdfSl2 = 1;
    }

    /* Perform actual authentication */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_AuthenticateGeneral(
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
    memcpy(pKmf, pDataParams->bKeyModifier, PHAL_MFP_SIZE_KEYMODIFIER);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_AuthenticateClassicSL2(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    )
{
    return phpalMifare_MfcAuthenticateKeyNo(
        pDataParams->pPalMifareDataParams,
        bBlockNo,
        bKeyType,
        wKeyNumber,
        wKeyVersion,
        &pUid[bUidLength- 4]);
}

phStatus_t phalMfp_SamAV2_MultiBlockRead(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNr,
    uint8_t bNumBlocks,
    uint8_t * pBlocks
    )
{
    return phalMfp_Int_MultiBlockRead(pDataParams->pPalMifareDataParams, bBlockNr, bNumBlocks, pBlocks);
}

phStatus_t phalMfp_SamAV2_MultiBlockWrite(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNr,
    uint8_t bNumBlocks,
    uint8_t * pBlocks
    )
{
    return phalMfp_Int_MultiBlockWrite(pDataParams->pPalMifareDataParams, bBlockNr, bNumBlocks, pBlocks);
}

phStatus_t phalMfp_SamAV2_Write(
                                phalMfp_SamAV2_DataParams_t * pDataParams,
                                uint8_t bEncrypted,
                                uint8_t bWriteMaced,
                                uint16_t wBlockNr,
                                uint8_t bNumBlocks,
                                uint8_t * pBlocks
                                )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bWriteMaced)
    {
        if (bEncrypted)
        {
            bCmd = PHAL_MFP_CMD_WRITE_EM;
        }
        else
        {
            bCmd = PHAL_MFP_CMD_WRITE_PM;
        }
    }
    else
    {
        if (bEncrypted)
        {
            bCmd = PHAL_MFP_CMD_WRITE_EN;
        }
        else
        {
            bCmd = PHAL_MFP_CMD_WRITE_PN;
        }
    }

    /* Perform actual write operation*/
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        pBlocks,
        (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE));
}

phStatus_t phalMfp_SamAV2_WriteValue(
                                     phalMfp_SamAV2_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_Write(pDataParams, bEncrypted, bWriteMaced, wBlockNr, 1, pBlock);
}

phStatus_t phalMfp_SamAV2_ChangeKey(
                                    phalMfp_SamAV2_DataParams_t * pDataParams,  
                                    uint8_t bChangeKeyMaced, 
                                    uint16_t wBlockNr,   
                                    uint16_t wKeyNumber, 
                                    uint16_t wKeyVersion,
                                    uint8_t bLenDivInput,
                                    uint8_t * pDivInput  
                                    )
{
    phStatus_t statusTmp;
    uint8_t bCmd[3];
    uint8_t * pResponse;
    uint16_t wResponseLength;
    uint16_t wRxLength;

    /* Evaluate command code */
    if (bChangeKeyMaced)
    {
        bCmd[0] = PHAL_MFP_CMD_WRITE_EM;
    }
    else
    {
        bCmd[0] = PHAL_MFP_CMD_WRITE_EN;
    }

    bCmd[1] = (uint8_t)(wBlockNr & 0xff); /* LSB */
    bCmd[2] = (uint8_t)(wBlockNr >> 8);   /* MSB */

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyMfp_Part1(
        pDataParams->pHalSamDataParams, 
        bCmd[0], 
        wBlockNr,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        pDivInput,
        bLenDivInput,
        &pResponse,
        &wResponseLength));

    /* First Exchange Command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmd,
        3,
        NULL,
        NULL));

    /* Then Exchange Data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pResponse,
        wResponseLength,
        &pResponse,
        &wRxLength));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 0x01));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyMfp_Part2(
        pDataParams->pHalSamDataParams, 
        pResponse,
        (uint8_t) wRxLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_AuthenticateSL3(
    phalMfp_SamAV2_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_AuthenticateGeneral(
        pDataParams,
        0x01,
        bFirstAuth, 
        wBlockNr, 
        wKeyNumber,
        wKeyVersion,
        bLenDivInput,
        pDivInput,
        0x00, /* No SL2 KDF */
        bLenPcdCap2,
        pPcdCap2In, 
        pPcdCap2Out, 
        pPdCap2);
}

phStatus_t phalMfp_SamAV2_Read(
                               phalMfp_SamAV2_DataParams_t * pDataParams,
                               uint8_t bEncrypted,
                               uint8_t bReadMaced,
                               uint8_t bMacOnCmd,
                               uint16_t wBlockNr,
                               uint8_t bNumBlocks,
                               uint8_t * pBlocks
                               )
{
    phStatus_t statusTmp, status;
    uint8_t pCmd[4];
    uint16_t wTxLength = 0;
    uint16_t wRxLength;
    uint8_t * pResponse = NULL;
    uint8_t finished = 0;
    uint8_t cmdOrResponse;
    uint16_t wTxOption = PH_EXCHANGE_DEFAULT;
    uint16_t wNumBlocksStartPos = 0;
    uint16_t wResponseLength;
    uint8_t * pPlainResponse = NULL;
    uint16_t wPlainRxLength;

    pCmd[wTxLength++] = PHAL_MFP_CMD_READ_ENM;
    if (bReadMaced)
    {
        pCmd[0] |= 0x01;
    }
    if (!bEncrypted)
    {
        pCmd[0] |= 0x02;
    }
    if (!bMacOnCmd)
    {
        pCmd[0] |= 0x04;
    }

    pCmd[wTxLength++] = (uint8_t)(wBlockNr & 0xff); /* LSB */
    pCmd[wTxLength++] = (uint8_t)(wBlockNr >> 8);   /* MSB */
    pCmd[wTxLength++] = bNumBlocks;

    /* Prepare for MAC on cmd */
    if (bMacOnCmd)
    {
        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            pCmd,
            wTxLength,
            &pResponse,
            &wRxLength));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_CombinedReadMfp(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT | PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND,
            pCmd,
            (uint8_t)wTxLength,
            &pResponse,
            &wResponseLength
            ));

        /* command exchange */
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pResponse,
            wResponseLength,
            &pResponse,
            &wRxLength);

        cmdOrResponse = PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_RESPONSE;

    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_CombinedReadMfp(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND_RESPONSE,
            pCmd,
            (uint8_t)wTxLength,  
            &pResponse,
            &wResponseLength
            ));

        /* command exchange */
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            pCmd,
            wTxLength,
            &pResponse,
            &wRxLength);

        cmdOrResponse = PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND;
        wTxOption = PH_EXCHANGE_BUFFER_LAST;
    }

    /* Error Check */
    if ((status & PH_ERR_MASK ) != PH_ERR_SUCCESS_CHAINING)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Check the return Code */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 0x01));

    /* Start the chaining loop */
    do
    {
        if (!bEncrypted)
        {
            pPlainResponse = pResponse;
            wPlainRxLength = wRxLength;
            if ((wNumBlocksStartPos == 0) && (wPlainRxLength))
            {
                pPlainResponse++;
                wPlainRxLength--;
            }

            /* We should decrease the response length in case of MAC */
            if ((bReadMaced) && ((wNumBlocksStartPos + wPlainRxLength) > (bNumBlocks << 4)))
            {
                if (wPlainRxLength < 8)
                {
					/* if response length is less than MAC length (8 bytes) throw an error. */
                    return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
                }
                wPlainRxLength = wPlainRxLength - 8;
            }

            memcpy(&pBlocks[wNumBlocksStartPos], pPlainResponse, wPlainRxLength);  /* PRQA S 3200 */
            wNumBlocksStartPos = wPlainRxLength + wNumBlocksStartPos;
        }

        /* Is this already the last block? */
        if (status == PH_ERR_SUCCESS)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_CombinedReadMfp(
                pDataParams->pHalSamDataParams,
                wTxOption | cmdOrResponse,
                pResponse,
                (uint8_t)wRxLength,
                &pResponse,
                &wResponseLength
                ));

            finished = 1;
        }
        else
        {
            status = phhalHw_SamAV2_Cmd_SAM_CombinedReadMfp(
                pDataParams->pHalSamDataParams,
                wTxOption | PH_EXCHANGE_TXCHAINING | cmdOrResponse,
                pResponse,
                (uint8_t)wRxLength,
                &pResponse,
                &wResponseLength
                );

            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                return status;
            }
        }
        cmdOrResponse = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        wTxOption = PH_EXCHANGE_DEFAULT; 

        if (bEncrypted)
        {
            if ((wNumBlocksStartPos + wResponseLength) > (bNumBlocks << 4))
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
            }
            memcpy(&pBlocks[wNumBlocksStartPos], pResponse, wResponseLength);  /* PRQA S 3200 */
            wNumBlocksStartPos = wNumBlocksStartPos + wResponseLength;
        }

        if (!finished)
        {
            /* Continue with next Data block */
            /* command exchange */
            status = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_RXCHAINING,
                NULL,
                0,
                &pResponse,
                &wRxLength);

            /* Error Check */
            if (((status & PH_ERR_MASK ) != PH_ERR_SUCCESS) &&
                ((status & PH_ERR_MASK ) != PH_ERR_SUCCESS_CHAINING))
            {
                /* finish SAM chaining with KillAuthenticate command */
                /* Kill only card Auth */
                statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01); 
                return status;
            }
        }
    }
    while (!finished);

    if ((wNumBlocksStartPos) != (bNumBlocks << 4))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_ReadValue(
                                    phalMfp_SamAV2_DataParams_t * pDataParams,
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
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_Read(pDataParams, bEncrypted, bReadMaced, bMacOnCmd, wBlockNr, 1, pData));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_CheckValueBlockFormat(pData));
    *pAddrData = pData[12];
    memcpy(pValue, pData, 4); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_ResetAuth(
                                    phalMfp_SamAV2_DataParams_t * pDataParams
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Call Reset Auth function */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ResetAuth(pDataParams->pPalMifareDataParams));

    /* Reset the crypto layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_ResetSecMsgState(pDataParams));

    /* return exchange status code */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_Increment(
                                    phalMfp_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bIncrementMaced,
                                    uint16_t wBlockNr,
                                    uint8_t * pValue
                                    )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bIncrementMaced)
    {
        bCmd = PHAL_MFP_CMD_INCR_M;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_INCR;
    }

    /* Perform actual write operation */
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_Decrement(
                                    phalMfp_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bDecrementMaced,
                                    uint16_t wBlockNr,
                                    uint8_t * pValue
                                    )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bDecrementMaced)
    {
        bCmd = PHAL_MFP_CMD_DECR_M;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_DECR;
    }

    /* Perform actual write operation */
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_IncrementTransfer(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bIncrementTransferMaced,
    uint16_t wSourceBlockNr,
    uint16_t wDestinationBlockNr,
    uint8_t * pValue
    )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bIncrementTransferMaced)
    {
        bCmd = PHAL_MFP_CMD_INCRTR_M;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_INCRTR;
    }

    /* Perform actual write operation */
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wSourceBlockNr,
        wDestinationBlockNr,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_DecrementTransfer(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bDecrementTransferMaced,
    uint16_t wSourceBlockNr,
    uint16_t wDestinationBlockNr,
    uint8_t * pValue
    )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bDecrementTransferMaced)
    {
        bCmd = PHAL_MFP_CMD_DECRTR_M;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_DECRTR;
    }

    /* Perform actual write operation */
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wSourceBlockNr,
        wDestinationBlockNr,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_Transfer(
                                   phalMfp_SamAV2_DataParams_t * pDataParams,
                                   uint8_t bTransferMaced,
                                   uint16_t wBlockNr
                                   )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bTransferMaced)
    {
        bCmd = PHAL_MFP_CMD_TRANS_M;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_TRANS;
    }

    /* Perform actual write operation */
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        NULL,
        0);
}

phStatus_t phalMfp_SamAV2_Restore(
                                  phalMfp_SamAV2_DataParams_t * pDataParams,
                                  uint8_t bRestoreMaced,
                                  uint16_t wBlockNr
                                  )
{
    uint8_t PH_MEMLOC_REM bCmd;

    /* Evaluate command code */
    if (bRestoreMaced)
    {
        bCmd = PHAL_MFP_CMD_REST_M;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_REST;
    }

    /* Perform actual write operation */
    return phalMfp_SamAV2_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        NULL,
        0);
}

phStatus_t phalMfp_SamAV2_ProximityCheck(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t     bGenerateRndC,
    uint8_t*    pRndC, 
    uint8_t     bPps1,
    uint8_t     bNumSteps,
    uint8_t*    pUsedRndC
    )
{
    /* local variables */
    phStatus_t status;
    phStatus_t statusTmp;
    uint8_t pRndRC[14];
    uint8_t pRndC_Local[7];
    uint8_t * pResponse = NULL;
    uint16_t wRxLength = 0;
    uint8_t pCmd[9];

    /* parameter checking */
    if ((bGenerateRndC == 0) && (pRndC == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_MFP);
    }

    /* send "Prepare Proximity Check" command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_PrepareProximityCheck(pDataParams->pPalMifareDataParams));

    if (bGenerateRndC)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetChallenge(pDataParams->pHalSamDataParams, 7, pRndC_Local));
    }
    else
    {
        memcpy(pRndC_Local, pRndC, 7);  /* PRQA S 3200 */
    }

    /* send "Proximity Check" command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ProximityCheck(
        pDataParams->pPalMifareDataParams,
        bNumSteps,
        pRndC_Local,
        pRndRC));

    /* send "Verify Proximity Check" command */
    status = phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part1(
        pDataParams->pHalSamDataParams,
        0xFF,
        0xFF,
        bPps1,
        pRndRC,
        NULL,
        0,
        &pCmd[1]);

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        return status;
    }

    pCmd[0] = PHAL_MFP_CMD_VPC;

    /* command exchange */
    status = phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        pCmd,
        9,
        &pResponse,
        &wRxLength);

    /* Check status */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* finish SAM chaining with KillAuthenticate command */
        /* Kill only card Auth */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
        return status;
    }

    /* Check the status Code */
    status = phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 0x01);
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* finish SAM chaining with KillAuthenticate command */
        /* Kill only card Auth */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
        return status;
    }

    /* check response length */
    if (wRxLength != 1 /* Status */ + 8 /* MAC */)
    {
        /* finish SAM chaining with KillAuthenticate command */
        /* Kill only card Auth */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    /* send "Verify Proximity Check" command */
    status = phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part2(
        pDataParams->pHalSamDataParams,
        &pResponse[1]);

    if ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_CRYPTO)
    {
        return PH_ADD_COMPCODE(PHAL_MFP_ERR_AUTH, PH_COMP_AL_MFP);
    }

    PH_CHECK_SUCCESS(status);

    if (pUsedRndC != NULL)
    {
        memcpy(pUsedRndC, pRndC_Local, 7);  /* PRQA S 3200 */
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_ResetSecMsgState(
    phalMfp_SamAV2_DataParams_t * pDataParams
    )
{
    /* Kill only card Auth */
    return phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
}

phStatus_t phalMfp_SamAV2_AuthenticateGeneral(
    phalMfp_SamAV2_DataParams_t * pDataParams,
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
    /* local variables */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM bRxLength;
    uint8_t     PH_MEMLOC_REM bValidBits = 0;
    uint8_t     PH_MEMLOC_REM bHeader[40];
    uint8_t *   PH_MEMLOC_REM pHeader;
    uint16_t    PH_MEMLOC_REM wHeaderLength = 0;
    uint16_t    PH_MEMLOC_REM wMfCryptoOff;
    uint8_t     PH_MEMLOC_REM bOption = 0;

    /* parameter checking */
    if (((bLenPcdCap2 > 0) && (pPcdCap2In == NULL)) || (bLenPcdCap2 > 6))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* Get status of Mifare Crypto */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1,
        &wMfCryptoOff));

    /* Create First Part of the Message */
    if (bFirstAuth)
    {
        bHeader[wHeaderLength++] = PHAL_MFP_CMD_AUTH1_FIRST;
    }
    else
    {
        bHeader[wHeaderLength++] = PHAL_MFP_CMD_AUTH1;
    }

    /* wBlockNr */
    bHeader[wHeaderLength++] = (uint8_t)(wBlockNr & 0xff); /* LSB */
    bHeader[wHeaderLength++] = (uint8_t)(wBlockNr >> 8);   /* MSB */

    /* Add pcd caps in case of auth first */
    if (bFirstAuth == 1)
    {
        bHeader[wHeaderLength++] = bLenPcdCap2;
        wHeaderLength = bLenPcdCap2 + wHeaderLength;
    }

    /* exchange command/response */
    if (bLayer4Comm)
    {
        /* buffer the header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            bHeader,
            wHeaderLength,
            &pResponse,
            &wRxLength));

        /* append the PcdCaps and exchange frame */
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
        /* MIFARE Crypto is disabled: use ISO3 exchange*/
        if (wMfCryptoOff)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_DEFAULT,
                bHeader,
                wHeaderLength,
                &pResponse,
                &wRxLength));
        }
        /* MIFARE Crypto is enabled: use Exchange Raw and retrieve encrypted response */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeRaw(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_DEFAULT | PHHAL_HW_SAMAV2_EXCHANGE_NO_DECIPHERING_BIT,
                bHeader,
                wHeaderLength,
                0,
                &pResponse,
                &wRxLength,
                &bValidBits));

            /* NACK Handling */
            if ((wRxLength == 1) && (bValidBits == 4))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], PH_OFF));
            }
        }
    }

    /* MIFARE Crypto is off */
    if (wMfCryptoOff)
    {
        /* Check the status code */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], bLayer4Comm));

        /* Check response length */
        if (wRxLength != (1 /* status */ + 16 /* ENC(RNDB) */))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }

        /* Skip the status byte in case of unencrypted operation */
        pResponse = &pResponse[1];
        pHeader = &bHeader[1];
    }
    /* MIFARE Crypto is on */
    else
    {
        /* status code check is done in the HAL, nothing to do here */

        /* Check response length */
        if (wRxLength != (1 /* status */ + 16 /* ENC(RNDB) */ + 2 /* CRC */ + 3 /* Parity */) ||
            (bValidBits != 3))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }

        /* status byte is also generated in encrypted mode */
        pHeader = &bHeader[0];
    }

    /* Key derrivation */
    if (bUseKdfSl2)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL2_KDF;
    }
    else if (!bLayer4Comm)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_NO_KDF;
    }
    else if ((wBlockNr & 0xF000) == 0x8000)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_NO_KDF;
    }
    else
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL3_KDF;
    }

    if (bFirstAuth)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_AUTH_FIRST;
    }

    if (!wMfCryptoOff)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_MF_ENCIPHERED;
    }

    /* Process the response */
    status = phhalHw_SamAV2_Cmd_SAM_AuthenticateMfp_Part1(
        pDataParams->pHalSamDataParams,
        bOption,
        (uint8_t) wKeyNumber,
        (uint8_t) wKeyVersion,
        pResponse,
        pDivInput,
        bLenDivInput,
        pHeader,
        &bRxLength
        );

    /* status check */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        /* finish SAM chaining with KillAuthenticate command */
        if ((status & PH_ERR_MASK) == PH_ERR_LENGTH_ERROR)
        {
            /* Kill only card Auth */
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
        }
        return status;
    }

    /* Start with part 2 of Authenticate MFP */

    /* MIFARE Crypto is disabled: Compute second part of the Auth sequence */
    if (wMfCryptoOff)
    {
        if (bRxLength != 32)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }
        bHeader[0] = PHAL_MFP_CMD_AUTH2;
        wHeaderLength = 33;
    }
    /* MIFARE Crypto is enabled: Second part of the Auth sequence is already computed including crypto */
    else
    {
        if (bRxLength != 40)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
        }
        wHeaderLength = 40;
        bValidBits = 3;
    }

    /* command exchange */
    if (bLayer4Comm)
    {
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            bHeader,
            wHeaderLength,
            &pResponse,
            &wRxLength);
    }
    else
    {
        /* MIFARE Crypto is disabled: use ISO3 exchange*/
        if (wMfCryptoOff)
        {
            status = phpalMifare_ExchangeL3(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_DEFAULT,
                bHeader,
                wHeaderLength,
                &pResponse,
                &wRxLength);
        }
        /* MIFARE Crypto is enabled: use Exchange Raw and transmit / retrieve encrypted response */
        else
        {
            status = phpalMifare_ExchangeRaw(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_DEFAULT | PHHAL_HW_SAMAV2_EXCHANGE_NO_ENCIPHERING_BIT | PHHAL_HW_SAMAV2_EXCHANGE_NO_DECIPHERING_BIT,
                bHeader,
                wHeaderLength,
                bValidBits,
                &pResponse,
                &wRxLength,
                &bValidBits);

            /* NACK Handling */
            if ((wRxLength == 1) && (bValidBits == 4))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], PH_OFF));
            }
        }
    }

    /* Status check */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* finish SAM chaining with KillAuthenticate command */
        /* Kill only card Auth */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
        return status;
    }

    /* MIFARE Crypto is off */
    if (wMfCryptoOff)
    {
        /* Check the status Code */
        status = phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], bLayer4Comm);
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            /* finish SAM chaining with KillAuthenticate command */
            /* Kill only card Auth */
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalSamDataParams, 0x01);
            return status;
        }
        /* No need to check response length here, this is done byte the processing below */

        /* Skip the status byte in case of unencrypted operation */
        pResponse = &pResponse[1];
        --wRxLength;
    }
    /* MIFARE Crypto is on */
    else
    {
        /* status code check is done in the HAL, nothing to do here */

        /* No need to check response length here, this is done byte the processing below */
    }

    /* Process second part response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_AuthenticateMfp_Part2(
        pDataParams->pHalSamDataParams,
        pResponse,
        (uint8_t)(wRxLength),
        pPdCap2,
        pPcdCap2Out
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_WriteExt(
                                   phalMfp_SamAV2_DataParams_t * pDataParams,
                                   uint8_t bCmdCode,
                                   uint16_t wSrcBnr,
                                   uint16_t wDstBnr,
                                   uint8_t * pData,
                                   uint8_t bDataLength
                                   )
{
    phStatus_t statusTmp;
    uint8_t pCmd[5];
    uint16_t wTxLength = 0;
    uint16_t wRxLength = 0;
    uint8_t * pResponse = NULL;
    uint16_t wResponseLength;

    pCmd[wTxLength++] = bCmdCode;
    pCmd[wTxLength++] = (uint8_t)(wSrcBnr & 0xff); /* LSB */
    pCmd[wTxLength++] = (uint8_t)(wSrcBnr >> 8);   /* MSB */

    if ((bCmdCode == PHAL_MFP_CMD_INCRTR) ||
        (bCmdCode == PHAL_MFP_CMD_INCRTR_M) |
        (bCmdCode == PHAL_MFP_CMD_DECRTR) |
        (bCmdCode == PHAL_MFP_CMD_DECRTR_M))
    {
        pCmd[wTxLength++] = (uint8_t)(wDstBnr & 0xff); /* LSB */
        pCmd[wTxLength++] = (uint8_t)(wDstBnr >> 8);   /* MSB */
    }

    /* First lets send the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        pCmd,
        wTxLength,
        &pResponse,
        &wRxLength));

    /* Lets process the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_CombinedWriteMfp(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_COMMAND,
        pCmd,
        (uint8_t)wTxLength,
        &pResponse,
        &wResponseLength
        ));

    /* Lets process the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_CombinedWriteMfp(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_LAST | PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_COMMAND,
        pData,
        bDataLength,
        &pResponse,
        &wResponseLength
        ));

    /* Finally exchange the data and the MAC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pResponse,
        wResponseLength,
        &pResponse,
        &wRxLength));

    /* Check the status Code */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 0x01));

    /* Lets process the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_CombinedWriteMfp(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT | PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_RESPONSE,
        pResponse,
        (uint8_t)wRxLength,
        &pResponse, 
        &wResponseLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */
