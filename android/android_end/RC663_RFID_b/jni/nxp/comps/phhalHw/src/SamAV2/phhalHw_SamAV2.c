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
* SAMAV2 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.12 $
* $Date: Fri Apr 15 09:27:58 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phbalReg.h>
#include <phhalHw.h>
#include <phCryptoSym.h>
#include <ph_RefDefs.h>
#include <phTools.h>

#ifdef NXPBUILD__PHHAL_HW_SAMAV2

#include "phhalHw_SamAV2.h"
#include "phhalHw_SamAV2_Int.h"
#include <phhalHw_SamAV2_Cmd.h>
#include "Hc_AV1/phhalHw_SamAV2_Hc_AV1.h"
#include "Hc_AV2/phhalHw_SamAV2_Hc_AV2.h"
#include "Hc_AV2/phhalHw_SamAV2_Hc_AV2_Int.h"
#include "phhalHw_SamAV2_HcUtils.h"

/* Default shadow for ISO14443-3A Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wSamAV2_DefaultShadow_I14443a[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_ON},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXDEAFBITS,            0x0008},
    {PHHAL_HW_CONFIG_TXDATARATE,            PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_RXDATARATE,            PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_SAMAV2_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_ASK100,                PH_ON}
};

phStatus_t phhalHw_SamAV2_Init(
                               phhalHw_SamAV2_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams,
                               void * pBalDataParams,
                               void * pReaderHalDataParams,
                               void * pKeyStoreDataParams,
                               void * pCryptoENCDataParams,
                               void * pCryptoMACDataParams,
                               void * pCryptoRngDataParams,
                               uint8_t bOpMode,
                               uint8_t bLogicalChannel,
                               uint8_t* pTxBuffer,
                               uint16_t wTxBufSize,
                               uint8_t* pRxBuffer,
                               uint16_t wRxBufSize
                               )
{
    if (sizeof(phhalHw_SamAV2_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pBalDataParams);
    PH_ASSERT_NULL (pCryptoENCDataParams);
    PH_ASSERT_NULL (pCryptoMACDataParams);
    PH_ASSERT_NULL (pCryptoRngDataParams);
    PH_ASSERT_NULL (pTxBuffer);
    PH_ASSERT_NULL (pRxBuffer);

    /* Init. private data */
    pDataParams->wId                            = PH_COMP_HAL | PHHAL_HW_SAMAV2_ID;
    pDataParams->pBalDataParams                 = pBalDataParams;
    pDataParams->pReaderHalDataParams           = pReaderHalDataParams;
    pDataParams->pKeyStoreDataParams            = pKeyStoreDataParams;
    pDataParams->pENCCryptoDataParams           = pCryptoENCDataParams;
    pDataParams->pMACCryptoDataParams           = pCryptoMACDataParams;
    pDataParams->pCryptoRngDataParams           = pCryptoRngDataParams;
    pDataParams->Cmd_Ctr                        = 0;
    pDataParams->bHostMode                      = PHHAL_HW_SAMAV2_HC_AV1_MODE;
    pDataParams->bAuthType                      = 0x00;
    pDataParams->bPendingEncCmdDataLength       = 0;
    pDataParams->bPendingMacCmdDataLength       = 0;
    pDataParams->bPendingMacRespDataLength      = 0;
    pDataParams->bCmdSM                         = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
    pDataParams->bRespSM                        = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
    pDataParams->bCommandChaining               = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
    pDataParams->bResponseChaining              = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
    pDataParams->bMasterKeyCmacMode             = PH_OFF;
    pDataParams->bChangeEntryPlain              = PH_OFF;
    pDataParams->bOpMode                        = bOpMode;
    pDataParams->bLogicalChannel                = bLogicalChannel;
    pDataParams->pTxBuffer                      = pTxBuffer;
    pDataParams->wTxBufSize                     = wTxBufSize;
    pDataParams->wTxBufLen                      = 0;
    pDataParams->pRxBuffer                      = pRxBuffer;
    pDataParams->wRxBufSize                     = wRxBufSize;
    pDataParams->wRxBufLen                      = 0;
    pDataParams->wRxBufStartPos                 = 0;
    pDataParams->wTxBufStartPos                 = 0;
    pDataParams->bCardType                      = PHHAL_HW_CARDTYPE_ISO14443A;
    pDataParams->bTimeoutUnit                   = PHHAL_HW_TIME_MICROSECONDS;
    pDataParams->wFieldOffTime                  = PHHAL_HW_FIELD_OFF_DEFAULT;
    pDataParams->wFieldRecoveryTime             = PHHAL_HW_FIELD_RECOVERY_DEFAULT;
    pDataParams->wAdditionalInfo                = 0;
    pDataParams->wTimingMode                    = PHHAL_HW_TIMING_MODE_OFF;
    pDataParams->dwTimingUs                     = 0;
    pDataParams->bMifareCryptoDisabled          = PH_ON;
    pDataParams->bRfResetAfterTo                = PH_OFF;
    pDataParams->bDisableNonXCfgMapping         = PH_OFF;

    /* Verify exchange buffers */
    if ((wTxBufSize <= PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN) || (wRxBufSize <= PHHAL_HW_SAMAV2_RESERVED_RX_BUFFER_LEN))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Verify operation mode */
    if (bOpMode > PHHAL_HW_SAMAV2_OPMODE_X_RC663)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Verify NonX reader HAL pointer */
    if ((bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X) && (pReaderHalDataParams == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Exchange(
                                   phhalHw_SamAV2_DataParams_t * pDataParams,
                                   uint16_t wOption,
                                   uint8_t * pTxBuffer,
                                   uint16_t wTxLength,
                                   uint8_t ** ppRxBuffer,
                                   uint16_t * pRxLength
                                   )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM bLeByte = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;
    uint16_t    PH_MEMLOC_REM wValidBits;
    uint16_t    PH_MEMLOC_REM wParity = PH_OFF;
    uint16_t    PH_MEMLOC_REM wCrc = PH_OFF;
    uint32_t    PH_MEMLOC_REM dwTimingSingle;
    uint8_t *	PH_MEMLOC_REM pRxBuffer;
    uint16_t	PH_MEMLOC_REM wRxLength;
    uint16_t    PH_MEMLOC_REM wTxStartPosTmp = 0;

    /* Check options */
    if (wOption & (uint16_t)~(uint16_t)(PH_EXCHANGE_BUFFERED_BIT | PH_EXCHANGE_LEAVE_BUFFER_BIT | PHHAL_HW_SAMAV2_EXCHANGE_NO_ENCIPHERING_BIT | PHHAL_HW_SAMAV2_EXCHANGE_NO_DECIPHERING_BIT))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Parameter check */
    if ((wOption & PH_EXCHANGE_BUFFERED_BIT) && (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] > 0))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Check if caller has provided valid RxBuffer */
    if (ppRxBuffer == NULL)
    {
        ppRxBuffer = &pRxBuffer;
    }
    if (pRxLength == NULL)
    {
        pRxLength = &wRxLength;
    }

    *pRxLength = 0;
    pDataParams->wRxBufLen = pDataParams->wRxBufStartPos;
    pDataParams->wAdditionalInfo = 0;

    /* Data enciphering  */
    if (!(pDataParams->bMifareCryptoDisabled) && !(wOption & PHHAL_HW_SAMAV2_EXCHANGE_NO_ENCIPHERING_BIT))
    {
        wRxLength = wTxLength;

        /* Non-X Mode: Sync. RxStartPos with Reader */
        if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                pDataParams->pReaderHalDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                &pDataParams->wRxBufLen));
        }

        /* Encipher transmission data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_EncipherData(
            pDataParams,
            wOption & (uint16_t)~(uint16_t)PH_EXCHANGE_CUSTOM_BITS_MASK,
            pTxBuffer,
            (uint8_t)wTxLength,
            0x00,
            &pTxBuffer,
            &wTxLength));

        /* Return after buffering */
        if (wOption & PH_EXCHANGE_BUFFERED_BIT)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
        }
        /* Else perform default exchange since buffering has already been done */
        else
        {
            wOption &= PH_EXCHANGE_CUSTOM_BITS_MASK;
        }

        /* Get amount of complete bytes */
        pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] = (uint8_t)(wTxLength % 9);
        if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] != 0x00)
        {
            --pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS];
        }

        /* Non-X Mode : Modify Parity and CRC settings */
        if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
        {
            /* Retrieve Parity-setting */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_PARITY, &wParity));

            /* Disable Parity */
            if (wParity != PH_OFF)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_PARITY, PH_OFF));
            }

            /* Disable TxCrc */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));

            /* Retrieve RxCrc-setting */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXCRC, &wCrc));

            /* Disable RxCrc */
            if (wCrc != PH_OFF)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));
            }

            /* Set TxLastBits */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, (uint16_t)pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS]));
        }
        /* X-Mode: Retrieve RxCrc status (for Mfc Decipher later on) */
        else
        {
            wCrc = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXCRC];
        }
    }

    /* Non-X Mode : Exchange via Reader HAL */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        /* Perform Exchange */
        status = phhalHw_Exchange(
            pDataParams->pReaderHalDataParams,
            wOption & (uint16_t)~(uint16_t)PH_EXCHANGE_CUSTOM_BITS_MASK,
            pTxBuffer,
            wTxLength,
            ppRxBuffer,
            pRxLength);

        /* do not perform real exchange, just fill the global TxBuffer */
        if (wOption & PH_EXCHANGE_BUFFERED_BIT)
        {
            return status;
        }

        /* Restore Parity-setting again since many PAL layers expect it */
        if (wParity != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_PARITY, PH_ON));
        }

        /* Restore RxCRC-setting again */
        if (wCrc != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
        }

        /* Retrieve RxLastBits */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXLASTBITS, &wValidBits));
        pDataParams->wAdditionalInfo = (uint8_t)wValidBits;

        /* Clear TxLastBits */
        pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] = 0;

        /* status check */
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            PH_CHECK_SUCCESS(status);
        }
    }
    /* X Mode : Exchange ourself */
    else
    {
        /* Mifare CRYPTO and TxBuffer equals RxBuffer -> ensure that encrypted data does not get overwritten */
        if (!(pDataParams->bMifareCryptoDisabled) && !(wOption & PHHAL_HW_SAMAV2_EXCHANGE_NO_ENCIPHERING_BIT))
        {
            wTxStartPosTmp = pDataParams->wTxBufStartPos;
            pDataParams->wTxBufStartPos = pDataParams->wTxBufStartPos + wTxLength;
        }

        /* Prepend Header */
        if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
        {
            /* Filling in ISO7816 header */
            aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
            aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS;
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0x00;
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0x00;

            /* Buffer header */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_FIRST,
                aCmd,
                PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
                NULL,
                NULL));
        }

        /* Check for buffer overflow (include LE) */
        if ((pDataParams->wTxBufStartPos + pDataParams->wTxBufLen + wTxLength + 1) > pDataParams->wTxBufSize)
        {
            return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }

        /* Append data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pTxBuffer,
            wTxLength,
            NULL,
            NULL));

        /* if we do not perform real exchange we're finished */
        if (wOption & PH_EXCHANGE_BUFFERED_BIT)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
        }

        /* Modify P1 byte (TxLastBits) */
        pDataParams->pTxBuffer[pDataParams->wTxBufStartPos + PHHAL_HW_SAMAV2_ISO7816_P1_POS] = (uint8_t)pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS];

        /* Modify LC byte */
        pDataParams->pTxBuffer[pDataParams->wTxBufStartPos + PHHAL_HW_SAMAV2_ISO7816_LC_POS] = (uint8_t)(pDataParams->wTxBufLen - PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH);

        /* Append LE byte and exchange */
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            &bLeByte,
            1,
            ppRxBuffer,
            pRxLength);

        /* Mifare CRYPTO and TxBuffer equals RxBuffer -> restore TxStartPos */
        if (!(pDataParams->bMifareCryptoDisabled) && !(wOption & PHHAL_HW_SAMAV2_EXCHANGE_NO_ENCIPHERING_BIT))
        {
            pDataParams->wTxBufStartPos = wTxStartPosTmp;
        }

        /* Clear TxLastBits */
        if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] > 0)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0));
            if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_X_RC663)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0));
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0));
            }
        }

        /* Reset buffered bytes */
        pDataParams->wTxBufLen = 0;

        /* Timing */
        if ((pDataParams->wTimingMode & (uint16_t)~(uint16_t)PHHAL_HW_TIMING_MODE_OPTION_MASK) == PHHAL_HW_TIMING_MODE_FDT)
        {
            /* Retrieve FDT Value */
            if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_X_RC663)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_GetFdt(pDataParams, status, &dwTimingSingle));
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_GetFdt(pDataParams, &dwTimingSingle));
            }

            /* FDT single -> replace the previous value */
            if (pDataParams->wTimingMode & PHHAL_HW_TIMING_MODE_OPTION_AUTOCLEAR)
            {
                pDataParams->dwTimingUs = dwTimingSingle;
            }
            /* FDT -> add current value to current count */
            else
            {
                /* Check for overflow */
                if ((0xFFFFFFFF - pDataParams->dwTimingUs) < dwTimingSingle)
                {
                    pDataParams->dwTimingUs = 0xFFFFFFFF;
                }
                /* Update global timing value */
                else
                {
                    pDataParams->dwTimingUs = pDataParams->dwTimingUs + dwTimingSingle;
                }
            }
        }

        /* Allow incomplete byte error */
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            /* Reset after timeout functionality */
            if ((pDataParams->bRfResetAfterTo != PH_OFF) &&
                ((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_FieldReset(pDataParams));
            }

            /* Collision error together with no data means protocol error for the library */
            if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) && (*pRxLength == 0))
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
            }

            /* check return code */
            PH_CHECK_SUCCESS(status);
        }
    }

    /* Data deciphering  */
    if (!(pDataParams->bMifareCryptoDisabled) &&
        (!(wOption & PHHAL_HW_SAMAV2_EXCHANGE_NO_DECIPHERING_BIT) ||
        ((*pRxLength == 1) && (pDataParams->wAdditionalInfo == 4))))
    {
        /* Non-X Mode: RxStartPos is RxLength in this case */
        if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
        {
            pDataParams->wRxBufStartPos = pDataParams->wRxBufLen;
        }

        /* Ignore data before RxStartPos for decryption */
        *ppRxBuffer += pDataParams->wRxBufStartPos;
        *pRxLength = *pRxLength - pDataParams->wRxBufStartPos;

        /* Perform actual deciphering */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_DecipherData(
            pDataParams, 
            PH_EXCHANGE_DEFAULT, 
            *ppRxBuffer,
            (uint8_t)*pRxLength, 
            NULL,
            ppRxBuffer, 
            pRxLength);

        /* Bail out on Error */
        if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            /* In NonX-Mode, reset RxStartPos */
            if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
            {
                pDataParams->wRxBufStartPos = 0;
                pDataParams->wRxBufLen = 0;
            }
            return statusTmp;
        }

        /* Received streams which are not ACK / NACK */
        if ((*pRxLength != 1)  || (pDataParams->wAdditionalInfo != 4))
        {
            /* DecipherData removes CRC, so calculate it again if it is expected */
            if (wCrc == PH_OFF)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                    PH_TOOLS_CRC_OPTION_DEFAULT,
                    PH_TOOLS_CRC16_PRESET_ISO14443A,
                    PH_TOOLS_CRC16_POLY_ISO14443,
                    *ppRxBuffer,
                    *pRxLength,
                    &wCrc));

                (*ppRxBuffer)[(*pRxLength)++] = (uint8_t)(wCrc);
                (*ppRxBuffer)[(*pRxLength)++] = (uint8_t)(wCrc >> 8);
            }

            /* Always byte-aligned after decryption */
            pDataParams->wAdditionalInfo = 0;
            status = PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
        }

        /* Always return complete buffer on exchange */
        *pRxLength = *pRxLength + pDataParams->wRxBufStartPos;
        *ppRxBuffer = pDataParams->pRxBuffer;
        pDataParams->wRxBufLen = *pRxLength;

        /* In NonX-Mode, reset RxStartPos */
        if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
        {
            pDataParams->wRxBufStartPos = 0;
        }
    }

    return status;
}

phStatus_t phhalHw_SamAV2_WriteRegister(
                                        phhalHw_SamAV2_DataParams_t * pDataParams,
                                        uint8_t bAddress,
                                        uint8_t bValue
                                        )
{
    uint8_t PH_MEMLOC_REM aData[2];

    /* In case of Non-X mode, the WriteRegister is directly redirected to the Reader IC */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        return phhalHw_WriteRegister(pDataParams->pReaderHalDataParams, bAddress, bValue);
    }

    aData[0] = bAddress;
    aData[1] = bValue;

    /* perform command */
    return phhalHw_SamAV2_Cmd_RC_WriteRegister(pDataParams, aData, 2);
}

phStatus_t phhalHw_SamAV2_ReadRegister(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bAddress,
                                       uint8_t * pValue
                                       )
{
    /* In case of Non-X mode, the ReadRegister is directly redirected to the Reader IC */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        return phhalHw_ReadRegister(pDataParams->pReaderHalDataParams, bAddress, pValue);
    }

    /* perform command */
    return phhalHw_SamAV2_Cmd_RC_ReadRegister(pDataParams, &bAddress, 1, pValue);
}

phStatus_t phhalHw_SamAV2_DetectMode(
                                     phhalHw_SamAV2_DataParams_t * pDataParams
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCompleteUid[PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_LENGTH];
    uint8_t     PH_MEMLOC_REM bCompleteUidLength;
    uint8_t     PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE];
    uint8_t     PH_MEMLOC_REM bKeyEntryLength;
    uint8_t     PH_MEMLOC_REM bSet1;

    /* Issue GetVersion command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetVersion(
        pDataParams,
        bCompleteUid,
        &bCompleteUidLength));

    /* Check length of received response */
    if (bCompleteUidLength == PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_LENGTH)
    {
        /* Retrieve Host-Mode */
        switch (bCompleteUid[PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_HOSTMODE_POS])
        {
        case 0xA1:
            pDataParams->bHostMode = PHHAL_HW_SAMAV2_HC_AV1_MODE;
            break;
        case 0xA2:
            pDataParams->bHostMode = PHHAL_HW_SAMAV2_HC_AV2_MODE;
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INTERFACE_ERROR, PH_COMP_HAL);
        }
    }
    /* Older SAM AV1 versions do not send the AV1/AV2 byte at the end */
    else if (bCompleteUidLength == (PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_LENGTH - 1))
    {
        pDataParams->bHostMode = PHHAL_HW_SAMAV2_HC_AV1_MODE;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Store the UID globally */
    memcpy(   /* PRQA S 3200 */
        pDataParams->bUid,
        &bCompleteUid[PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_UID_OFFSET],
        PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);

    /* Retrieve status of MasterKey */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams,
        0x00,
        pKeyEntryBuffer,
        &bKeyEntryLength
        ));

    /* check for AV1 mode */
    if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        bSet1 = pKeyEntryBuffer[bKeyEntryLength - 1];
    }
    /* AV2 mode */
    else
    {
        bSet1 = pKeyEntryBuffer[bKeyEntryLength - 2];
    }

    /* Check if CMAC mode is enabled */
    if (bSet1 & 0x01)
    {
        pDataParams->bMasterKeyCmacMode = PH_ON;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_ApplyProtocolSettings(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCardType
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_COUNT wIndex;
    uint16_t *  PH_MEMLOC_REM pShadowDefault;
    uint16_t    PH_MEMLOC_REM wShadowCount;
    uint8_t     PH_MEMLOC_REM bUseDefaultShadow;
    uint16_t    PH_MEMLOC_REM wConfig;

    /* MIFARE Crypto1 state is disabled by default */
    pDataParams->bMifareCryptoDisabled = PH_ON;

    /* In case of Non-X mode, the ApplyProtocolSettings is directly redirected to the Reader IC */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        return phhalHw_ApplyProtocolSettings(pDataParams->pReaderHalDataParams, bCardType);
    }

    /* We only support ISO 14443A */
    if (pDataParams->bCardType != PHHAL_HW_CARDTYPE_ISO14443A)
    {
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
    }

    /* Store new card type */
    if (bCardType != PHHAL_HW_CARDTYPE_CURRENT)
    {
        pDataParams->bCardType = bCardType;
        pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MICROSECONDS;
        bUseDefaultShadow = 1;

        /* Initialize config shadow */
        memset(pDataParams->wCfgShadow, 0x00, PHHAL_HW_RC523_SHADOW_COUNT);  /* PRQA S 3200 */
    }
    else
    {
        bUseDefaultShadow = 0;
    }

    /* Use 14443a default shadow */
    pShadowDefault = (uint16_t*)wSamAV2_DefaultShadow_I14443a;
    wShadowCount = sizeof(wSamAV2_DefaultShadow_I14443a) / (sizeof(uint16_t) * 2);

    /* Apply shadowed registers */
    for (wIndex = 0; wIndex < wShadowCount; ++wIndex)
    {
        /* Get wConfig */
        wConfig = pShadowDefault[wIndex << 1];

        /* Apply only one the correct timeout unit */
        if (!(((wConfig == PHHAL_HW_CONFIG_TIMEOUT_VALUE_US) &&
            (pDataParams->bTimeoutUnit != PHHAL_HW_TIME_MICROSECONDS)) ||
            ((wConfig == PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS) &&
            (pDataParams->bTimeoutUnit != PHHAL_HW_TIME_MILLISECONDS))))
        {
            /* Default shadow: */
            if (bUseDefaultShadow)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_SetConfig(pDataParams, wConfig, pShadowDefault[(wIndex << 1) + 1]));
            }
            /* Current shadow: */
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_SetConfig(pDataParams, wConfig, pDataParams->wCfgShadow[wConfig]));
            }
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_SetConfig(
                                    phhalHw_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* In case of Non-X mode, the SetConfig is directly redirected to the Reader IC if not disabled. */
    /* Exceptions are: Crypto1 and custom configs. */
    if (pDataParams->bDisableNonXCfgMapping == PH_OFF)
    {
        if ((pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X) &&
            (wConfig != PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1) &&
            (wConfig != PHHAL_HW_SAMAV2_CONFIG_HOSTMODE) &&
            (wConfig != PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING))
        {
            return phhalHw_SetConfig(pDataParams->pReaderHalDataParams, wConfig, wValue);
        }
    }

    switch (wConfig)
    {
    case PHHAL_HW_CONFIG_TXLASTBITS:

        /* check parameter */
        if (wValue > 7)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TIMING_MODE:

        /* Check supported option bits */
        switch (wValue & PHHAL_HW_TIMING_MODE_OPTION_MASK)
        {
        case PHHAL_HW_TIMING_MODE_OPTION_DEFAULT:
        case PHHAL_HW_TIMING_MODE_OPTION_AUTOCLEAR:
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Check supported timing modes */
        switch (wValue & (uint16_t)~(uint16_t)PHHAL_HW_TIMING_MODE_OPTION_MASK)
        {
        case PHHAL_HW_TIMING_MODE_OFF:
        case PHHAL_HW_TIMING_MODE_FDT:
            pDataParams->dwTimingUs = 0;
            pDataParams->wTimingMode = wValue;
            break;
        case PHHAL_HW_TIMING_MODE_COMM:
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        break;

    case PHHAL_HW_CONFIG_FIELD_OFF_TIME:

        /* Parameter Check */
        if ((wValue == 0) || (wValue > 0xFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Store config data */
        pDataParams->wFieldOffTime = wValue;
        break;

    case PHHAL_HW_CONFIG_FIELD_RECOVERY_TIME:

        /* Store config data */
        pDataParams->wFieldRecoveryTime = wValue;
        break;

    case PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1:

        pDataParams->bMifareCryptoDisabled = (uint8_t)wValue;
        if (wValue != PH_OFF)
        {
            /* We also need to reset the authentication inside of the SAM */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams, PH_ON));
        }
        break;

    case PHHAL_HW_CONFIG_ADDITIONAL_INFO:

        /* Modify additional info parameter */
        pDataParams->wAdditionalInfo = wValue;
        break;

    case PHHAL_HW_CONFIG_RXBUFFER_STARTPOS:

        /* Boundary check */
        if ((PHHAL_HW_SAMAV2_RESERVED_RX_BUFFER_LEN + wValue) >= pDataParams->wRxBufSize)
        {
            return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }

        /* Set start position */
        pDataParams->wRxBufStartPos = wValue;
        pDataParams->wRxBufLen = wValue;

        /* Preserve RxBuffer contents if needed */
        if (pDataParams->pTxBuffer == pDataParams->pRxBuffer)
        {
            pDataParams->wTxBufStartPos = pDataParams->wRxBufStartPos;
        }
        else
        {
            pDataParams->wTxBufStartPos = 0;
        }
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_LENGTH:

        /* Needed for Mifare Encrypted buffered data */
        if (pDataParams->wTxBufLen_Cmd >= (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1))
        {
            /* Check parameter */
            if (((PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1) + pDataParams->wTxBufStartPos + wValue) > pDataParams->wTxBufSize)
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* set buffer length */
            pDataParams->wTxBufLen_Cmd = (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1) + wValue;
        }
        /* Normal Exchange */
        else
        {
            /* Check parameter */
            if ((PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + pDataParams->wTxBufStartPos + wValue) > pDataParams->wTxBufSize)
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* set buffer length */
            pDataParams->wTxBufLen = PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + wValue;
        }
        break;

    case PHHAL_HW_CONFIG_TXBUFFER:

        /* Needed for Mifare Encrypted buffered data */
        if (pDataParams->wTxBufLen_Cmd >= (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1))
        {
            /* Check additional info parameter */
            if (((PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1) + pDataParams->wTxBufStartPos + pDataParams->wAdditionalInfo) >= pDataParams->wTxBufSize)
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* Modify TxBuffer byte */
            pDataParams->pTxBuffer[(PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1) + pDataParams->wTxBufStartPos + pDataParams->wAdditionalInfo] = (uint8_t)wValue;
        }
        /* Normal Exchange */
        else
        {
            /* Check additional info parameter */
            if ((PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + pDataParams->wTxBufStartPos + pDataParams->wAdditionalInfo) >= pDataParams->wTxBufSize)
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* Modify TxBuffer byte */
            pDataParams->pTxBuffer[PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + pDataParams->wTxBufStartPos + pDataParams->wAdditionalInfo] = (uint8_t)wValue;
        }
        break;

    case PHHAL_HW_CONFIG_RFRESET_ON_TIMEOUT:

        if (wValue == PH_OFF)
        {
            pDataParams->bRfResetAfterTo = PH_OFF;
        }
        else
        {
            pDataParams->bRfResetAfterTo = PH_ON;
        }        
        break;

    case PHHAL_HW_SAMAV2_CONFIG_HOSTMODE:

        pDataParams->bHostMode = (uint8_t)wValue;
        break;

    case PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING:

        if (wValue != PH_OFF)
        {
            pDataParams->bDisableNonXCfgMapping = PH_ON;
        }
        else
        {
            pDataParams->bDisableNonXCfgMapping = PH_OFF;
        }
        break;

    default:

        /* Perform ReaderIC specific configuration */
        if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_X_RC663)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, wConfig, wValue));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetConfig(pDataParams, wConfig, wValue));
        }

    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_GetConfig(
                                    phhalHw_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    )
{
    /* In case of Non-X mode, the GetConfig is directly redirected to the Reader IC if not disabled. */
    /* Exceptions are: RxLastbits, Crypto1 and custom configs. */
    if (pDataParams->bDisableNonXCfgMapping == PH_OFF)
    {
        if ((pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X) &&
            (wConfig != PHHAL_HW_CONFIG_RXLASTBITS) &&
            (wConfig != PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1) &&
            (wConfig != PHHAL_HW_SAMAV2_CONFIG_HOSTMODE) &&
            (wConfig != PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING))
        {
            /* Also do not forward if TxBufferLen is requested and MfCrypto is enabled (buffering is done in this HAL) */
            if (!((wConfig == PHHAL_HW_CONFIG_TXBUFFER_LENGTH) && (!pDataParams->bMifareCryptoDisabled)))
            {
                return phhalHw_GetConfig(pDataParams->pReaderHalDataParams, wConfig, pValue);
            }
        }
    }

    switch (wConfig)
    {
    case PHHAL_HW_CONFIG_PARITY:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_TXCRC:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_RXCRC:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_TXLASTBITS:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_ADDITIONAL_INFO:
    case PHHAL_HW_CONFIG_RXLASTBITS:

        *pValue = pDataParams->wAdditionalInfo;
        break;

    case PHHAL_HW_CONFIG_RXDEAFBITS:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_CLEARBITSAFTERCOLL:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_TXDATARATE:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_RXDATARATE:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_MODINDEX:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_ASK100:

        /* Read config from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:

        if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
        {
            *pValue = pDataParams->wCfgShadow[wConfig];
        }
        else
        {
            if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS] > (0xFFFF / 1000))
            {
                return PH_ADD_COMPCODE(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
            }
            *pValue = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS] * 1000;
        }
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:

        if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MILLISECONDS)
        {
            *pValue = pDataParams->wCfgShadow[wConfig];
        }
        else
        {
            *pValue = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_US] / 1000;
        }
        break;

    case PHHAL_HW_CONFIG_TIMING_MODE:

        *pValue = pDataParams->wTimingMode;
        break;

    case PHHAL_HW_CONFIG_TIMING_US:

        if (pDataParams->dwTimingUs > 0xFFFF)
        {
            return PH_ADD_COMPCODE(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
        }

        *pValue = (uint16_t)pDataParams->dwTimingUs;
        pDataParams->dwTimingUs = 0;
        break;

    case PHHAL_HW_CONFIG_TIMING_MS:

        if (pDataParams->dwTimingUs > (0xFFFF * 1000))
        {
            pDataParams->dwTimingUs = 0;
            return PH_ADD_COMPCODE(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
        }

        *pValue = (uint16_t)(pDataParams->dwTimingUs / 1000);
        pDataParams->dwTimingUs = 0;
        break;

    case PHHAL_HW_CONFIG_FIELD_OFF_TIME:

        *pValue = pDataParams->wFieldOffTime;
        break;

    case PHHAL_HW_CONFIG_FIELD_RECOVERY_TIME:

        *pValue = pDataParams->wFieldRecoveryTime;
        break;

    case PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1:

        *pValue = pDataParams->bMifareCryptoDisabled;
        break;

    case PHHAL_HW_CONFIG_RXBUFFER_STARTPOS:

        /* Return parameter */
        *pValue = pDataParams->wRxBufStartPos;
        break;

    case PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE:

        /* Return parameter */
        *pValue = pDataParams->wRxBufSize - PHHAL_HW_SAMAV2_RESERVED_RX_BUFFER_LEN;
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE:

        /* Return parameter */
        *pValue = pDataParams->wTxBufSize - (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + pDataParams->wTxBufStartPos);
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_LENGTH:

        /* Needed for Mifare Encrypted buffered data */
        if (pDataParams->wTxBufLen_Cmd >= (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1))
        {
            *pValue = pDataParams->wTxBufLen_Cmd - (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1);
        }
        else
        {
            /* Normal Exchange */
            if (pDataParams->wTxBufLen >= PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN)
            {
                *pValue = pDataParams->wTxBufLen - PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN;
            }
            else
            {
                *pValue = 0;
            }
        }
        break;

    case PHHAL_HW_CONFIG_TXBUFFER:

        /* Needed for Mifare Encrypted buffered data */
        if (pDataParams->wTxBufLen_Cmd >= (PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1))
        {
            /* Check additional info parameter */
            if (((PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1) + pDataParams->wAdditionalInfo) >= pDataParams->wTxBufSize)
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* Return TxBuffer byte */
            *pValue = (uint16_t)pDataParams->pTxBuffer[(PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN - 1) + pDataParams->wTxBufStartPos + pDataParams->wAdditionalInfo];
        }
        /* Normal Exchange */
        else
        {
            /* Check additional info parameter */
            if ((PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + pDataParams->wAdditionalInfo) >= pDataParams->wTxBufSize)
            {
                return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* Return TxBuffer byte */
            *pValue = (uint16_t)pDataParams->pTxBuffer[PHHAL_HW_SAMAV2_RESERVED_TX_BUFFER_LEN + pDataParams->wTxBufStartPos + pDataParams->wAdditionalInfo];
        }
        break;

    case PHHAL_HW_CONFIG_RFRESET_ON_TIMEOUT:

        *pValue = (uint16_t)pDataParams->bRfResetAfterTo;
        break;

    case PHHAL_HW_SAMAV2_CONFIG_HOSTMODE:

        *pValue = pDataParams->bHostMode;
        break;

    case PHHAL_HW_SAMAV2_CONFIG_DISABLE_NONX_CFG_MAPPING:

        *pValue = pDataParams->bDisableNonXCfgMapping;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_FieldOn(
                                  phhalHw_SamAV2_DataParams_t * pDataParams
                                  )
{
    /* In case of Non-X mode, the FieldOn is directly redirected to the Reader IC */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        return phhalHw_FieldOn(pDataParams->pReaderHalDataParams);
    }

    return phhalHw_SamAV2_Cmd_RC_RFControl(pDataParams,1);
}

phStatus_t phhalHw_SamAV2_FieldOff(
                                   phhalHw_SamAV2_DataParams_t * pDataParams
                                   )
{
    /* In case of Non-X mode, the FieldOff is directly redirected to the Reader IC */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        return phhalHw_FieldOff(pDataParams->pReaderHalDataParams);
    }
    return phhalHw_SamAV2_Cmd_RC_RFControl(pDataParams,0);
}

phStatus_t phhalHw_SamAV2_FieldReset(
                                     phhalHw_SamAV2_DataParams_t * pDataParams
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* In case of Non-X mode, the FieldReset is directly redirected to the Reader */
    if (pDataParams->bOpMode == PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        return phhalHw_FieldReset(pDataParams->pReaderHalDataParams);
    }

    /* Perform field reset */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_RC_RFControl(pDataParams, (uint8_t)pDataParams->wFieldOffTime));

    /* Wait recovery time */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Wait(
        pDataParams,
        PHHAL_HW_TIME_MILLISECONDS,
        pDataParams->wFieldRecoveryTime));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Wait(
                               phhalHw_SamAV2_DataParams_t * pDataParams,
                               uint8_t bUnit,
                               uint16_t wTimeout
                               )
{
    switch (pDataParams->bOpMode)
    {
        /* In case of Non-X mode, the Wait is directly redirected to the Reader IC */
    case PHHAL_HW_SAMAV2_OPMODE_NON_X:
        return phhalHw_Wait(pDataParams->pReaderHalDataParams, bUnit, wTimeout);
        /* Rc663 in X-Mode */
    case PHHAL_HW_SAMAV2_OPMODE_X_RC663:
        return phhalHw_SamAV2_Rc663_Wait(pDataParams, bUnit, wTimeout);
        /* Rc523 in X-Mode */
    default:
        return phhalHw_SamAV2_Rc523_Wait(pDataParams, bUnit, wTimeout);
    }
}

phStatus_t phhalHw_SamAV2_MfcAuthenticateKeyNo(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM abCmd[5];
    uint8_t     PH_MEMLOC_REM abCmdOut[9];
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wParity;
    uint16_t    PH_MEMLOC_REM wRxCrc;

    /* Parameter check */
    if ((wKeyNo > 0xFF) || (wKeyVersion > 0xFF))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Prepare first part of authenticate command */
    if (PHHAL_HW_MFC_KEYA == (bKeyType & 0x0F))
    {
        bKeyType = PHHAL_HW_MFC_KEYA;
    }
    else if (PHHAL_HW_MFC_KEYB == (bKeyType & 0x0F))
    {
        bKeyType = PHHAL_HW_MFC_KEYB;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* In case of X mode, we directly call MFC Authenticate */
    if (pDataParams->bOpMode != PHHAL_HW_SAMAV2_OPMODE_NON_X)
    {
        /* perform authentication */
        status = phhalHw_SamAV2_Cmd_MfAuthenticate(
            pDataParams,
            PHHAL_HW_SAMAV2_CMD_CHANGE_MFAUTHENTICATE_DEFAULT,
            pUid,
            (uint8_t)wKeyNo,
            (uint8_t)wKeyVersion,
            bKeyType,
            bBlockNo,
            0);

        /* Map invalid key stuff to invalid parameter */
        if (((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_KEY_REF_NO_INVALID) ||
            ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_KEY_KUC_NO_INVALID) ||
            ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_KEY_VERSION_INVALID))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        /* Check for error */
        else
        {
            if ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_MIFARE_GEN)
            {
                return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
            }
            else
            {
                PH_CHECK_SUCCESS(status);
            }
        }
    }
    /* Non-X mode */
    else
    {
        /* Build Authentication command */
        if (bKeyType == PHHAL_HW_MFC_KEYA)
        {
            abCmd[0] = PHHAL_HW_SAMAV2_AUTHMODE_KEYA;
        }
        else
        {
            abCmd[0] = PHHAL_HW_SAMAV2_AUTHMODE_KEYB;
        }
        abCmd[1] = bBlockNo;

        /* Retrieve RxCrc-setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXCRC, &wRxCrc));

        /* Disable RxCrc */
        if (wRxCrc != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));
        }

        /* Exchange AUTH1 command */
        status = phhalHw_SamAV2_Exchange(
            pDataParams,
            PH_EXCHANGE_DEFAULT | PHHAL_HW_SAMAV2_EXCHANGE_NO_DECIPHERING_BIT,
            abCmd,
            2,
            &pResponse,
            &wRxLength);

        /* Check status, allow incomplete byte return code */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            /* Bytelength must be 5 */
            if (wRxLength != 5)
            {
                return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
            }
        }
        else
        {
            PH_CHECK_SUCCESS(status);

            /* Bytelength must be 4 */
            if (wRxLength != 4)
            {
                return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
            }

            /* Force last byte to zero */
            abCmd[4] = 0x00;
        }

        /* Copy response */
        memcpy(abCmd, pResponse, wRxLength);  /* PRQA S 3200 */

        /* Process authentication part 1 */
        status = phhalHw_SamAV2_Cmd_SAM_AuthenticateMifare_Part1(
            pDataParams,
            PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MIFARE_DEFAULT,
            pUid,
            (uint8_t)wKeyNo,
            (uint8_t)wKeyVersion,
            bKeyType,
            bBlockNo,
            abCmd,
            0x00,
            abCmdOut);

        /* Return code should be chaining */
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
            }
            else
            {
                /* Map invalid key stuff to invalid parameter */
                if (((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_KEY_REF_NO_INVALID) ||
                    ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_KEY_KUC_NO_INVALID) ||
                    ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_KEY_VERSION_INVALID))
                {
                    return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
                }
                else
                {
                    return status;
                }
            }
        }

        /* Disable TxCrc */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));

        /* Retrieve Parity-setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_PARITY, &wParity));

        /* Disable Parity */
        if (wParity != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_PARITY, PH_OFF));
        }

        /* Exchange AUTH2 command */
        status = phhalHw_SamAV2_Exchange(
            pDataParams,
            PH_EXCHANGE_DEFAULT | PHHAL_HW_SAMAV2_EXCHANGE_NO_ENCIPHERING_BIT | PHHAL_HW_SAMAV2_EXCHANGE_NO_DECIPHERING_BIT,
            abCmdOut,
            9,
            &pResponse,
            &wRxLength);

        /* Restore Parity-setting again since many PAL layers expect it */
        if (wParity != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_PARITY, PH_ON));
        }

        /* Restore RxCrc */
        if (wRxCrc != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pReaderHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
        }

        /* Exchange error */
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            /* finish SAM chaining with KillAuthenticate command */
            /* Kill only card Auth */
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams, 0x01);

            /* return error */
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
            }
            else
            {
                return status;
            }
        }

        /* Process authentication part 2 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_AuthenticateMifare_Part2(pDataParams, pResponse));
    }

    /* MIAFRE Crypto is now enabled */
    pDataParams->bMifareCryptoDisabled = PH_OFF;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_MfcAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
    )
{
    /* satisfy compiler */
    if (pDataParams || bBlockNo || bKeyType || pKey || pUid);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_UpdateLc(
                                   phhalHw_SamAV2_DataParams_t * pDataParams
                                   )
{
    uint8_t * PH_MEMLOC_REM pTmpBuffer;

    /* Check for internal error */
    if (pDataParams->wTxBufLen_Cmd < (PHHAL_HW_SAMAV2_ISO7816_LC_POS+1))
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    /* TxBuffer equals RxBuffer */
    if (pDataParams->pTxBuffer == pDataParams->pRxBuffer)
    {
        /* Start at TxLength if neccessary */
        if ((pDataParams->wTxBufStartPos + pDataParams->wTxBufLen) >
            (pDataParams->wRxBufLen))
        {
            pTmpBuffer = &pDataParams->pTxBuffer[pDataParams->wTxBufStartPos + pDataParams->wTxBufLen];
        }
        /* Start at RxLength if neccessary */
        else
        {
            pTmpBuffer = &pDataParams->pTxBuffer[pDataParams->wRxBufLen];
        }
    }
    /* Buffers are different */
    else
    {
        pTmpBuffer = &pDataParams->pTxBuffer[pDataParams->wTxBufLen];
    }

    /* Perform actual update */
    pTmpBuffer[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = (uint8_t)(pDataParams->wTxBufLen_Cmd - PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_UpdateP2( 
                                   phhalHw_SamAV2_DataParams_t * pDataParams,
                                   uint8_t bP2
                                   )
{
    uint8_t * PH_MEMLOC_REM pTmpBuffer;

    /* Check for internal error */
    if (pDataParams->wTxBufLen_Cmd < (PHHAL_HW_SAMAV2_ISO7816_P2_POS+1))
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    /* TxBuffer equals RxBuffer */
    if (pDataParams->pTxBuffer == pDataParams->pRxBuffer)
    {
        /* Start at TxLength if neccessary */
        if ((pDataParams->wTxBufStartPos + pDataParams->wTxBufLen) >
            (pDataParams->wRxBufLen))
        {
            pTmpBuffer = &pDataParams->pTxBuffer[pDataParams->wTxBufStartPos + pDataParams->wTxBufLen];
        }
        /* Start at RxLength if neccessary */
        else
        {
            pTmpBuffer = &pDataParams->pTxBuffer[pDataParams->wRxBufLen];
        }
    }
    /* Buffers are different */
    else
    {
        pTmpBuffer = &pDataParams->pTxBuffer[pDataParams->wTxBufLen];
    }

    /* Perform actual update */
    pTmpBuffer[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bP2;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
