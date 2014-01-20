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
* Secure Messaging Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.7 $
* $Date: Fri Apr 15 09:26:33 2011 $
*
* History:
*  CHu: Generated 27. July 2009
*
*/

#include <ph_Status.h>
#include <phTools.h>
#include <phKeyStore.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_SAMAV2

#include <phhalHw_SamAV2_Cmd.h>
#include "phhalHw_SamAV2_Hc_AV1.h"
#include "phhalHw_SamAV2_Hc_AV1_Int.h"
#include "../phhalHw_SamAV2.h"
#include "../phhalHw_SamAV2_Int.h"
#include "../phhalHw_SamAV2_HcUtils.h"

/*
* Private constants
*/
static const uint8_t PH_MEMLOC_CONST_ROM phhalHw_SamAV2_Hc_AV1_FirstIv[PH_CRYPTOSYM_AES_BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

phStatus_t phhalHw_SamAV2_Hc_AV1_AuthenticateHost(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bAuthType,
    uint16_t wRdKeyRefNum,
    uint16_t wRdKeyRefVer,
    uint8_t bSamKeyRefNum,
    uint8_t bSamKeyRefVer,
    uint8_t *pDivInput,
    uint8_t bLenDivInput
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[7];
    uint8_t     PH_MEMLOC_BUF aKey[32];
    uint8_t     PH_MEMLOC_BUF bRndB[16];
    uint8_t     PH_MEMLOC_BUF bRndA[16];
    uint8_t     PH_MEMLOC_BUF bRndAB[32];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint8_t     PH_MEMLOC_REM bKeyLength;
    uint8_t     PH_MEMLOC_REM bCrcLength;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bRndBLength;
    uint8_t     PH_MEMLOC_REM bBlockLength;
    uint8_t     PH_MEMLOC_REM aDefaultLe[1];
    uint8_t     PH_MEMLOC_REM bKeepIV;
    uint8_t     PH_MEMLOC_REM b2K3DESOption;
    uint16_t    PH_MEMLOC_REM wOption;

    /* Check if keep IV flag is set for the selected SAM key entry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams,
        bSamKeyRefNum,
        aKey,
        &bBlockLength));

    if (bBlockLength != 11 && bBlockLength != 12)
    {
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_HAL);
    }

    /* Update the keep IV flag in the local crypto object */
    if (aKey[bBlockLength-2] & 0x04)
    {
        bKeepIV = PH_CRYPTOSYM_VALUE_KEEP_IV_ON;
    }
    else
    {
        bKeepIV = PH_CRYPTOSYM_VALUE_KEEP_IV_OFF;
    }

    /* determine the DES key option for all 2Key3DES keys */
    switch ((uint8_t) ((aKey[bBlockLength-2] & PH_SECMSG_HC_SAMAV1_INT_KEYTYPE_MASK) >> 3))
    {
    case PH_SECMSG_HC_SAMAV1_INT_3DESDF4_MASK:
        b2K3DESOption = PHHAL_HW_SAMAV2_DES_OPTION_DESFIRE4;
        break;
    case PH_SECMSG_HC_SAMAV1_INT_2K3DES_MASK:
        b2K3DESOption = PHHAL_HW_SAMAV2_DES_OPTION_ISO_CRC16;
        break;
    case PH_SECMSG_HC_SAMAV1_INT_2K3DESDF8_MASK:
        b2K3DESOption = PHHAL_HW_SAMAV2_DES_OPTION_ISO_CRC32;
        break;
    default:
        b2K3DESOption = 0;
        break;
    }

    /* Prepare default LE buffer */
    aDefaultLe[0] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wRdKeyRefNum,
        wRdKeyRefVer,
        sizeof(aKey),
        aKey,
        &wKeyType));

    /* The second half of the single DES key shall be filled for diversification */
    if (wKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        memcpy(&aKey[8], &aKey[0], 8);  /* PRQA S 3200 */
    }

    /* Retrieve key length parameters */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_GetKeyLengthParams(
        wKeyType,
        b2K3DESOption,
        &bKeyLength,
        &bRndBLength,
        &bBlockLength,
        &bCrcLength));

    /* in case of DES or 3KDES key, we have to encode the KeyVersion in the SessionKey */
    if ((wKeyType == PH_KEYSTORE_KEY_TYPE_3K3DES) || (wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES) || (wKeyType == PH_KEYSTORE_KEY_TYPE_DES))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_EncodeVersionInDESKey(aKey, bSamKeyRefVer, 2, aKey));
    }

    /* Build up the command */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]  = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]  = PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_HOST_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]   = bAuthType;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]   = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]   = 2 + bLenDivInput;    
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1] = bSamKeyRefNum;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2] = bSamKeyRefVer;

    /* Exchange first part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        7,
        NULL,
        NULL));

    /* Append diversification input */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bLenDivInput,
        NULL,
        NULL));

    /* Append LE and execute the command */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* We expect chaining active state */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        /* Now only success is allowed */
        PH_CHECK_SUCCESS(status);

        /* Set DES key option */
        pDataParams->b2K3DESEncOption = b2K3DESOption;

        if ((bAuthType & 0x04) == 0x00)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
        }

        if (wResponseLength != 0x00)
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }
    /* Success case */
    else
    {
        /* received data must be RndB */
        if (wResponseLength != bRndBLength)
        {
            /* finish SAM chaining with KillAuthenticate command */
            /* Kill only card Auth */
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(pDataParams, 0x01);
            return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_HAL);
        }
    }

    /* Check the P1 byte if the secret key should be diversified*/
    if (bAuthType & 0x01)
    {
        /* MIFARE Plus Diversification */
        if (bAuthType & 0x10)
        {
            wOption = PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS;
        }
        /* DESFire Diversification */
        else
        {
            wOption = PH_CRYPTOSYM_DIV_MODE_DESFIRE;

            if (bAuthType & 0x08)
            {
                wOption |= PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF;
            }
            else
            {
                wOption |= PH_CRYPTOSYM_DIV_OPTION_2K3DES_FULL;
            }
        }

        /* Perform diversification */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
            pDataParams->pENCCryptoDataParams,
            wOption,
            aKey,
            wKeyType,
            pDivInput,
            bLenDivInput,
            aKey));
    }

    /* Load session key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pENCCryptoDataParams,
        aKey,
        wKeyType));     

    /* reset the IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        phhalHw_SamAV2_Hc_AV1_FirstIv,
        bBlockLength));

    /* Store Authentication type */
    pDataParams->bAuthType = bAuthType;

    /* if secret key is used*/
    /* the Authentication process is now finished, the Session key is the key we have or not diversified*/
    if ((bAuthType & 0x04) != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /*   set the correct options */
    if (bKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON)
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_FIRST;
    }
    else
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC;
    }

    /* Decrypt RndB */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams,
        wOption,
        pResponse,
        bRndBLength,
        bRndB));

    /* Initialize the RndA array */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndBLength, bRndA));

    /* Prepare RndA || RndB' buffer */
    memcpy(bRndAB, bRndA, (uint16_t)bRndBLength);  /* PRQA S 3200 */
    memcpy(&bRndAB[bRndBLength], &bRndB[1], (uint16_t)(bRndBLength - 1));  /* PRQA S 3200 */
    bRndAB[(bRndBLength << 1) - 1] = bRndB[0];

    /*   set the correct options */
    if (bKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON)
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT;
    }
    else
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC;
    }

    /* Encrypt RndAB  buffer*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pENCCryptoDataParams, 
        wOption, 
        bRndAB, 
        (bRndBLength << 1), 
        bRndAB));

    /* Format AuthHost Part2 data bytes */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = (uint8_t)(bRndBLength << 1);

    /* Exchange first part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Append ENC(RndA || RndB') */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bRndAB,
        (bRndBLength << 1),
        NULL,
        NULL));

    /* Append LE and execute the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    /* received data must be ENC(RndA') */
    if (wResponseLength != bRndBLength)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /*   set the correct options */
    if (bKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON)
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT;
    }
    else
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC;
    }

    /* decrypt the Response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams,
        wOption,
        pResponse,
        wResponseLength,
        &bRndAB[1]));

    /* shift RndA' */
    bRndAB[0] = bRndAB[bRndBLength];

    /* compare RndA */
    if (memcmp(bRndAB, bRndA, bRndBLength) != 0)
    {
        /* the response for RndA is not equal to sent RndA, Authentication failed PH_ERR_AUTH_ERROR */
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
    }

    /* Host Authentication is successfully completed */
    /* Generate the current SessionKey for this HostAuthentication*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_GenerateSessionKey(
        aKey,
        wKeyType,
        bRndA,
        bRndB,
        aKey));

    /* Load Session Key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(pDataParams->pENCCryptoDataParams, aKey, wKeyType));

    /* reset the IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        phhalHw_SamAV2_Hc_AV1_FirstIv,
        bBlockLength));

    /* Set DES key option */
    pDataParams->b2K3DESEncOption = b2K3DESOption;

    if (bKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON)
    {
        /* Set the correct override IV flag */
        /* Set override Keep IV Flag */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_SetConfig(
            pDataParams->pENCCryptoDataParams, 
            PH_CRYPTOSYM_CONFIG_KEEP_IV, 
            bKeepIV));
    }

    /* Check for current Auth Type (CMAC auth or not) */
    if ((bAuthType) & 0x02)
    {
        /* Load Session key for CMAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pMACCryptoDataParams, 
            aKey, 
            wKeyType));

        /* Set override Keep IV Flag */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_SetConfig(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CONFIG_KEEP_IV, 
            bKeepIV));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_Encrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBuffer,
    uint16_t wLengthIn,
    uint16_t wBufferSize,
    uint16_t * pLengthOut
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wCrc16;
    uint32_t    PH_MEMLOC_REM dwCrc32;
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bKeyLength;
    uint8_t     PH_MEMLOC_REM bRndBLength;
    uint8_t     PH_MEMLOC_REM bBlockLength;
    uint8_t     PH_MEMLOC_REM bCrcLength;
    uint8_t *   PH_MEMLOC_REM pCrcInput;
    uint16_t    PH_MEMLOC_REM wCrcInputLen;
    uint8_t     PH_MEMLOC_REM bPaddingLength;
    uint16_t    PH_MEMLOC_REM wKeepIV;
    uint16_t    PH_MEMLOC_REM wOption;

    /* Set output length */
    *pLengthOut = wLengthIn;

    /* Retrieve information about ENC key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
        pDataParams->pENCCryptoDataParams,
        PH_CRYPTOSYM_CONFIG_KEY_TYPE,
        &wKeyType));

    /* Get the correct override IV flag */
    /* Get override Keep IV Flag */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
        pDataParams->pENCCryptoDataParams,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        &wKeepIV));

    /* If change entry plain is enabled a CRC16 has to be calculated. */
    if (pDataParams->bChangeEntryPlain)
    {
        bCrcLength = 2;
        bBlockLength = 8;
    }
    else
    {
        /* Retrieve key length parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_GetKeyLengthParams(
            wKeyType,
            pDataParams->b2K3DESEncOption,
            &bKeyLength,
            &bRndBLength,
            &bBlockLength,
            &bCrcLength));
    }

    /* Calculate number of padding bytes */
    bPaddingLength = (uint8_t)((*pLengthOut + bCrcLength - PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH) % bBlockLength);
    if (bPaddingLength > 0)
    {
        bPaddingLength = bBlockLength - bPaddingLength;
    }

    /* Update Lc */
    pBuffer[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = (uint8_t)(*pLengthOut + bCrcLength + bPaddingLength - PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH);

    /* CMAC Authentication: CRC is calculated on whole APDU, Lc need to be updated now */
    if (pDataParams->bMasterKeyCmacMode)
    {
        /* CRC is calculated over the whole stream */
        pCrcInput = pBuffer;
        wCrcInputLen = *pLengthOut;
    }
    else
    {
        /* CRC is calculated only over the data */
        pCrcInput = &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
        wCrcInputLen = *pLengthOut - PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH;
    }

    /* CRC16 */
    if (bCrcLength == 2)
    {
        /* Crc Calculation */
        PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
            PH_TOOLS_CRC_OPTION_DEFAULT,
            PH_TOOLS_CRC16_PRESET_ISO14443A,
            PH_TOOLS_CRC16_POLY_ISO14443,
            pCrcInput,
            wCrcInputLen,
            &wCrc16));

        /* Append CRC */
        pBuffer[(*pLengthOut)++] = (uint8_t)(wCrc16);
        pBuffer[(*pLengthOut)++] = (uint8_t)(wCrc16 >> 8);
    }
    /* CRC32 */
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
            PH_TOOLS_CRC_OPTION_DEFAULT,
            PH_TOOLS_CRC32_PRESET_DF8,
            PH_TOOLS_CRC32_POLY_DF8,
            pCrcInput,
            wCrcInputLen,
            &dwCrc32));

        /* Append CRC */
        pBuffer[(*pLengthOut)++] = (uint8_t)(dwCrc32);
        pBuffer[(*pLengthOut)++] = (uint8_t)(dwCrc32 >> 8);
        pBuffer[(*pLengthOut)++] = (uint8_t)(dwCrc32 >> 16);
        pBuffer[(*pLengthOut)++] = (uint8_t)(dwCrc32 >> 24);                
    }

    /* Apply Padding */
    if (wBufferSize < (*pLengthOut + bPaddingLength))
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
    }
    memset(&pBuffer[*pLengthOut], 0x00, bPaddingLength);  /* PRQA S 3200 */
    *pLengthOut = *pLengthOut + bPaddingLength;

    /*   set the correct options */
    if (wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON)
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT;
    }
    else
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC;
    }

    /* Encrypt the data stream */
    if (!(pDataParams->bChangeEntryPlain))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(pDataParams->pENCCryptoDataParams, 
            wOption,
            &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH],
            (uint16_t)(*pLengthOut - PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH),
            &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH]));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_Decrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBuffer,
    uint16_t wLengthIn,
    uint16_t * pLengthOut
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wCrc16Rec = 0;
    uint32_t    PH_MEMLOC_REM dwCrc32Rec = 0;
    uint16_t    PH_MEMLOC_REM wCrc16;
    uint32_t    PH_MEMLOC_REM dwCrc32;
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bKeyLength;
    uint8_t     PH_MEMLOC_REM bRndBLength;
    uint8_t     PH_MEMLOC_REM bBlockLength;
    uint8_t     PH_MEMLOC_REM bCrcLength;
    uint16_t    PH_MEMLOC_REM wKeepIV;
    uint16_t    PH_MEMLOC_REM wOption;

    /* Do not decrypt only SW1/SW2 */
    if (wLengthIn == PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH)
    {
        *pLengthOut = wLengthIn;
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Reset output length */
    *pLengthOut = 0;

    /* Retrieve information about ENC key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
        pDataParams->pENCCryptoDataParams,
        PH_CRYPTOSYM_CONFIG_KEY_TYPE,
        &wKeyType));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
        pDataParams->pENCCryptoDataParams,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        &wKeepIV));

    /* Retrieve key length parameters */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_GetKeyLengthParams(
        wKeyType,
        pDataParams->b2K3DESEncOption,
        &bKeyLength,
        &bRndBLength,
        &bBlockLength,
        &bCrcLength));

    /* Check for block length error */
    if (((wLengthIn - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH) % bBlockLength) != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /*   set the correct options */
    if (wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON)
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT;
    }
    else
    {
        wOption = PH_CRYPTOSYM_CIPHER_MODE_CBC;
    }

    /* Decrypt the response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(pDataParams->pENCCryptoDataParams, 
        wOption,
        pBuffer,
        (wLengthIn - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH),
        pBuffer));

    /*
    Since the only response which is encrypted is a MIAFARE key,
    we assume that we've got 6 bytes MIAFRE key + CRC + padding
    */

    /* Backup the received CRC */
    if (bCrcLength == 2)
    {
        wCrc16Rec = (uint16_t)((pBuffer[6]) | ((uint16_t)pBuffer[7] << 8));
    }
    else
    {
        dwCrc32Rec = (uint32_t)((pBuffer[6]) | ((uint32_t)pBuffer[7] << 8) | ((uint32_t)pBuffer[8] << 16) | ((uint32_t)pBuffer[9] << 24));
    }

    /* Append SW1/SW2 to the MIFARE key */
    memmove(  /* PRQA S 3200 */
        &pBuffer[6],
        &pBuffer[wLengthIn - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH],
        PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH);

    /* In CMAC auth. mode, the CRC is calculated over the whole frame */
    if (pDataParams->bMasterKeyCmacMode)
    {
        wLengthIn = 6 + PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH;
    }
    /* Otherwise the CRC is calculated only over the data */
    else
    {
        wLengthIn = 6;
    }

    /* CRC16 */
    if (bCrcLength == 2)
    {
        /* Crc Calculation */
        PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
            PH_TOOLS_CRC_OPTION_DEFAULT,
            PH_TOOLS_CRC16_PRESET_ISO14443A,
            PH_TOOLS_CRC16_POLY_ISO14443,
            pBuffer,
            wLengthIn,
            &wCrc16));

        /* Check CRC */
        if (wCrc16 != wCrc16Rec)
        {
            return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_HAL);
        }
    }
    /* CRC32 */
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
            PH_TOOLS_CRC_OPTION_DEFAULT,
            PH_TOOLS_CRC32_PRESET_DF8,
            PH_TOOLS_CRC32_POLY_DF8,
            pBuffer,
            wLengthIn,
            &dwCrc32));

        /* Check CRC */
        if (dwCrc32 != dwCrc32Rec)
        {
            return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_HAL);
        }            
    }

    /* Set response length */
    *pLengthOut = 6 + PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_CalculateMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBuffer,
    uint16_t wLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM pCalcMac[16];
    uint8_t     PH_MEMLOC_REM bCalcMacLen;
    uint16_t    PH_MEMLOC_REM wKeepIV;
    uint16_t    PH_MEMLOC_REM wOption;

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
        pDataParams->pMACCryptoDataParams,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        &wKeepIV));

    /*   set the correct options */
    wOption = PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST;

    /* compute the MAC to get the IV for response verification. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(pDataParams->pMACCryptoDataParams,
        wOption,
        pBuffer,
        wLength,
        pCalcMac, 
        &bCalcMacLen));

    /* update the IV. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(pDataParams->pMACCryptoDataParams,
        pCalcMac, 
        bCalcMacLen));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_VerifyRemoveMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBuffer,
    uint16_t wLengthIn,
    uint16_t * pLengthOut
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRecMac[8];
    uint8_t     PH_MEMLOC_REM pCalcMac[16];
    uint8_t     PH_MEMLOC_REM bCalcMacLen;
    uint16_t    PH_MEMLOC_REM wKeepIV;
    uint16_t    PH_MEMLOC_REM wBlockSize;
    uint16_t    PH_MEMLOC_REM wOption;

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(pDataParams->pMACCryptoDataParams, 
        PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, 
        &wBlockSize));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(pDataParams->pMACCryptoDataParams, 
        PH_CRYPTOSYM_CONFIG_KEEP_IV, 
        &wKeepIV));

    /* compute the MAC so that IV will be ready to verify the MAC in the response */

    /* If a MAC was expected but the SAM does not return one, reset the CMAC key. */
    if (wLengthIn < 10)
    {
        /* Enable Higher Security bit has been reset of CMAC key has been changed: reset IV and invalidate key for CMAC. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey(pDataParams->pMACCryptoDataParams));
        *pLengthOut = wLengthIn;
    }
    else
    {
        /* Extract the received MAC */
        memcpy(bRecMac, &pBuffer[wLengthIn - 8 - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH], 8);  /* PRQA S 3200 */

        /* Move SW1 SW2 to end of data */
        *pLengthOut = wLengthIn - 8;
        memmove(  /* PRQA S 3200 */
            &pBuffer[*pLengthOut - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH],
            &pBuffer[wLengthIn - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH],
            PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH);

        /*   set the correct options */
        wOption = PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST;

        /* Calculate MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(pDataParams->pMACCryptoDataParams,
            wOption,
            pBuffer,
            *pLengthOut,
            pCalcMac, 
            &bCalcMacLen));

        if (wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_OFF)
        {
            /* Clear the IV if keep IV is not set */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(pDataParams->pMACCryptoDataParams,
                phhalHw_SamAV2_Hc_AV1_FirstIv,
                (uint8_t)wBlockSize));
        }
        else
        {
            /* update the IV. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(pDataParams->pMACCryptoDataParams,
                pCalcMac, 
                bCalcMacLen));
        }

        /* Compare MAC */
        if (memcmp(bRecMac, pCalcMac, 8) != 0)
        {
            return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_GenerateSessionKey(
    uint8_t * pKey,
    uint16_t wKeyType,
    uint8_t * pRndAData,
    uint8_t * pRndBData,
    uint8_t * pSessionKey
    )
{
    uint8_t keyLength;
    uint8_t twoKey = 0;
    uint8_t count = 0;

    switch (wKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_DES:
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        keyLength = 16;
        for (count = 0; count < keyLength / 2; count++)
        {
            if ((pKey[count] & 0xFE) != (pKey[count + keyLength / 2] & 0xFE))
            {
                twoKey = 1;
            }
        }
        memcpy(&pSessionKey[0],&pRndAData[0], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[4],&pRndBData[0], 4); /* PRQA S 3200 */
        if (twoKey == 1)
        {
            memcpy(&pSessionKey[8],&pRndAData[4], 4); /* PRQA S 3200 */
            memcpy(&pSessionKey[12],&pRndBData[4], 4); /* PRQA S 3200 */
        }
        else
        {
            memcpy(&pSessionKey[8],&pRndAData[0], 4); /* PRQA S 3200 */
            memcpy(&pSessionKey[12],&pRndBData[0], 4); /* PRQA S 3200 */
        }
        break;

    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        keyLength = 16;
        memcpy(&pSessionKey[0],&pRndAData[0], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[4],&pRndBData[0], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[8],&pRndAData[6], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[12],&pRndBData[6], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[16],&pRndAData[12], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[20],&pRndBData[12], 4); /* PRQA S 3200 */
        break;

    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        keyLength = 24;
        memcpy(&pSessionKey[0],&pRndAData[0], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[4],&pRndBData[0], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[8],&pRndAData[12], 4); /* PRQA S 3200 */
        memcpy(&pSessionKey[12],&pRndBData[12], 4); /* PRQA S 3200 */
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_EncodeVersionInDESKey(
    uint8_t * pKey,
    uint8_t KeyVersion,
    uint8_t round,
    uint8_t * pEncodedKey
    )
{
    uint8_t count;

    /* perform the encoding*/
    for (count = 0; count < (8 * round); count ++)
    {
        /*pEncodedKey[count] = pKey[count] & (uint8_t)0xFEU;*/
        pEncodedKey[count] = (uint8_t)(pKey[count] & 0xFEU) | ((KeyVersion >> (7 - (count % 8))) & 0x01);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_GetCmdProtection(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t aCmd,
    uint8_t bP1,
    uint8_t bP2,
    uint8_t bIsResponse,
    uint8_t * bEncrypted,
    uint8_t * bMaced
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wKeyType;

    if (!bIsResponse)
    {
        /* Encrypt the following commands */
        switch (aCmd)
        {
        case PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_INS:
        case PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_INS:
            *bEncrypted = 1;
            break;

        default:

            *bEncrypted = 0;
            break;
        }
    }
    /* No decryption on response */
    else
    {
        /* Decrypt only ChangeKeyMifare if dumped key is encrypted */
        if ((aCmd == PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_INS) && (bP2 & 0x01))
        {
            *bEncrypted = 1;
        }
        else
        {
            *bEncrypted = 0;
        }
    }

    /* Only append MAC if the MAC key is valid */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(pDataParams->pMACCryptoDataParams, PH_CRYPTOSYM_CONFIG_KEY_TYPE, &wKeyType));
    if ((wKeyType != PH_CRYPTOSYM_KEY_TYPE_INVALID) && pDataParams->bMasterKeyCmacMode)
    {
        switch (aCmd)
        {
        case PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_HOST_INS:
        case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_INS:
        case PHHAL_HW_SAMAV2_CMD_GET_CHALLENGE_INS:
        case PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_INS:
        case PHHAL_HW_SAMAV2_CMD_ISO_INTERNAL_AUTHENTICATE_INS:
            *bMaced = 0;
            break;

        case PHHAL_HW_SAMAV2_CMD_KILL_AUTHENTICATION_INS:
            if ((pDataParams->bAuthType & 0x02) && bIsResponse)
                *bMaced = 0;
            else
                *bMaced = 1;
            break;

        case PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_INS:
        case PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_INS:
            if (!bIsResponse)
                *bMaced = 0;
            else
                *bMaced = 1;
            break;

        case PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_INS:
            if ((bP2 & 0x01) && bIsResponse)
                *bMaced = 0;
            else
                *bMaced = 1;
            break;

        case PHHAL_HW_SAMAV2_CMD_DUMP_SESSIONKEY_INS:
            if ((bP1 & 0x01) && bIsResponse)
                *bMaced = 0;
            else
                *bMaced = 1;
            break;

        default:
            *bMaced = 1;
            break;
        }
    }
    else
    {
        *bMaced = 0;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_GetKeyLengthParams(
    uint16_t wKeyType,
    uint8_t b2K3DESOption,
    uint8_t * pKeyLength,
    uint8_t * pRndBLength,
    uint8_t * pBlockLength,
    uint8_t * pCrcLength
    )
{
    /* Set the keyLength according the keyType*/
    switch (wKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_DES:
        *pKeyLength = 16;
        *pRndBLength = 8;
        *pBlockLength = 8;
        *pCrcLength = 2;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        *pKeyLength = 16;
        *pRndBLength = 8;
        *pBlockLength = 8;
        if (b2K3DESOption == PHHAL_HW_SAMAV2_DES_OPTION_DESFIRE4 || b2K3DESOption == PHHAL_HW_SAMAV2_DES_OPTION_ISO_CRC16)
        {
            *pCrcLength = 2;
        }
        else
        {
            *pCrcLength = 4;
        }
        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        *pKeyLength = 24;
        *pRndBLength = 16;
        *pBlockLength = 8;
        *pCrcLength = 4;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        *pKeyLength = 16;
        *pRndBLength = 16;
        *pBlockLength = 16;
        *pCrcLength = 4;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        *pKeyLength = 24;
        *pRndBLength = 16;
        *pBlockLength = 16;
        *pCrcLength = 4;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
