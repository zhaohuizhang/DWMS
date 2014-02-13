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
* Software MIFARE Plus(R) Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:01 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phalMfp.h>
#include <phpalMifare.h>
#include <ph_RefDefs.h>
#include <phTools.h>

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X

#include "../phalMfp_Int.h"
#include "phalMfp_SamAV2_X.h"
#include "phalMfp_SamAV2_X_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

static const uint8_t PH_MEMLOC_CONST_ROM gaPaddingBlock[] =
{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

phStatus_t phalMfp_SamAV2_X_Init(
                                 phalMfp_SamAV2_X_DataParams_t * pDataParams,
                                 uint16_t wSizeOfDataParams, 
                                 phhalHw_SamAV2_DataParams_t * pHalDataParams,
                                 phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams
                                 )
{
    /* Check Dataparams */
    if (sizeof(phalMfp_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);
    PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                        = PH_COMP_AL_MFP | PHAL_MFP_SAMAV2_X_ID;
    pDataParams->pHalDataParams             = pHalDataParams;
    pDataParams->pPalMifareDataParams       = pPalMifareDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_WritePerso(
                                       phalMfp_SamAV2_X_DataParams_t * pDataParams,
                                       uint8_t bLayer4Comm,
                                       uint16_t wBlockNr,
                                       uint8_t * pValue
                                       )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM pEntryPair[2 + PHAL_MFP_MIFARE_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bResponseCode;

    /* Sam natively only supprots L4 WRITE Perso writing*/
    if (bLayer4Comm)
    {
        pEntryPair[0] = (uint8_t)(wBlockNr);
        pEntryPair[1] = (uint8_t)(wBlockNr >> 8);

        memcpy(&pEntryPair[2], pValue, PHAL_MFP_MIFARE_BLOCK_SIZE);  /* PRQA S 3200 */

        status = phhalHw_SamAV2_Cmd_MfpWritePerso(
            pDataParams->pHalDataParams,
            pEntryPair,
            2 + PHAL_MFP_MIFARE_BLOCK_SIZE,
            &bResponseCode);

        /* Convert HAL status codes to palMifare status codes */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

        /* Check Card response*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, bLayer4Comm));

    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_WritePerso(
            pDataParams->pPalMifareDataParams,
            bLayer4Comm,
            wBlockNr,
            pValue));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_CommitPerso(
                                        phalMfp_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t bLayer4Comm
                                        )
{
    return phalMfp_Int_CommitPerso(pDataParams->pPalMifareDataParams, bLayer4Comm);
}

phStatus_t phalMfp_SamAV2_X_AuthenticateSL0(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bOption = 0;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    if (bFirstAuth)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST;
    }
    bOption |= PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_NO_KDF;

    status = phhalHw_SamAV2_Cmd_MfpAuthenticate(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNumber, 
        (uint8_t)wKeyVersion,
        wBlockNr,
        pPcdCap2In,
        bLenPcdCap2,
        pDivInput,
        bLenDivInput,
        pPcdCap2Out,
        pPdCap2,
        &bResponseCode); 

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    /* Check Card response*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, bLayer4Comm));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_AuthenticateSL1(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bOption = 0;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    if (bFirstAuth)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST;
    }
    bOption |= PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_NO_KDF;

    status = phhalHw_SamAV2_Cmd_MfpAuthenticate(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNumber, 
        (uint8_t)wKeyVersion,
        wBlockNr,
        pPcdCap2In, 
        bLenPcdCap2,
        pDivInput,
        bLenDivInput,
        pPcdCap2Out,
        pPdCap2,
        &bResponseCode);

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    /* Check Card response*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, bLayer4Comm));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_AuthenticateSL2(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bOption = 0;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    /* satisfy compiler */
    pKmf = NULL;

    if (bFirstAuth)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST;
    }

    if (bLayer4Comm)
    {
        status = phhalHw_SamAV2_Cmd_MfpAuthenticate(
            pDataParams->pHalDataParams,
            bOption | PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL3_KDF,
            (uint8_t)wKeyNumber, 
            (uint8_t)wKeyVersion,
            wBlockNr,
            pPcdCap2In,
            bLenPcdCap2,
            pDivInput,
            bLenDivInput,
            pPcdCap2Out,
            pPdCap2,
            &bResponseCode);
    }
    else
    {
        status = phhalHw_SamAV2_Cmd_MfpAuthenticate(
            pDataParams->pHalDataParams,
            bOption | PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL2_KDF,
            (uint8_t)wKeyNumber, 
            (uint8_t)wKeyVersion,
            wBlockNr,
            pPcdCap2In, 
            bLenPcdCap2,
            pDivInput,
            bLenDivInput,
            pPcdCap2Out,
            pPdCap2,
            &bResponseCode);
    }

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, bLayer4Comm));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_AuthenticateClassicSL2(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    )
{
    /* Check paramters */
    if ((wKeyNumber > 0xFF) || (wKeyVersion > 0xFF))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* Evaluate Key-Type */
    if (bKeyType == PHAL_MFP_KEYA)
    {
        bKeyType = PHPAL_MIFARE_KEYA;
    }
    else
    {
        bKeyType = PHPAL_MIFARE_KEYB;
    }

    /* Key modifying is done autmatically in SAM */
    return phpalMifare_MfcAuthenticateKeyNo(
        pDataParams->pPalMifareDataParams,
        bBlockNo,
        bKeyType,
        wKeyNumber,
        wKeyVersion,
        &pUid[bUidLength - 4]);    
}

phStatus_t phalMfp_SamAV2_X_MultiBlockRead(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNr,
    uint8_t bNumBlocks,
    uint8_t * pBlocks
    )
{
    phStatus_t PH_MEMLOC_REM status;
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bNumBlocksBnr[2];
    uint8_t *  PH_MEMLOC_REM pRxBuffer;
    uint16_t   PH_MEMLOC_REM wRxBufferLength;

    bNumBlocksBnr[0] = bBlockNr;
    bNumBlocksBnr[1] = bNumBlocks;

    status = phhalHw_SamAV2_Cmd_MfRead(
        pDataParams->pHalDataParams,
        PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_MULTIBLOCK,
        bNumBlocksBnr, 
        2, 
        &pRxBuffer,
        &wRxBufferLength
        );

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    memcpy(pBlocks, pRxBuffer, wRxBufferLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_MultiBlockWrite(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNr,
    uint8_t bNumBlocks,
    uint8_t * pBlocks
    )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t PH_MEMLOC_REM pBuffer[2];

    pBuffer[0] = bBlockNr;
    pBuffer[1] = bNumBlocks;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MULTIBLOCK,
        pBuffer,
        2));

    status = phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_LAST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MULTIBLOCK,
        pBlocks,
        bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE);

    /* Convert HAL status codes to palMifare status codes */
    return phalMfp_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfp_SamAV2_X_Write(
                                  phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        pBlocks,
        (bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE));
}

phStatus_t phalMfp_SamAV2_X_WriteValue(
                                       phalMfp_SamAV2_X_DataParams_t * pDataParams,
                                       uint8_t bEncrypted,
                                       uint8_t bWriteMaced,
                                       uint16_t wBlockNr,
                                       uint8_t * pValue,
                                       uint8_t bAddrData
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM pBlock[PHAL_MFP_MIFARE_BLOCK_SIZE];

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_CreateValueBlock(pValue, bAddrData, pBlock));

    return phalMfp_SamAV2_X_Write(pDataParams, bEncrypted, bWriteMaced, wBlockNr, 1, pBlock);
}

phStatus_t phalMfp_SamAV2_X_ChangeKey(
                                      phalMfp_SamAV2_X_DataParams_t * pDataParams,  
                                      uint8_t bChangeKeyMaced, 
                                      uint16_t wBlockNr,   
                                      uint16_t wKeyNumber, 
                                      uint16_t wKeyVersion,
                                      uint8_t bLenDivInput,
                                      uint8_t * pDivInput  
                                      )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    /* Evaluate command code */
    if (bChangeKeyMaced)
    {
        bCmd = PHAL_MFP_CMD_WRITE_EM;
    }
    else
    {
        bCmd = PHAL_MFP_CMD_WRITE_EN;
    }

    status = phhalHw_SamAV2_Cmd_MfpChangeKey(
        pDataParams->pHalDataParams,
        bCmd,
        wBlockNr,
        (uint8_t)wKeyNumber,
        (uint8_t)wKeyVersion,
        pDivInput,
        bLenDivInput,
        &bResponseCode);

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    /* Check Card response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, PH_ON));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_AuthenticateSL3(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bOption = PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL3_KDF;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    if (bFirstAuth)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST;
    }

    status = phhalHw_SamAV2_Cmd_MfpAuthenticate(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNumber, 
        (uint8_t)wKeyVersion,
        wBlockNr,
        pPcdCap2In, 
        bLenPcdCap2,
        pDivInput,
        bLenDivInput,
        pPcdCap2Out,
        pPdCap2,
        &bResponseCode);

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    /* Check Card response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, PH_ON));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_Read(
                                 phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    uint8_t     PH_MEMLOC_REM aCmd[4];    
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint16_t    PH_MEMLOC_REM wTotalRxLength = 0;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    /* Evaluate command code */
    if (bReadMaced)
    {
        if (bEncrypted)
        {
            if (bMacOnCmd)
            {
                aCmd[0] = PHAL_MFP_CMD_READ_EMM;
            }
            else
            {
                aCmd[0] = PHAL_MFP_CMD_READ_EMU;
            }
        }
        else
        {
            if (bMacOnCmd)
            {
                aCmd[0] = PHAL_MFP_CMD_READ_PMM;
            }
            else
            {
                aCmd[0] = PHAL_MFP_CMD_READ_PMU;
            }
        }
    }
    else
    {
        if (bEncrypted)
        {
            if (bMacOnCmd)
            {
                aCmd[0] = PHAL_MFP_CMD_READ_ENM;
            }
            else
            {
                aCmd[0] = PHAL_MFP_CMD_READ_ENU;
            }
        }
        else
        {
            if (bMacOnCmd)
            {
                aCmd[0] = PHAL_MFP_CMD_READ_PNM;
            }
            else
            {
                aCmd[0] = PHAL_MFP_CMD_READ_PNU;
            }
        }
    }

    /* Build the command frame */
    aCmd[1] = (uint8_t)(wBlockNr & 0xFF); /* LSB */
    aCmd[2] = (uint8_t)(wBlockNr >> 8);   /* MSB */
    aCmd[3] = bNumBlocks;

    /* Perform first read command */
    status = phhalHw_SamAV2_Cmd_MfpCombinedRead(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        4,
        &pResponse,
        &wRxLength,
        &bResponseCode
        );

    /* Convert HAL status codes to palMifare status codes */
    status = phalMfp_SamAV2_X_ConvertNak(status);

    /* We only allow MIFARE Plus error, CHAINING or SUCCESS */
    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        return status;
    }

    /* Check card response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, PH_ON));

    do
    {
        if ((wRxLength + wTotalRxLength) > (bNumBlocks << 4))
        {
            /* This case is possible ok, namely if SAM sends complete padding block without MAC in last frame, thus 
            we manually need to remove padding - it has to be 15 bytes */
            if (wRxLength >= sizeof(gaPaddingBlock))
            {
                if (memcmp(&pResponse[wRxLength - sizeof(gaPaddingBlock)], gaPaddingBlock, sizeof(gaPaddingBlock)) != 0)
                {
                    return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFP);
                }
                wRxLength = wRxLength - sizeof(gaPaddingBlock);
            }
            else
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFP);
            }            
        }

        memcpy(&pBlocks[wTotalRxLength], pResponse, wRxLength);  /* PRQA S 3200 */
        wTotalRxLength = wTotalRxLength + wRxLength;

        /* In case of success, we break the loop */
        if (status == PH_ERR_SUCCESS)
        {
            break;
        }

        /* Lets get the next chunk of data */
        status = phhalHw_SamAV2_Cmd_MfpCombinedRead(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_RXCHAINING,
            NULL,
            0,
            &pResponse,
            &wRxLength,
            &bResponseCode
            );

        /* Convert HAL status codes to palMifare status codes */
        status = phalMfp_SamAV2_X_ConvertNak(status);

        /* Chaining finished -> last block handling */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            memcpy(&pBlocks[wTotalRxLength], pResponse, wRxLength);  /* PRQA S 3200 */
            wTotalRxLength = wTotalRxLength + wRxLength;
        }
    }
    while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING);

    /* Check status */
    PH_CHECK_SUCCESS(status);

    /* All data has been previously copied */   
    if (wTotalRxLength != (bNumBlocks << 4))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_ReadValue(
                                      phalMfp_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bEncrypted,
                                      uint8_t bReadMaced,
                                      uint8_t bMacOnCmd,
                                      uint16_t wBlockNr,
                                      uint8_t * pValue,
                                      uint8_t * pAddrData
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM pData[PHAL_MFP_MIFARE_BLOCK_SIZE];

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_Read(pDataParams, bEncrypted, bReadMaced, bMacOnCmd, wBlockNr, 1, pData));

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_CheckValueBlockFormat(pData));

    *pAddrData = pData[12];
    memcpy(pValue, pData, 4); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_ResetAuth(
                                      phalMfp_SamAV2_X_DataParams_t * pDataParams
                                      )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* Call Reset Auth function */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ResetAuth(pDataParams->pPalMifareDataParams));

    /* Reset the crypto layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ResetSecMsgState(pDataParams));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_Increment(
                                      phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_X_Decrement(
                                      phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_X_IncrementTransfer(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wSourceBlockNr,
        wDestinationBlockNr,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_X_DecrementTransfer(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wSourceBlockNr,
        wDestinationBlockNr,
        pValue,
        PHAL_MFP_MIFARE_VALUE_BLOCK_SIZE);
}

phStatus_t phalMfp_SamAV2_X_Transfer(
                                     phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        NULL,
        0);
}

phStatus_t phalMfp_SamAV2_X_Restore(
                                    phalMfp_SamAV2_X_DataParams_t * pDataParams,
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
    return phalMfp_SamAV2_X_WriteExt(
        pDataParams,
        bCmd,
        wBlockNr,
        0x00,
        NULL,
        0);
}

phStatus_t phalMfp_SamAV2_X_ProximityCheck(
    phalMfp_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bGenerateRndC,
    uint8_t * pRndC,
    uint8_t bPps1,
    uint8_t bNumSteps,
    uint8_t * pUsedRndC
    )
{    
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    /* Satisfy Compiler */
    if (bPps1 ||pUsedRndC || bGenerateRndC || pRndC);

    /* Recalculate bNumSteps to X-config */
    switch (bNumSteps)
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
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    status = phhalHw_SamAV2_Cmd_MfpProximityCheck(
        pDataParams->pHalDataParams,
        PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK_DEFAULT,
        0xFF,
        0xFF,
        bNumSteps,
        NULL,
        0,
        &bResponseCode);

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    /* Check Card response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, PH_ON));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_ResetSecMsgState(
    phalMfp_SamAV2_X_DataParams_t * pDataParams
    )
{
    /* Kill only card Auth */
    return phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams->pHalDataParams, 0x01);
}

phStatus_t phalMfp_SamAV2_X_WriteExt(
                                     phalMfp_SamAV2_X_DataParams_t * pDataParams,
                                     uint8_t bCmdCode,
                                     uint16_t wSrcBnr,
                                     uint16_t wDstBnr,
                                     uint8_t * pData,
                                     uint8_t bDataLength
                                     )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM pCmd[5];
    uint8_t     PH_MEMLOC_REM bTxLength = 0;
    uint8_t     PH_MEMLOC_REM bResponseCode;

    pCmd[bTxLength++] = bCmdCode;
    pCmd[bTxLength++] = (uint8_t)(wSrcBnr & 0xff); /* LSB */
    pCmd[bTxLength++] = (uint8_t)(wSrcBnr >> 8);   /* MSB */

    if ((bCmdCode == PHAL_MFP_CMD_INCRTR) ||
        (bCmdCode == PHAL_MFP_CMD_INCRTR_M) |
        (bCmdCode == PHAL_MFP_CMD_DECRTR) |
        (bCmdCode == PHAL_MFP_CMD_DECRTR_M))
    {
        pCmd[bTxLength++] = (uint8_t)(wDstBnr & 0xff); /* LSB */
        pCmd[bTxLength++] = (uint8_t)(wDstBnr >> 8);   /* MSB */
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_MfpCombinedWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        pCmd,
        bTxLength,
        &bResponseCode));

    status = phhalHw_SamAV2_Cmd_MfpCombinedWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pData,
        bDataLength,
        &bResponseCode);

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_SamAV2_X_ConvertNak(status));

    /* Check Card response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(1, bResponseCode, PH_ON));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_SamAV2_X_ConvertNak(
                                       phStatus_t status
                                       )
{
    switch (status & PH_ERR_MASK)
    {
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK0:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK0, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK1:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK1, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK4:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK4, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK5:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK5, PH_COMP_PAL_MIFARE);
    case PH_ERR_SUCCESS_INCOMPLETE_BYTE:
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_CRYPTO:
        return PH_ADD_COMPCODE(PHAL_MFP_ERR_AUTH, PH_COMP_AL_MFP);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN:
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
    default:
        return status;
    }
}

#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */
