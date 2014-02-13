/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/**
* \file
* Software MIFARE DESFire Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:18 2011 $
*
* History:
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <phKeyStore.h>
#include <ph_RefDefs.h>
#include <string.h>
#include <ph_TypeDefs.h>

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
#include "../phalMfdf_Int.h"
#include "phalMfdf_SamAV2_Int.h"

phStatus_t phalMfdf_SamAV2_Int_Authenticate_General(
    phalMfdf_SamAV2_DataParams_t *pDataParams, 
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    )
{
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE+1];
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wKeyType;    
    uint16_t    PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[33];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmdBuff[33];
    uint8_t     PH_MEMLOC_REM bRndSize;
    uint8_t     PH_MEMLOC_REM bSessKeySize;
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint8_t     PH_MEMLOC_REM bAuthMode = 0x00; /* This is not the same as pDataParams->bAuthMode */
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bTmp;
    uint16_t    PH_MEMLOC_REM wHostMode;

    if ((wKeyNo > 0x7F) || (wKeyVer > 0xFF) || (bKeyNoCard > 0x0D))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) &&
        (wOption != (PH_CRYPTOSYM_DIV_MODE_DESFIRE | PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF)) &&
        (wOption != (PH_CRYPTOSYM_DIV_MODE_DESFIRE | PH_CRYPTOSYM_DIV_OPTION_2K3DES_FULL)) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen > 31))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Kill existing authentications with SAM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
        pDataParams->pHalSamDataParams,
        0x00));

    /* Getkey entry from SAM to switch the key type */
    PH_CHECK_SUCCESS_FCT(statusTmp,
        phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams->pHalSamDataParams,
        (uint8_t)wKeyNo,
        bWorkBuffer,
        &bTmp
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_DetectMode(
        pDataParams->pHalSamDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_HOSTMODE,
        &wHostMode));

    /* bWorkBuffer[keyentrylength - 2] should have the key type in bits 3 to 5 */
    if (wHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE )
    {
        bTmp = bWorkBuffer[bTmp - 3];
    }
    else
    {
        bTmp = bWorkBuffer[bTmp - 2];
    }
    bTmp &= 0x38;  /* 00011 1000 */
    bTmp >>= 3;

    switch(bTmp)
    {
    case 0x00:
        /* Equivalent to 2K3DES, DESFire keys.*/
        wKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        bRndSize = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 2*PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x06:
        /* 2K3DES keys. */
        wKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        bRndSize = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 2*PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x03:
        /* 3K3DES keys. */
        wKeyType = PH_KEYSTORE_KEY_TYPE_3K3DES;
        bRndSize = 2*PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 3*PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x04:
        /* AES128 keys. */
        wKeyType = PH_KEYSTORE_KEY_TYPE_AES128;
        bRndSize = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        bSessKeySize = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        break;

    default:
        /* Wrong key type specified. Auth. will not work */
        /* TDEA ISO 10116 (CRC 16, 4 B MAC) will also not work */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }    
    /* Build the bAuthMode parameter for SAM */
    /* Key selection by Key entry number. bit 1 stays as zero*/
    /* Bit 2 = RFU. Stays zero */
    /* Bits 5 to 7 are RFU =0 */
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        /* Bit 0. Indicates key diversification */
        bAuthMode |= 0x01;

        if ((wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES) && (wOption & PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF))
        {
            /* bit 3 is set for TDEA Key, SAM AV1 method, half key diversification */
            bAuthMode |= 0x08;
        }

        if (wOption & PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS)
        {
            /* Bit 4 is set to indicate Sam AV2 method */
            bAuthMode |= 0x10;
        }

        /* Validate diversification input length */
        if ((wOption & PH_CRYPTOSYM_DIV_MODE_MASK) == PH_CRYPTOSYM_DIV_MODE_DESFIRE) 
        {
            if (wKeyType == PH_KEYSTORE_KEY_TYPE_AES128)
            {
                if (bDivLen != 16)
                {
                    return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
                }
            }
            else
            {
                if (bDivLen != 8)
                {
                    return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
                }
            }
        }
        else
        {
            if ((wOption & PH_CRYPTOSYM_DIV_MODE_MASK) == PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS)                
            {
                if (wKeyType == PH_KEYSTORE_KEY_TYPE_AES128)
                {
                    if (bDivLen > 31)
                    {
                        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
                    }
                }
                else
                {
                    if (bDivLen > 15)
                    {
                        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
                    }
                }
            }
        }
    }

    /* Send the cmd and receive the encrypted RndB */
    bCmdBuff[wCmdLen++] = pDataParams->bAuthMode;
    bCmdBuff[wCmdLen++] = bKeyNoCard; /* key number card */

    status = phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        );
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00);
            /* Clear existing auth status in data params */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
        }
        if (status == PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        else
        {
            return status;
        }
    }
    if (wRxlen != bRndSize)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00);
            /* Clear existing auth status in data params */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
        }
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Store the encrypted RndB */
    memcpy(bRndB, bWorkBuffer, bRndSize); /* PRQA S 3200 */

    /* Perform 1st part of authentication */
    status = phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part1(
        pDataParams->pHalSamDataParams,
        bAuthMode,
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        bRndB,
        bRndSize,
        pDivInput,
        bDivLen,
        bWorkBuffer,
        &bTmp
        );

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00);
            /* Clear existing auth status in data params */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
        }
        return status;
    }

    /* Concat RndA and RndB' */
    bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    memcpy(&bCmdBuff[1], bWorkBuffer, bTmp); /* PRQA S 3200 */

    wCmdLen = bTmp + 1;

    /* Exchange on SW protocol */
    status = phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        );

    if (status != PH_ERR_SUCCESS)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00);
            /* Clear existing auth status in data params */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
        }
        return status;
    }
    if (wRxlen != bRndSize)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00);
            /* Clear existing auth status in data params */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
        }
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* bWorkBuffer now has the encrypted RndA 
    Verify the received RndA. This is the second part of authentication.*/
    status = phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part2(
        pDataParams->pHalSamDataParams,
        bWorkBuffer,
        bRndSize
        );

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00);
            /* Clear existing auth status in data params */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
        }
        return status;
    }

    /* Generation of the session key is handled by the SAM itself in this mode.*/
    pDataParams->bKeyNo = bKeyNoCard;

    return  PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_Int_Write_Enc(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wLen;
    uint16_t    PH_MEMLOC_REM wTmp;
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint8_t     PH_MEMLOC_REM bMaxDataToSam;
    uint16_t    PH_MEMLOC_REM wTxBufferSize;
    uint16_t    PH_MEMLOC_REM wOption;
    uint8_t     PH_MEMLOC_REM bMaxBlocks;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[16];
    uint16_t    PH_MEMLOC_REM wIndex = 0;
    uint8_t     PH_MEMLOC_REM * pSamBuffer;

    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES) 
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE))
    {
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }
    else
    {
        /* This function cannot be used without authentication */
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_ON
        ));
    /* Get the length of the SAM HAL Tx buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE,
        &wTxBufferSize
        ));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_OFF
        ));

    /* Assumed that wTxBufferSize can never be greater than 
    SamTxBuffer size (255 bytes), but can be configured to be lesser in dataparams struct */
    bMaxDataToSam = (uint8_t)(wTxBufferSize/bIvLen) * bIvLen;

    /* Check the max data criteria for encipher command */
    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE)
    {
        /* bMaxDataToSam <= F6 */
        if (bMaxDataToSam >= 246)
        {
            bMaxDataToSam = 232;  /* 240 - 8 bytes for cmd */
        }
        else
        {
            /* Approximating 1 block less than buffer size */
            bMaxDataToSam = ((uint8_t)(bMaxDataToSam/bIvLen) * bIvLen) - bIvLen;
        }
    }
    else if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO)
    {
        /* bMaxDataToSam <= F5 */
        if (bMaxDataToSam >= 245)
        {
            bMaxDataToSam = 232; /*  240 - 8 bytes for cmd */
        }
        else
        {
            /* Approximating 1 block less than buffer size */
            bMaxDataToSam = ((uint8_t)(bMaxDataToSam/bIvLen) * bIvLen) - bIvLen;
        }
    }
    else /* AuthenticateAES */
    {
        /* bMaxDataToSam <= EC */
        if (bMaxDataToSam >= 236)
        {
            bMaxDataToSam = 224; /* multiple of block length i.e., 16 byets */
        }
        else
        {
            /* Approximating 1 block less than buffer size */
            bMaxDataToSam = bMaxDataToSam - bIvLen;
        }
    }
    bMaxBlocks = bMaxDataToSam/bIvLen;

    memcpy(bWorkBuffer, bCmdBuff, wCmdLen); /* PRQA S 3200 */

    if (wCmdLen + wDataLen <= bMaxDataToSam) 
    {
        /* If authmode is 0x0A, CRC is needed only on the data */
        wOption = PH_EXCHANGE_DEFAULT;
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            /* Send the cmd+params first to the API for buffering */
            wOption = PH_EXCHANGE_BUFFER_FIRST;
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_EncipherData(
                pDataParams->pHalSamDataParams, 
                (PH_EXCHANGE_BUFFER_FIRST),
                bCmdBuff, /* Send only the cmd+params first to this API */
                (uint8_t)wCmdLen,
                (uint8_t)wCmdLen,  /* Offset from where enc. should start. */
                &pSamBuffer,
                &wRxlen
                ));

            wOption = PH_EXCHANGE_BUFFER_LAST;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_EncipherData(
            pDataParams->pHalSamDataParams, 
            wOption,
            pData,
            (uint8_t)wDataLen,
            0x00,
            &pSamBuffer,
            &wRxlen
            ));

        /* Send the data to PICC */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_SamAV2_Int_SendDataToPICC(
            pDataParams,
            bWorkBuffer,
            wCmdLen,
            pSamBuffer,
            wRxlen,
            bWorkBuffer,
            &wLen
            ));
    }
    else
    {
        /* temporarily assign wTmp with data length */
        wTmp = wDataLen;

        /* Round the cmd length to the next block number */
        if(wCmdLen % bIvLen)
        {
            bTmp = (uint8_t)wCmdLen/bIvLen + 1;
        }
        else 
        {
            bTmp = (uint8_t)wCmdLen/bIvLen;
        }

        /* Break the data into chunks that fit into the maxdatatosam size.
        The chunk should be a multiple of the iv size

        Set TXCHAINING and send data to SAM for encryption.
        SAM returns only the data part in encrypted form and not the
        cmd + params that was sent earlier. These are used to calculate
        the CRC but are not returned.        

        If authmode is 0x0A, CRC is needed only on the data */

        wOption = PH_EXCHANGE_DEFAULT;
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            /* Send the cmd+params first to the API for buffering */
            wOption = PH_EXCHANGE_BUFFER_FIRST | PH_EXCHANGE_TXCHAINING ;

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_EncipherData(
                pDataParams->pHalSamDataParams, 
                wOption,
                bCmdBuff, /* Send only the cmd+params first to this API */
                (uint8_t)wCmdLen,
                (uint8_t)wCmdLen,  /* Offset from where enc. should start. */
                &pSamBuffer,
                &wRxlen
                ));

            wOption = PH_EXCHANGE_BUFFER_LAST;
        }
        else
        {
            wOption = PH_EXCHANGE_DEFAULT | PH_EXCHANGE_TXCHAINING;
        }

        /* Ask SAM to encipher the data */
        status = phhalHw_SamAV2_Cmd_SAM_EncipherData(
            pDataParams->pHalSamDataParams, 
            wOption,
            pData,
            (bMaxBlocks - bTmp) * bIvLen, /* data length equal to multiple of Iv len */
            0x00, /* Offset from where encryption will start */
            &pSamBuffer,
            &wRxlen
            );

        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
            {
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00); 
            }
            return status;
        }

        /* Send the encrypted data to PICC */
        status = phalMfdf_SamAV2_Int_SendDataToPICC(
            pDataParams,
            bWorkBuffer,
            wCmdLen,
            pSamBuffer,
            wRxlen,
            bWorkBuffer,
            &wLen
            );

        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            return status; /* Component code already added by phalMfdf_SamAV2_Int_SendDataToPICC */
        }

        /* Update wTmp with remaining data to be encrypted */
        wTmp -= (bMaxBlocks - bTmp) * bIvLen;

        /* Update the next index in the data */
        wIndex += (bMaxBlocks - bTmp) * bIvLen;

        while (wTmp)
        {
            bWorkBuffer[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
            wCmdLen = 1;
            if(wTmp > bMaxDataToSam)
            {
                /* Ask SAM to encipher data. Indicate that more data is to come */
                status = phhalHw_SamAV2_Cmd_SAM_EncipherData(
                    pDataParams->pHalSamDataParams, 
                    (PH_EXCHANGE_TXCHAINING),
                    &pData[wIndex],
                    bMaxBlocks * bIvLen, /* Data size equal to multiple of bIvLen */
                    0x00, /* Offset of encryption starts at zero here */
                    &pSamBuffer,
                    &wRxlen
                    );

                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) 
                {
                    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                    {
                        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                            pDataParams->pHalSamDataParams,
                            0x00); 
                    }
                    return status;
                }

                status = phalMfdf_SamAV2_Int_SendDataToPICC(
                    pDataParams,
                    bWorkBuffer, /* Cmd only. */
                    wCmdLen,
                    pSamBuffer, /* Data only */
                    wRxlen,
                    bWorkBuffer,
                    &wLen
                    );

                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
                {
                    return statusTmp; /* Component code already added by phalMfdf_SamAV2_Int_SendDataToPICC */
                }

                /* Update the remaining data size */
                wTmp -= bMaxBlocks * bIvLen;

                /* Calculate the next data index. */
                wIndex += (bMaxBlocks * bIvLen);
            }
            else
            {
                status = phhalHw_SamAV2_Cmd_SAM_EncipherData(
                    pDataParams->pHalSamDataParams, 
                    (PH_EXCHANGE_DEFAULT), /* Last chunk of data */
                    &pData[wIndex],
                    (uint8_t)wTmp,
                    0x00,
                    &pSamBuffer,
                    &wRxlen
                    );
                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                    {
                        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                            pDataParams->pHalSamDataParams,
                            0x00); 
                    }
                    return status;
                }

                status = phalMfdf_SamAV2_Int_SendDataToPICC(
                    pDataParams,
                    bWorkBuffer,
                    wCmdLen,
                    pSamBuffer,
                    wRxlen, /* wRxlen will also include CRC + padding (if any) */
                    bWorkBuffer,
                    &wLen
                    );
                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    return status; /* Component code already added by phalMfdf_SamAV2_Int_SendDataToPICC */
                }

                wIndex = wIndex + wTmp;

                /* Remaining data bytes = zero */
                wTmp = 0;
            }                
        }
    }

    /* At this point data is already sent to PICC
    * The status and MAC(if any) returned from the 
    * card is now in bWorkBuffer. 
    */  

    /* Verify the MAC. MAC is not received if in 0x0A MAC'd mode */
    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) || 
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
    {
        if(wLen < 8 )
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        /* copy CMAC received from card*/
        memcpy(bCMacCard, &bWorkBuffer[wLen-8], 8);  /* PRQA S 3200 */
        wLen -= 8;            

        /* Copy the status byte at the end */
        bWorkBuffer[wLen] = (uint8_t)status;

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            0x00,
            bWorkBuffer,
            0x01
            ));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            0x00,
            bCMacCard,
            0x08
            ));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}


phStatus_t phalMfdf_SamAV2_Int_Write_Plain(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t bCommOption,
    uint8_t * pData,
    uint16_t wDataLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint16_t    PH_MEMLOC_REM wTotalLen = 0;
    uint16_t    PH_MEMLOC_REM wFrameLen = 0;
    uint16_t    PH_MEMLOC_REM wSamTxBufferSize;
    uint8_t     PH_MEMLOC_REM bMaxDataToSam;
    uint8_t     PH_MEMLOC_REM bMaxBlocks;
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t     PH_MEMLOC_REM * pRecv = NULL;
    uint8_t     PH_MEMLOC_REM bCMac[8];
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint16_t    PH_MEMLOC_REM wMacLen = 0;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
    uint16_t    PH_MEMLOC_REM wTmp = 0;
    uint16_t    PH_MEMLOC_REM wIndex = 0;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };

    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES) 
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_ON
        ));
    /* Get the length of the SAM HAL Tx buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE,
        &wSamTxBufferSize
        ));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_OFF
        ));

    /* Assumed that wRxBufferSize can never be greater than SamRxBuffer size (256 bytes) */
    bMaxDataToSam = (uint8_t)(wSamTxBufferSize / bIvLen) * bIvLen;
    bMaxBlocks = bMaxDataToSam / bIvLen;

    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) || 
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES) ||
        ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE) && 
        (bCommOption == PHAL_MFDF_COMMUNICATION_MACD)))
    {
        if(wCmdLen + wDataLen <= bMaxDataToSam)
        {
            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
            {
                /* Calculate CMAC over the cmd+params first. Only if notin 0x0A mode */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
                    pDataParams->pHalSamDataParams, 
                    (PH_EXCHANGE_BUFFER_FIRST),
                    0x00 /* bIvLen */,
                    bCmdBuff,
                    (uint8_t)wCmdLen,
                    &pRecv,
                    &wMacLen
                    ));
            }

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
                pDataParams->pHalSamDataParams, 
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE)?PH_EXCHANGE_DEFAULT:(PH_EXCHANGE_BUFFER_LAST),
                0x00,
                pData,
                (uint8_t)wDataLen,
                &pRecv,
                &wMacLen
                ));
        }
        else
        {
            /* Data size bigger than 0xFF */
            wTmp = wDataLen;

            /* Round the cmd length to the next block number */
            if(wCmdLen % bIvLen)
            {
                bTmp = (uint8_t)wCmdLen / bIvLen + 1;
            }
            else 
            {
                bTmp = (uint8_t)wCmdLen / bIvLen;
            }

            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
            {
                /*Prepare cmd+params */
                status = phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
                    pDataParams->pHalSamDataParams, 
                    (PH_EXCHANGE_TXCHAINING),
                    0x00,
                    bCmdBuff,
                    (uint8_t)wCmdLen,
                    &pRecv,
                    &wMacLen
                    );

                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
                {
                    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                    {
                        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                            pDataParams->pHalSamDataParams,
                            0x00);
                        /* Clear existing auth status in data params */
                        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
                    }                    
                    return status;
                }
            }

            /* Index in data */
            wIndex = 0;
            while(wTmp)
            {
                if (wTmp > bMaxDataToSam)
                {
                    status = phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
                        pDataParams->pHalSamDataParams, 
                        PH_EXCHANGE_TXCHAINING,
                        0x00,
                        &pData[wIndex],
                        bMaxBlocks * bIvLen,
                        &pRecv,
                        &wMacLen
                        );
                    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) 
                    {
                        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                        {
                            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                pDataParams->pHalSamDataParams,
                                0x00);
                            /* Clear existing auth status in data params */
                            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
                        }   
                        return status;
                    }
                    /* Update the remaining data size */
                    wTmp -= bMaxBlocks * bIvLen;

                    /* Calculate the next data index. */
                    wIndex += (bMaxBlocks * bIvLen);
                }
                else
                {
                    status = phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
                        pDataParams->pHalSamDataParams, 
                        PH_EXCHANGE_DEFAULT,
                        0x00,
                        &pData[wIndex],
                        (uint8_t)wTmp,
                        &pRecv,
                        &wMacLen
                        );

                    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                    {
                        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                        {
                            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                pDataParams->pHalSamDataParams,
                                0x00);
                            /* Clear existing auth status in data params */
                            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
                        }   
                        return status;
                    }
                    wIndex = wIndex + wTmp;
                    wTmp = 0;
                }                
            }
        }

        /* Store the CMAC. To be sent with data in MAC'd mode */
        if (bCommOption == PHAL_MFDF_COMMUNICATION_MACD)
        {
            memcpy(bCMac, pRecv, wMacLen); /* PRQA S 3200 */
        }

        /* If communication mode is set to plain, then MAC is only calculated to update
        the init vector but is not sent with the data */
        if (bCommOption == PHAL_MFDF_COMMUNICATION_PLAIN)
        {
            wMacLen = 0;
        }
        else
        {
            if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE)
            {
                wMacLen = 4;
            }
            else
            {
                wMacLen = 8;
            }
        }
    }
    wTotalLen = wDataLen + wMacLen;
    if (pDataParams->bWrappedMode)
    {
        wFrameLen = PHAL_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PHAL_MFDF_MAXDFAPDU_SIZE;
    }
    wIndex = 0;

    /* if wCmdLen + wDataLen + wMacLen <= framelength */
    if (wTotalLen == 0x0000)
    {
        /* Single frame cmd without any data. Just send it */
        status = phalMfdf_ExchangeCmd(
            pDataParams,
            pDataParams->pPalMifareDataParams,
            pDataParams->bWrappedMode,
            bCmdBuff,
            wCmdLen,
            bWorkBuffer,
            &wRxlen
            );
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00);
                /* Clear existing auth status in data params */
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);  
            }
            return PH_ADD_COMPCODE(status, PH_COMP_AL_MFDF);
        }
    }
    else
    {
        wIndex = 0;
        wTmp = wTotalLen;

        if (wTmp <= (wFrameLen - wCmdLen))
        {
            if (pDataParams->bWrappedMode)
            {
                pApdu[1] = bCmdBuff[0]; 
                pApdu[4] = (uint8_t)(wCmdLen + wTotalLen) - 0x01u;

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
                    pApdu,
                    PHAL_MFDF_WRAP_HDR_LEN,
                    &pRecv,
                    &wRxlen));

                if (pApdu[4] != 0x00)
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                        pDataParams->pPalMifareDataParams,
                        PH_EXCHANGE_BUFFER_CONT,
                        &bCmdBuff[1],
                        wCmdLen - 1,
                        &pRecv,
                        &wRxlen));

                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                        pDataParams->pPalMifareDataParams,
                        PH_EXCHANGE_BUFFER_CONT,
                        &pData[wIndex],
                        wDataLen,
                        &pRecv,
                        &wRxlen));
                    if (wMacLen != 0x0000)
                    {
                        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                            pDataParams->pPalMifareDataParams,
                            PH_EXCHANGE_BUFFER_CONT,
                            bCMac,
                            wMacLen,
                            &pRecv,
                            &wRxlen));
                    }
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                        pDataParams->pPalMifareDataParams,
                        PH_EXCHANGE_BUFFER_LAST,
                        &pApdu[2],
                        0x01,
                        &pRecv,
                        &wRxlen));
                }
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_FIRST,
                    bCmdBuff,
                    wCmdLen,
                    &pRecv,
                    &wRxlen));

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    (wMacLen == 0x00) ? PH_EXCHANGE_BUFFER_LAST : PH_EXCHANGE_BUFFER_CONT,
                    &pData[wIndex],
                    wDataLen,
                    &pRecv,
                    &wRxlen));

                if (wMacLen != 0x0000)
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                        pDataParams->pPalMifareDataParams,
                        PH_EXCHANGE_BUFFER_LAST,
                        bCMac,
                        wMacLen,
                        &pRecv,
                        &wRxlen));
                }
            }
            if (pDataParams->bWrappedMode)
            {
                status = pRecv[wRxlen - 1];
                wRxlen -= 2;
            } 
            else 
            {
                status = pRecv[0];
                pRecv++; /* Increment pointer to point only to data */
                wRxlen -= 1;
            }
            if (status != PH_ERR_SUCCESS)
            {
                /* Reset authentication status */
                if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) || 
                    (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
                {
                    statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00);
                    /* Clear existing auth status in data params */
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
                }
                return phalMfdf_Int_ComputeErrorResponse(pDataParams, (uint8_t)status);
            }

            memcpy(bWorkBuffer, pRecv, wRxlen); /* PRQA S 3200 */
        }
        else
        {
            status = phalMfdf_SamAV2_Int_SendDataToPICC(
                pDataParams,
                bCmdBuff,
                wCmdLen,
                pData,
                wDataLen,
                bWorkBuffer,
                &wRxlen
                );

            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
            {
                /* Send MAC in the last frame */
                if ((wMacLen > 0) && (pRecv != NULL))
                {
                    wCmdLen = 1;
                    bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

                    status = phalMfdf_SamAV2_Int_SendDataToPICC(
                        pDataParams,
                        bCmdBuff,
                        wCmdLen,
                        bCMac,
                        wMacLen,
                        bWorkBuffer,
                        &wRxlen
                        );

                    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                    {
                        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams); 
                        return status; /* Component code already added by phalMfdf_SamAV2_Int_SendDataToPICC */
                    }
                }
                else
                {
                    return status;
                }
            }
            else
            {
                if (status != PH_ERR_SUCCESS)
                {
                    return status;
                }
            }
        }
    }
    /* Verify the MAC. MAC is not received if in 0x0A MAC'd mode */
    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) || 
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
    {
        if(wRxlen < 8 )
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        /* copy CMAC received from card*/
        memcpy(bCMacCard, &bWorkBuffer[wRxlen - 8], 8);  /* PRQA S 3200 */
        wRxlen -= 8;            

        /* Copy the status byte at the end */
        bWorkBuffer[wRxlen] = (uint8_t)status;

        /* Verify the MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            0x00,
            bWorkBuffer,
            0x01
            ));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            0x00,
            bCMacCard,
            0x08
            ));
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

void phalMfdf_SamAV2_Int_ResetAuthStatus(phalMfdf_SamAV2_DataParams_t * pDataParams)
{
    pDataParams->bKeyNo = 0xFF;
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED;
}

phStatus_t phalMfdf_SamAV2_Int_SendDataToPICC(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pCmd,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen,
    uint8_t * pResp,
    uint16_t * pRespLen
    )
{
    /* Utility function to send encrypted data to PICC as and when it is available from SAM */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wTmp;
    uint16_t    PH_MEMLOC_REM wLen;
    uint16_t    PH_MEMLOC_REM wFrameLen;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };
    uint8_t     PH_MEMLOC_REM * pRecv;

    if (pDataParams->bWrappedMode)
    {
        wFrameLen = PHAL_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PHAL_MFDF_MAXDFAPDU_SIZE;
    }

    /* Send the data to PICC */
    wIndex = 0;
    wTmp = wDataLen;

    do
    {
        wLen = (wTmp < (wFrameLen - wCmdLen))? wTmp : (wFrameLen - wCmdLen);

        if (pDataParams->bWrappedMode)
        {
            pApdu[1] = pCmd[0]; /* Desfire cmd code in INS */
            pApdu[4] = (uint8_t)(wCmdLen + wLen) - 0x01u;

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
                pApdu,
                PHAL_MFDF_WRAP_HDR_LEN,
                &pRecv,
                pRespLen));

            if (pApdu[4] != 0x00)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    &pCmd[1],
                    wCmdLen - 1,
                    &pRecv,
                    pRespLen));

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    &pData[wIndex],
                    wLen,
                    &pRecv,
                    pRespLen));

                /* Le byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    &pApdu[2],
                    0x01,
                    &pRecv,
                    pRespLen));
            }
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST,
                pCmd,
                wCmdLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                &pData[wIndex],
                wLen,
                &pRecv,
                pRespLen));
        }
        wIndex = wIndex + wLen;
        wTmp = wTmp - wLen;

        if (pDataParams->bWrappedMode)
        {
            memcpy(pResp, pRecv, (*pRespLen) - 2); /* PRQA S 3200 */
            bStatusByte = pRecv[(*pRespLen) - 1];
            (*pRespLen) -= 2;
        } 
        else 
        {
            memcpy(pResp, &pRecv[1], (*pRespLen) - 1); /* PRQA S 3200 */
            bStatusByte = pRecv[0];
            (*pRespLen) -= 1;
        }

        if ((bStatusByte != PHAL_MFDF_RESP_ADDITIONAL_FRAME) &&
            (bStatusByte != PH_ERR_SUCCESS))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) || 
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00);
            }
            return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
        }

        /* Success returned even before writing all data? protocol error */
        if ((bStatusByte == PH_ERR_SUCCESS) && (wTmp != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) || 
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00); 
            }
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF); 
        }

        if(bStatusByte != 0x00)
        {
            pCmd[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
            wCmdLen = 1;
        }
    }
    while(wTmp);

    return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfdf_SamAV2_Int_GetData(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint8_t * pSendBuff,
                                       uint16_t wCmdLen,
                                       uint8_t ** pResponse,
                                       uint16_t * pRxlen
                                       )
{
    /* GetData gets data from the PICC and chains the 
    data on the Reader HAL Rx buffer. 
    It moves the start position of this Rx buffer 
    after the first reception so as to store the 
    next frame of data in the next position */

    uint16_t    PH_MEMLOC_REM wOption;
    uint8_t     PH_MEMLOC_REM * pRecv;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bStatusByte = 0xFF;
    uint8_t     PH_MEMLOC_REM bCmdBuff[10];
    uint16_t    PH_MEMLOC_REM wNextPos = 0;
    uint16_t    PH_MEMLOC_REM wRxBufferSize = 0;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint8_t     PH_MEMLOC_REM bBackupBytes[3];
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };
    uint16_t    PH_MEMLOC_REM wBackUpLen = 0;
    uint8_t     PH_MEMLOC_REM bBackUpByte;
    uint8_t     PH_MEMLOC_REM bBackUpByte1;

    /* Status and two other bytes to be backed up before getting new frame of data */
    memset(bBackupBytes, 0x00, 3); /* PRQA S 3200 */

    wOption = PH_EXCHANGE_DEFAULT;
    if (pDataParams->bWrappedMode)
    {
        if (wCmdLen > PHAL_MFDF_MAXWRAPPEDAPDU_SIZE)
        {
            return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFDF);
        }

        pApdu[1] = pSendBuff[0];  /* Desfire command code. */
        /* Encode APDU Length*/
        pApdu[4]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */

        statusTmp = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PHAL_MFDF_WRAP_HDR_LEN,
            &pRecv,
            pRxlen
            );

        if ((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1,
                &pRecv,
                pRxlen
                ));

            statusTmp = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2],
                0x01,
                &pRecv,
                pRxlen
                );            
        }
        /* To handle the case where the card returns only status 91 and returns
        AF in the next frame */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            /* One more status byte to read from DesFire */
            bBackUpByte = pRecv[0];
            bBackUpByte1 = pRecv[1];
            wBackUpLen = *pRxlen;

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_RXCHAINING,
                &pApdu[2],
                0x01,
                &pRecv,
                pRxlen
                ));

            /* Received length can be one or two Ex: 0x91 0xAF */
            if (*pRxlen == 2)
            {
                pRecv[wBackUpLen] = pRecv[0];
                pRecv[wBackUpLen + 1] = pRecv[1];
                bStatusByte = pRecv[1];
            }
            else if (*pRxlen == 1)
            {
                bStatusByte = pRecv[0];
                pRecv[wBackUpLen] = bStatusByte;
            }
            else
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
            }

            *pRxlen = wBackUpLen + *pRxlen;

            /* Set back the backed up bytes */
            pRecv[0] = bBackUpByte;
            pRecv[1] = bBackUpByte1;
        }
        else
        {
            if (statusTmp != PH_ERR_SUCCESS)
            {
                return statusTmp;
            }
        }
    }
    else
    {
        /* Normal mode */
        if (wCmdLen > PHAL_MFDF_MAXDFAPDU_SIZE)
        {
            return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFDF);
        }

        /* Send this on L4 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            wOption,
            pSendBuff,
            wCmdLen,
            &pRecv,
            pRxlen
            ));
    }

    /* Storing the original pointer */
    *pResponse = pRecv;

    /* Status is 0xAF or Ox00? */
    if (*pRxlen > 0x0000)
    {
        if (pDataParams->bWrappedMode)
        {
            bStatusByte = (*pResponse)[(*pRxlen) - 1];
        } 
        else 
        {
            bStatusByte = (*pResponse)[0];
        }
    }

    if (bStatusByte == PHAL_MFDF_RESP_ADDITIONAL_FRAME)
    {
        if (((pDataParams->bWrappedMode) && (*pRxlen == 2)) ||
            ((!(pDataParams->bWrappedMode)) && (*pRxlen == 1)))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if (pDataParams->bWrappedMode)
        {
            /* Next position will ensure overwriting on the 
            SW1SW2 received from previous command */
            wNextPos = (*pRxlen) - 2;

            /* backing up three bytes. */
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */
        } 
        else
        {
            /* Backup the last byte */
            memcpy(bBackupBytes, &(*pResponse)[(*pRxlen - 3)], 3); /* PRQA S 3200 */
            wNextPos = (*pRxlen) - 1;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            ));

        if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
        {
            bIvLen = 16;
        }
        else if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
            (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE))
        {
            bIvLen = 8;
        }
        else
        {
            bIvLen = 0;
        }
    }
    while (bStatusByte == PHAL_MFDF_RESP_ADDITIONAL_FRAME)
    {
        if (((pDataParams->bWrappedMode) && (*pRxlen == 2)) ||
            ((!(pDataParams->bWrappedMode)) && (*pRxlen == 1)))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        if (wNextPos + 60 > wRxBufferSize)
        {
            /* Return 0xAF and let the caller recall the function with
            option = PH_EXCHANGE_RXCHAINING 
            Return the data accumulated till now and its length */
            if (pDataParams->bWrappedMode)
            {
                (*pRxlen) -= 2;
            } 
            else
            {
                (*pRxlen) -= 1;
                (*pResponse)++;
            }
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            ));

        bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
        wCmdLen = 1;
        wOption = PH_EXCHANGE_DEFAULT;
        if (pDataParams->bWrappedMode)
        {
            pApdu[1] = bCmdBuff[0];  /* Desfire command code. */
            /* Encode APDU Length*/
            pApdu[4]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */

            statusTmp = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
                pApdu,
                PHAL_MFDF_WRAP_HDR_LEN,
                &pRecv,
                pRxlen
                );

            if ((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    bCmdBuff,
                    wCmdLen,
                    &pRecv,
                    pRxlen
                    ));

                bCmdBuff[0] = 0x00; /* Le */

                statusTmp = phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    &pApdu[2],
                    0x01,
                    &pRecv,
                    pRxlen
                    );                
            }
            /* To handle the case where the card returns only status 91 and returns
            AF in the next frame */
            if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
            {
                /* One more status byte to read from DesFire */
                bBackUpByte = pRecv[0];
                bBackUpByte1 = pRecv[1];
                wBackUpLen = *pRxlen;

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_RXCHAINING,
                    &pApdu[2],
                    0x01,
                    &pRecv,
                    pRxlen
                    ));

                /* Received length can be one or two Ex: 0x91 0xAF */
                if (*pRxlen == 2)
                {
                    pRecv[wBackUpLen] = pRecv[0];
                    pRecv[wBackUpLen + 1] = pRecv[1];
                    bStatusByte = pRecv[1];
                }
                else if (*pRxlen == 1)
                {
                    bStatusByte = pRecv[0];
                    pRecv[wBackUpLen] = bStatusByte;
                }
                else
                {
                    return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
                }

                *pRxlen = wBackUpLen + *pRxlen;

                /* Set back the backed up bytes */
                pRecv[0] = bBackUpByte;
                pRecv[1] = bBackUpByte1;
            }
            else
            {
                if (statusTmp != PH_ERR_SUCCESS)
                {
                    return statusTmp;
                }
            }
        }
        else
        {
            /* Send this on L4 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                wOption,
                bCmdBuff,
                wCmdLen,
                &pRecv,
                pRxlen
                ));
        }

        /* Update wNextPos */
        if (pDataParams->bWrappedMode)
        {
            bStatusByte = (*pResponse)[(*pRxlen) - 1];

            /* Putback the backed up bytes */
            memcpy(&(*pResponse)[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */

            wNextPos = (*pRxlen) - 2;
            /* backup the last three bytes of current frame */
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */        
        } 
        else
        {
            bStatusByte = (*pResponse)[wNextPos];

            /* Putback the backed up bytes */
            memcpy(&(*pResponse)[wNextPos - 2], bBackupBytes, 3); /* PRQA S 3200 */

            wNextPos = (*pRxlen) - 1;

            /* Backup 3 bytes. The nxt frame will overwrite these */
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 2], 3); /* PRQA S 3200 */
        }
    }
    if (pDataParams->bWrappedMode)
    {
        (*pRxlen) -= 2;
    } 
    else
    {
        (*pRxlen) -= 1;
        (*pResponse)++;
    }
    return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfdf_SamAV2_Int_ReadData_Plain(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    )
{
    uint16_t    PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM statusTmp1 = 0;
    uint16_t    PH_MEMLOC_REM wTmp = 0;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM * pCMAC;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint16_t    PH_MEMLOC_REM wSamTxBufferSize;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wOption;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint8_t     PH_MEMLOC_REM bLen;
    uint8_t     PH_MEMLOC_REM bNumBlocks;
    uint8_t     PH_MEMLOC_REM * pRecv;

    memset(bWorkBuffer, 0x00, 32); /* PRQA S 3200 */
    memset(bCMacCard, 0x00, 8); /* PRQA S 3200 */

    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }

    if (((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)) &&
        (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))        
    {
        /* Check for 0xAF added above to ensure that we dont update the
        IV or calculate CMAC for cases where in the application has called
        this API with bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME */

        /* Calculate MAC to update the init vector */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            0x00, /* bIvLen, */
            bCmdBuff,
            (uint8_t)wCmdLen,
            &pCMAC,
            &wRxlen
            ));
    }

    /* Send the command */
    status = phalMfdf_SamAV2_Int_GetData(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        );

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) && 
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        /* Reset authentication status */
        if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
            (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
        {
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        }
        /* Component code is already added by GetData */
        return status;
    }
    if (((status & PH_ERR_MASK) == PHAL_MFDF_RESP_ADDITIONAL_FRAME) &&
        (((pDataParams->bWrappedMode) && (wRxlen == 1)) ||
        ((!(pDataParams->bWrappedMode)) && (wRxlen == 0))))
    {
        /* AF should always be accompanied by data. Otherwise
        it is a protocol error */
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* If plain data, dont verify MAC */
    if (((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE) 
        && ((bOption & 0xF0U) != PHAL_MFDF_COMMUNICATION_MACD))
        || (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED))
    {
        *ppRxdata = pRecv;
        *pRxdataLen = wRxlen;

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
    }

    /* Verify the MAC
    if (((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
    (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)) ||
    ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE) 
    && ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD)))
    */
    /* Size of MAC bytes returned */
    (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE) ? (wTmp = 4) :( wTmp = 8);

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_ON
        ));
    /* Get the length of the SAM HAL Tx buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE,
        &wSamTxBufferSize
        ));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_OFF
        ));

    if (wRxlen > wSamTxBufferSize)
    {
        /* Index of data on Reader Rx Buffer */
        wIndex = 0;
        wOption = PH_EXCHANGE_TXCHAINING;
        bNumBlocks = (uint8_t)(wSamTxBufferSize)/bIvLen;
        bLen = bNumBlocks * bIvLen;

        do
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                wOption,
                0x00,
                &pRecv[wIndex],
                bLen
                );

            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00); 
                }
                return statusTmp;
            }

            /* Increment wIndex with the amount of data received from SAM */
            wIndex = wIndex + bLen;

            if ((wRxlen - wIndex) > wSamTxBufferSize)
            {
                bNumBlocks = (uint8_t)wSamTxBufferSize / bIvLen;
                bLen = bNumBlocks * bIvLen;
            }
            else
            {
                bLen = (uint8_t)(wRxlen - wIndex);
                if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE)
                    {
                        statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                            pDataParams->pHalSamDataParams,
                            PH_EXCHANGE_DEFAULT,
                            0x00,
                            &pRecv[wIndex],
                            bLen
                            );            
                        if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
                        {
                            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                            {
                                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                                statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                    pDataParams->pHalSamDataParams,
                                    0x00); 
                            }
                            return statusTmp;
                        }
                        wRxlen = wRxlen - wTmp;
                    }
                    else
                    {
                        memcpy(bCMacCard, &pRecv[wRxlen - wTmp], wTmp); /* PRQA S 3200 */
                        bLen = bLen - (uint8_t)wTmp;

                        statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                            pDataParams->pHalSamDataParams,
                            PH_EXCHANGE_TXCHAINING,
                            0x00,
                            &pRecv[wIndex],
                            bLen
                            );

                        if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
                        {
                            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                            {
                                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                                statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                    pDataParams->pHalSamDataParams,
                                    0x00); 
                            }
                            return statusTmp;
                        }
                        /* Next buffer the status */
                        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                            pDataParams->pHalSamDataParams,
                            PH_EXCHANGE_BUFFER_FIRST,
                            0x00,
                            (uint8_t *)&status,
                            0x01
                            ));

                        /* Lastly send it along with the MAC */
                        statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                            pDataParams->pHalSamDataParams,
                            PH_EXCHANGE_BUFFER_LAST,
                            0x00,
                            bCMacCard,
                            (uint8_t)wTmp
                            );

                        if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
                        {
                            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                            {
                                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                                statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                    pDataParams->pHalSamDataParams,
                                    0x00); 
                            }
                            return statusTmp;
                        }
                        /* Return the data minus the cmac length */
                        wRxlen = wRxlen - wTmp;
                    }
                    /* Set bLen to zero. No more looping */
                    bLen = 0;
                }
            }
        }
        while (bLen);
    }
    else
    {
        /* If wRxlen <= wSamRxBufferSize, it may still be the case
        * that more data is expected from the card. 
        */

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE)
            {
                statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                    pDataParams->pHalSamDataParams,
                    PH_EXCHANGE_DEFAULT,
                    0x00,
                    pRecv,
                    (uint8_t)wRxlen
                    );
                if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                    {
                        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                        statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                            pDataParams->pHalSamDataParams,
                            0x00); 
                    }
                    return statusTmp;
                }
                wRxlen = wRxlen - wTmp;
            }
            else
            {
                memcpy(bCMacCard, &pRecv[wRxlen - wTmp], wTmp); /* PRQA S 3200 */
                wRxlen = wRxlen - wTmp;
                if (wRxlen != 0)
                {
                    statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                        pDataParams->pHalSamDataParams,
                        PH_EXCHANGE_TXCHAINING,
                        0x00,
                        pRecv,
                        (uint8_t)wRxlen
                        );

                    if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
                    {
                        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                        {
                            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                            statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                pDataParams->pHalSamDataParams,
                                0x00); 
                        }
                        return statusTmp;
                    }
                }
                /*  Next goes the status byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                    pDataParams->pHalSamDataParams,
                    PH_EXCHANGE_BUFFER_FIRST,
                    0x00,
                    (uint8_t *)&status,
                    0x01
                    ));

                /* Last frame of data. MAC goes here */
                statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                    pDataParams->pHalSamDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    0x00,
                    bCMacCard,
                    (uint8_t)wTmp
                    );
                if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                    {
                        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                        statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                            pDataParams->pHalSamDataParams,
                            0x00); 
                    }
                    return statusTmp;
                }
            }
        }
        else
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                PH_EXCHANGE_TXCHAINING,
                0x00,
                pRecv,
                (uint8_t)wRxlen
                );

            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00); 
                }
                return statusTmp;
            }
        }

    }
    *ppRxdata = pRecv;
    *pRxdataLen = wRxlen;

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_Int_ReadData_Enc(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t * pLength,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    )
{
    /* 
    pLength parameter indicates the length of data to be read 
    or else will be 0x000000 which means unknown length of data 
    to be read
    */
    uint16_t    PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM statusTmp1;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wLen; /* Temporary length variable */
    uint8_t     PH_MEMLOC_REM * pCMAC;
    uint8_t     PH_MEMLOC_REM bNumBlocks = 0;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint8_t     PH_MEMLOC_REM * pSamBuffer;
    uint16_t    PH_MEMLOC_REM wSamDataLen;
    uint16_t    PH_MEMLOC_REM wSamTxBufferSize;
    uint16_t    PH_MEMLOC_REM wIndex = 0;
    uint16_t    PH_MEMLOC_REM wRxBufferIndex = 0;
    uint16_t    PH_MEMLOC_REM wOption;

    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF); 
    }
    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }

    if (((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)) &&
        (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
    {
        /* Calculate MAC to update the init vector */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            0x00 /* bIvLen */,
            bCmdBuff,
            (uint8_t)wCmdLen,
            &pCMAC,
            &wRxlen
            ));
    }

    /* Send the command */
    status = phalMfdf_SamAV2_Int_GetData(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        );

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) && 
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        /* Reset authentication status */
        if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
            (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
        {
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        }
        return status;
    }

    /* THE DATA IS NOW ON THE RXBUFFER OF THE READER HAL. 
    - This may be the only data.
    - there may be more data.

    * Decrypt the received data 
    * The RxBuffer of the reader may be bigger than the Sam's Tx Buffer.
    * In this case, data upto the size of Sam HAL Tx Buffer should be written
    */

    /* If the length of encrypted data received from the card
    * is larger than the length of the SAM HAL Tx buffer, then
    * we need to break this data into chunks that fit into 
    * SAM Tx buffer and get it decrypted 
    * if length of data to be read is known, padding method (bOption) is one. 
    * else it is option 2.
    * if option 1, wOption to the decipherdata API should be 
    * wOption = PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH
    * pLenEncInputData[3] parameter should be filled
    * else wOption = PH_EXCHANGE_TXCHAINING
    */

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_ON
        ));
    /* Get the length of the SAM HAL Tx buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE,
        &wSamTxBufferSize
        ));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_OFF
        ));

    /* To ensure that we only send permissible 
    data length to SAM 
    wSamTxBufferSize = wSamTxBufferSize - bIvLen;
    */

    if (wRxlen > wSamTxBufferSize)
    {
        /* Index of data on Reader Rx Buffer */
        wIndex = 0;
        wRxBufferIndex = 0;

        /* 
        If length of data to be transmitted is known, then 
        wOption should be or'd with PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH.
        wOption |= PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH;

        In addition, the three byte length information should be sent in the first
        frame to SAM */

        /* How many data blocks can the SAM HAL rx buffer hold? */
        if (((bOption & 0x0FU) == PH_CRYPTOSYM_PADDING_MODE_1) &&
            (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
        { 
            /* The length of data to be read is known. First three bytes of data
            sent to the SAM should have this information */
            bNumBlocks = (uint8_t)(wSamTxBufferSize - 3) / bIvLen;
            wOption = PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH;
        }
        else if ((bOption & 0x0FU) == PH_CRYPTOSYM_PADDING_MODE_2)
        {
            bNumBlocks = (uint8_t)(wSamTxBufferSize) / bIvLen;
            wOption = PH_EXCHANGE_TXCHAINING;
        }
        else
        {
            wOption = PH_EXCHANGE_DEFAULT;
            bNumBlocks = (uint8_t)(wSamTxBufferSize)/bIvLen;
        }

        statusTmp = phhalHw_SamAV2_Cmd_SAM_DecipherData(
            pDataParams->pHalSamDataParams,
            wOption,
            pRecv,
            (uint8_t)(bNumBlocks * bIvLen),
            pLength,
            &pSamBuffer,
            &wSamDataLen
            );

        if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
            {
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00); 
                return statusTmp;
            }
        }
        /* Copy the decrypted data back on the Reader Rx Buffer */
        memcpy(pRecv, pSamBuffer, wSamDataLen);  /* PRQA S 3200 */

        /* Increment wIndex with the amount of data received from SAM */
        wIndex = wIndex + wSamDataLen;
        /* Index to keep track of the next position from where
        to send data to SAM for decryption. This is required
        for cases when using TX_CHAINING option to SAM. In the
        first instance SAM returns 1 block less than the size
        sent to it for decryption. This block is later returned
        in the subsequent calls */
        wRxBufferIndex = wRxBufferIndex + (bNumBlocks * bIvLen);

        /* Chain the data to SAM to be decrypted */
        while (bNumBlocks)
        {
            /* Recalculate bNumBlocks */
            if ((wRxlen - wRxBufferIndex) > wSamTxBufferSize)
            {
                bNumBlocks = (uint8_t)wSamTxBufferSize / bIvLen;
                wLen = bNumBlocks * bIvLen;

                if (wOption & PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH)
                {
                    /* Reset this option from second call onwards */
                    wOption = PH_EXCHANGE_DEFAULT;
                }
                /* else it should already be PH_EXCHANGE_TXCHAINING */
            }
            else
            {
                /* This is the last chunk of the data on pRecv */
                /* The encrypted data from card is always multiple of IV len */
                if ((bOption & 0x0FU) == PH_CRYPTOSYM_PADDING_MODE_2)
                {
                    ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ? (wOption = PH_EXCHANGE_TXCHAINING) : (wOption = PH_EXCHANGE_DEFAULT);
                }
                else
                {
                    wOption = PH_EXCHANGE_DEFAULT;
                }

                bNumBlocks = (uint8_t)(wRxlen - wRxBufferIndex) / bIvLen;

                wLen = bNumBlocks * bIvLen;

                /* Set bNumBlocks to zero here since everything will be now sent to sam in one shot */
                bNumBlocks = 0;

                if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS) && (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE))
                {
                    pRecv[wRxlen++] = 0x00; /* Status byte at the end for CRC verification by SAM */
                    wLen++;  /* Increment wLen due to additional status byte */
                }
            }

            /* Decrypt */
            statusTmp = phhalHw_SamAV2_Cmd_SAM_DecipherData(
                pDataParams->pHalSamDataParams,
                wOption,
                &pRecv[wRxBufferIndex],
                (uint8_t)wLen,
                NULL,
                &pSamBuffer,
                &wSamDataLen
                );

            if (((status == PH_ERR_SUCCESS) && (((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
                && ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))) ||
                (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) && 
                ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)))
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00); 
                }
                return statusTmp;
            }
            /* Put the deciphered data in pSamBuffer onto the Reader HAL Rx buffer */
            memcpy(&pRecv[wIndex], pSamBuffer, wSamDataLen); /* PRQA S 3200 */

            /* wIndex is incremented with the actual data received from SAM */
            wIndex = wIndex + wSamDataLen;
            wRxBufferIndex = wRxBufferIndex + (bNumBlocks * bIvLen);
        }
    }
    else
    {
        wOption = PH_EXCHANGE_DEFAULT;

        /* Data received is less than the size of Sam Tx buffer 
        * Still more data may be expected from the card.
        */

        /* If the length of data to be read is already specified in the API
        then send this option to the decipher data function */
        if (((bOption & 0x0F) == PH_CRYPTOSYM_PADDING_MODE_1) &&
            (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
        {
            /* This option is only applicable for the first frame sent to SAM */
            wOption = PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH;

            if ((status == 0x00) && (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE))
            {
                /* If status from card is already 0x00, then no more data expected to be deciphered */
                pRecv[wRxlen++] = 0x00; /* Status byte for CRC calc. purposes */
            }
        }
        else if ((bOption & 0x0FU) == PH_CRYPTOSYM_PADDING_MODE_2)
        {
            wOption = PH_EXCHANGE_TXCHAINING;

            /* If no more data expected to be deciphered, set wOption to default */
            if (status == 0x00) 
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    pRecv[wRxlen++] = 0x00; /* Status byte for CRC calc. purposes */
                }
                wOption = PH_EXCHANGE_DEFAULT;
            }
        }
        else
        {
            wOption = PH_EXCHANGE_DEFAULT;
            /* If no more data expected to be deciphered, set wOption to default */
            if (status == 0x00) 
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    pRecv[wRxlen++] = 0x00; /* Status byte for CRC calc. purposes */
                }
            }
        }

        /* Decrypt */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_DecipherData(
            pDataParams->pHalSamDataParams,
            wOption,
            pRecv,
            (uint8_t)wRxlen,
            pLength,
            &pSamBuffer,
            &wSamDataLen
            );

        if (((status == PH_ERR_SUCCESS) && ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)) ||
            (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) && 
            ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)))
        {
            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
            {
                /* Authentication should be reset */
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00);
            }
            return statusTmp;
        }

        /* Copy the decrypted data back on the RxBuffer of the Reader HAL */
        memcpy(&pRecv[wIndex], pSamBuffer, wSamDataLen); /* PRQA S 3200 */

        /* Update the length to the length of data returned */
        wIndex = wIndex + wSamDataLen;
    }    

    *ppRxdata = pRecv;
    /* *pRxdataLen = wRxlen; */
    *pRxdataLen = wIndex;

    return status;
}

phStatus_t phalMfdf_SamAV2_Int_IsoRead(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint8_t * bCmdBuff,
                                       uint16_t wCmdLen,
                                       uint8_t ** ppRxBuffer,
                                       uint16_t * pBytesRead
                                       )
{
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint8_t     PH_MEMLOC_REM bNumBlocks;
    uint8_t     PH_MEMLOC_REM bLen;
    uint16_t    PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM statusTmp1;
    uint16_t    PH_MEMLOC_REM wTmp;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wIntOption;
    uint16_t    PH_MEMLOC_REM wRxBufferSize;
    uint16_t    PH_MEMLOC_REM wNextPos = 0;
    uint16_t    PH_MEMLOC_REM wSamTxBufferSize;
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint8_t     PH_MEMLOC_REM bBackupBytes[3];

    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }
    status = phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );

    /* First put everything on the reader Rx buffer upto buffer size - 60 */

    wRxlen = *pBytesRead;
    pRecv = *ppRxBuffer;

    if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        /* Reset authentication status */
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
            pDataParams->pHalSamDataParams,
            0x00); 
        return status;
    }
    while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            ));

        wNextPos = *pBytesRead;
        memcpy(bBackupBytes, &pRecv[wNextPos - 3], 3); /* PRQA S 3200 */

        if (wNextPos + 60 > wRxBufferSize)
        {
            /* Calculate partical cmac if authenticated and return PH_ERR_SUCCESS_CHAINING */
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            ));
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_RXCHAINING,
            bCmdBuff,
            wCmdLen,
            ppRxBuffer,
            pBytesRead
            );

        /* Put back the backed up bytes */
        memcpy(&pRecv[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */

        if ((status != PH_ERR_SUCCESS) && 
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
        {
            if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
            {
                /* Authentication should be reset */
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00);
            }
            return status;
        }
        wRxlen = *pBytesRead;
    }
    /* The data is now in *ppRxBuffer, length = wRxlen */

    if (status == PH_ERR_SUCCESS)
    {
        statusTmp = pRecv[wRxlen - 2];
        statusTmp <<= 8;
        statusTmp |= pRecv[wRxlen - 1];
        (*pBytesRead) -= 2;
        status = phalMfdf_Int_ComputeErrorResponse(pDataParams, statusTmp);
        if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
        {
            return status;
        }
        if (status != PH_ERR_SUCCESS)
        {
            /* Authentication should be reset */
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00); 
            return status;
        }
    }
    else
    {
        if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
    }

    /* Size of MAC bytes */
    wTmp = 0x08;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_ON
        ));
    /* Get the length of the SAM HAL Tx buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE,
        &wSamTxBufferSize
        ));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING,
        PH_OFF
        ));

    if (wRxlen > wSamTxBufferSize)
    {
        /* Index of data on Reader Rx Buffer */
        wIndex = 0;
        wIntOption = PH_EXCHANGE_TXCHAINING;
        bNumBlocks = (uint8_t)(wSamTxBufferSize)/bIvLen;
        bLen = bNumBlocks * bIvLen;

        do
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                wIntOption,
                0x00,
                &pRecv[wIndex],
                bLen
                );

            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    /* Authentication should be reset */
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00);
                }
                return statusTmp;
            }

            /* Increment wIndex with the amount of data received from SAM */
            wIndex = wIndex + bLen;

            if ((wRxlen - wIndex) > wSamTxBufferSize)
            {
                bNumBlocks = (uint8_t)wSamTxBufferSize / bIvLen;
                bLen = bNumBlocks * bIvLen;
            }
            else
            {
                bLen = (uint8_t)(wRxlen - wIndex);
                if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
                {
                    statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                        pDataParams->pHalSamDataParams,
                        PH_EXCHANGE_TXCHAINING,
                        0x00,
                        &pRecv[wIndex],
                        bLen
                        );
                    bLen = 0;
                }
                else /* ((status & PH_ERR_MASK) == PH_ERR_SUCCESS) */
                {
                    memcpy(bCMacCard, &pRecv[wRxlen - wTmp - 2], wTmp); /* PRQA S 3200 */
                    bLen = bLen - (uint8_t)wTmp - 2;

                    statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                        pDataParams->pHalSamDataParams,
                        PH_EXCHANGE_TXCHAINING,
                        0x00,
                        &pRecv[wIndex],
                        bLen
                        );

                    if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
                    {
                        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                        {
                            /* Authentication should be reset */
                            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                            statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                pDataParams->pHalSamDataParams,
                                0x00);
                        }
                        return statusTmp;
                    }

                    /* Next buffer the status */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                        pDataParams->pHalSamDataParams,
                        PH_EXCHANGE_BUFFER_FIRST,
                        0x00,
                        (uint8_t *)&status,
                        0x01
                        ));

                    /* Lastly send it along with the MAC */
                    statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                        pDataParams->pHalSamDataParams,
                        PH_EXCHANGE_BUFFER_LAST,
                        0x00,
                        bCMacCard,
                        (uint8_t)wTmp
                        );

                    if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
                    {
                        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                        {
                            /* Authentication should be reset */
                            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                                pDataParams->pHalSamDataParams,
                                0x00);
                        }
                        return statusTmp;
                    }
                    /* Set bLen to zero. No more looping */
                    bLen = 0;
                }
            }
        }
        while (bLen);
    }
    else
    {
        if (status == PH_ERR_SUCCESS)
        {
            memcpy(bCMacCard, &pRecv[wRxlen - wTmp - 2], wTmp); /* PRQA S 3200 */
            wRxlen = wRxlen - wTmp - 2;

            statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                PH_EXCHANGE_TXCHAINING,
                0x00,
                pRecv,
                (uint8_t)wRxlen
                );

            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    /* Authentication should be reset */
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00);
                }
                return statusTmp;
            }

            /*  Next goes the status byte */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                PH_EXCHANGE_BUFFER_FIRST,
                0x00,
                (uint8_t *)&status,
                0x01
                ));

            /* Last frame of data. MAC goes here */
            statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                0x00,
                bCMacCard,
                (uint8_t)wTmp
                );
            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    /* Authentication should be reset */
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00);
                }
                return statusTmp;
            }
        }
        else
        {
            statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
                pDataParams->pHalSamDataParams,
                PH_EXCHANGE_TXCHAINING,
                0x00,
                pRecv,
                (uint8_t)wRxlen
                );

            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
                {
                    /* Authentication should be reset */
                    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                    statusTmp1 = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                        pDataParams->pHalSamDataParams,
                        0x00);
                }
                return statusTmp;
            }
        }
    }
    if (status == PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
    }
}

#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */
