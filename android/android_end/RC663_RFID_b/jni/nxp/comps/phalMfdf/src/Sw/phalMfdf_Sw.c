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

#include <ph_Status.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <phKeyStore.h>
#include <ph_RefDefs.h>
#include <ph_TypeDefs.h>
#include <string.h>
#include <phTools.h>

#ifdef NXPBUILD__PHAL_MFDF_SW

#include "../phalMfdf_Int.h"
#include "phalMfdf_Sw.h"
#include "phalMfdf_Sw_Int.h"

phStatus_t phalMfdf_Sw_Init(
                            phalMfdf_Sw_DataParams_t * pDataParams,
                            uint16_t wSizeOfDataParams,
                            void * pPalMifareDataParams,
                            void * pKeyStoreDataParams,
                            void * pCryptoDataParamsEnc,
                            void * pCryptoRngDataParams,
                            void * pHalDataParams
                            )
{
    /* data param check */
    if (sizeof(phalMfdf_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalMifareDataParams);
    PH_ASSERT_NULL (pKeyStoreDataParams);
    PH_ASSERT_NULL (pCryptoDataParamsEnc);
    PH_ASSERT_NULL (pCryptoRngDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFDF | PHAL_MFDF_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;
    pDataParams->pCryptoDataParamsEnc   = pCryptoDataParamsEnc;
    pDataParams->pCryptoRngDataParams   = pCryptoRngDataParams;
    pDataParams->pHalDataParams  = pHalDataParams;
    /* 2 Byte CRC initial value in Authenticate mode. */
    pDataParams->wCrc = PH_TOOLS_CRC16_PRESET_ISO14443A;

    /* 4 Byte CRC initial value in 0x1A, 0xAA mode. */
    pDataParams->dwCrc = PH_TOOLS_CRC32_PRESET_DF8;

    memset(pDataParams->bSessionKey, 0x00, 24);  /* PRQA S 3200 */
    pDataParams->bKeyNo = 0xFF; /* Set to invalid */
    memset(pDataParams->bIv, 0x00, 16); /* PRQA S 3200 */
    memset(pDataParams->pAid, 0x00, 3);  /* PRQA S 3200 */
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED; /* Set to invalid */
    pDataParams->bWrappedMode = 0x00; /* Set to false */
    pDataParams->bCryptoMethod = 0xFF; /* No crypto just after init */
    pDataParams->wAdditionalInfo = 0x0000;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_Authenticate(
                                    phalMfdf_Sw_DataParams_t * pDataParams,
                                    uint16_t wOption,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVer,
                                    uint8_t bKeyNoCard,
                                    uint8_t * pDivInput,
                                    uint8_t bDivLen
                                    )
{
    uint8_t     PH_MEMLOC_REM bRndA[PH_CRYPTOSYM_DES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_DES_BLOCK_SIZE+1];
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_3K3DES_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint16_t    PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[40];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmdBuff[40];
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM bSessKeySize;
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bKeyNoCard > 0x0d)
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

    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        sizeof(bKey),
        bKey,
        &wKeyType));

    switch(wKeyType)
    {
    case PH_KEYSTORE_KEY_TYPE_DES:
        bRndLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 2 * PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        memcpy(&bKey[8], bKey, 8); /* PRQA S 3200 */
        wKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        break;

    case PH_KEYSTORE_KEY_TYPE_2K3DES:
        bRndLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 2 * PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    default:
        /* Wrong key type specified. Auth. will not work */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        /* Key is diversified and put back in bKey */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
            pDataParams->pCryptoDataParamsEnc,
            wOption,
            bKey,
            wKeyType,
            pDivInput,
            bDivLen,
            bKey
            ));  
    }

    /* load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bKey,
        wKeyType
        ));

    /* Send the cmd and receive the encrypted RndB */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_AUTHENTICATE;
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
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        else
        {
            return status;
        }
    }
    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* Store the unencrypted RndB */
    memcpy(bRndB, bWorkBuffer, bRndLen); /* PRQA S 3200 */
    
    /* Reset IV before start of a crypto operation */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Load Iv. Always zero for native mode */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Decrypt the RndB received from PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC,
        bRndB,
        bRndLen, 
        bRndB));

    /* Generate RndA */
    PH_CHECK_SUCCESS_FCT(statusTmp,phCryptoRng_Seed(pDataParams->pCryptoRngDataParams, bRndB, bRndLen));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndLen, bRndA));

    /* Concat RndA and RndB' */
    bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    memcpy(&bCmdBuff[1], bRndA, bRndLen); /* PRQA S 3200 */
    memcpy(&bCmdBuff[9], &bRndB[1], bRndLen - 1); /* PRQA S 3200 */
    bCmdBuff[16] = bRndB[0]; /* RndB left shifted by 8 bits */

    /* Load Iv. All zeroes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* DF4 Decrypt */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4 | PH_EXCHANGE_BUFFER_CONT,
        &bCmdBuff[1],
        2 * bRndLen,
        bWorkBuffer
        ));

    /* reset bIv to zero */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Copy the encrypted RndA + RndB' */
    memcpy(&bCmdBuff[1], bWorkBuffer, 2 * bRndLen); /* PRQA S 3200 */

    wCmdLen = 2 * bRndLen + 1;

    /* Get the encrypted RndA' into bWorkBuffer */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        ));

    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* bWorkBuffer now has the encrypted RndA */
    /* Decrypt the received RndA' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Decrypt RndA'*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC,
        bWorkBuffer,
        bRndLen,
        &bWorkBuffer[1]
    ));

    bWorkBuffer[0] = bWorkBuffer[8]; /* right shift to get back RndA */

    /* Compare RndA and buff */
    if (memcmp(bWorkBuffer, bRndA, bRndLen) != 0)
    {
        /* Authentication failed */
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDF);
    }

    /* Generate the session key */
    /* If key used for authentication is 2K3DES, Session key would be 16 bytes. */
    memcpy(pDataParams->bSessionKey, bRndA, 4); /* PRQA S 3200 */
    memcpy(&pDataParams->bSessionKey[4], bRndB, 4); /* PRQA S 3200 */
    pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_DES;

    if (memcmp(bKey, &bKey[PH_CRYPTOSYM_DES_KEY_SIZE], PH_CRYPTOSYM_DES_KEY_SIZE) == 0)
    {
        memcpy(&pDataParams->bSessionKey[8], bRndA, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[12], bRndB, 4); /* PRQA S 3200 */            
    }
    else 
    {
        memcpy(&pDataParams->bSessionKey[8], &bRndA[4], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[12], &bRndB[4], 4); /* PRQA S 3200 */
    }
    pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_2K3DES;

    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATE;
    pDataParams->bKeyNo = bKeyNoCard;

    return phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bSessionKey,
        pDataParams->bCryptoMethod
        );
}

phStatus_t phalMfdf_Sw_AuthenticateISO(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVer,
                                       uint8_t bKeyNoCard,
                                       uint8_t * pDivInput,
                                       uint8_t bDivLen
                                       )
{
    /**
    * The key type can be DES, 3DES, 3K3DES.
    * Random numbers can be 8 or 16 bytes long
    * Init vector can be 8 or 16 bytes long
    * Session key max size is 24 bytes if 3k3DES keys are used.
    *
    */

    uint8_t     PH_MEMLOC_REM bRndA[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE + 1];
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_3K3DES_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmdBuff[64];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[64];
    uint16_t    PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM bSessKeySize;
    uint8_t     PH_MEMLOC_REM bIvSize;
    uint8_t     PH_MEMLOC_REM bIv_bak[PH_CRYPTOSYM_DES_BLOCK_SIZE];

    /* Set the current authentication status to NOT AUTHENTICATED i.e., invalid key number */
    pDataParams->bKeyNo = 0xFF;

    if (bKeyNoCard > 0x0d)
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
    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        sizeof(bKey),
        bKey,
        &wKeyType
        ));

    switch(wKeyType)
    {
    case PH_KEYSTORE_KEY_TYPE_DES:
        bRndLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 2 * PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvSize = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        memcpy(&bKey[8], bKey, 8); /* PRQA S 3200 */
        wKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        break;

    case PH_KEYSTORE_KEY_TYPE_2K3DES:
        bRndLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = 2*PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bIvSize = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case PH_KEYSTORE_KEY_TYPE_3K3DES:
        bRndLen = 2*PH_CRYPTOSYM_DES_BLOCK_SIZE;
        bSessKeySize = PH_CRYPTOSYM_3K3DES_KEY_SIZE;
        bIvSize = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    default:
        /* Wrong key type specified. Auth. will not work */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        /* Key is diversified and put back in bKey */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
            pDataParams->pCryptoDataParamsEnc,
            wOption,
            bKey,
            wKeyType,
            pDivInput,
            bDivLen,
            bKey
            ));  
    }

    /* load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bKey,
        wKeyType));

    /* Initialize the init vector to all zeors */
    memset(bIv_bak, 0x00, bIvSize); /* PRQA S 3200 */

    /* Send the cmd and receive the encrypted RndB */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_AUTHENTICATE_ISO;
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
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        else
        {
            return status;
        }
    }
    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Store the unencrypted RndB */
    memcpy(bRndB, bWorkBuffer, bRndLen); /* PRQA S 3200 */

    /* Store the IV to be used for encryption later */
    memcpy(bIv_bak, &bRndB[bRndLen - bIvSize], bIvSize); /* PRQA S 3200 */

    /* Reset IV for the first crypto operation */
    memset(pDataParams->bIv, 0x00, bIvSize); /* PRQA S 3200 */

    /* Load Iv.*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvSize));

    /* Decrypt the RndB received from PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        bRndB,
        bRndLen,
        bRndB
        ));

    /* Generate RndA */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Seed(pDataParams->pCryptoRngDataParams, bRndB, bRndLen));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndLen, bRndA));

    /* Concat RndA and RndB' */
    bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    memcpy(&bCmdBuff[1], bRndA, bRndLen); /* PRQA S 3200 */
    memcpy(&bCmdBuff[bRndLen + 1], &bRndB[1], bRndLen - 1); /* PRQA S 3200 */
    bCmdBuff[2 * bRndLen] = bRndB[0]; /* RndB left shifted by 8 bits */

    memcpy(pDataParams->bIv, bIv_bak, bIvSize); /* PRQA S 3200 */

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvSize));

    /* Encrypt RndA + RndB' */
    PH_CHECK_SUCCESS_FCT(statusTmp,
        phCryptoSym_Encrypt(pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        &bCmdBuff[1],
        2 * bRndLen,
        &bCmdBuff[1]
    ));

    /* Update command length */
    wCmdLen = 2 * bRndLen + 1;

    /* Update Iv */
    memcpy(pDataParams->bIv, &bCmdBuff[wCmdLen - bIvSize], bIvSize); /* PRQA S 3200 */

    /* Get the encrypted RndA' into bWorkBuffer */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        ));
    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* bWorkBuffer now has the encrypted RndA' */
    /* Decrypt the received RndA' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvSize));

    /* Decrypt RndA'*/
    PH_CHECK_SUCCESS_FCT(statusTmp,
        phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        bWorkBuffer,
        bRndLen,
        &bCmdBuff[1]
    ));

    /* Using bCmdBuff for storage of decrypted RndA */
    bCmdBuff[0] = bCmdBuff[bRndLen]; /* right shift to get back RndA */

    /* Compare RndA and buff */
    if (memcmp(bCmdBuff, bRndA, bRndLen) != 0)
    {
        /* Authentication failed */
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDF);
    }

    /* Generate the session key */
    /*
    DES - 8 byte
    2K3DES - 16 bytes
    3K3DES - 24 bytes session key
    */
    memcpy(pDataParams->bSessionKey, bRndA, 4); /* PRQA S 3200 */
    memcpy(&pDataParams->bSessionKey[4], bRndB, 4); /* PRQA S 3200 */
    pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_DES;

    /*
    If first half of bKey is same as the second half it is a single
    DES Key.
    the session key generated is different.
    RndA 1st half + Rnd b 1st half + RndA1st half + RndB 1st half
    */

    if (wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES)
    {
        if (memcmp(bKey, &bKey[PH_CRYPTOSYM_DES_KEY_SIZE], PH_CRYPTOSYM_DES_KEY_SIZE) == 0)
        {
            memcpy(&pDataParams->bSessionKey[8], bRndA, 4); /* PRQA S 3200 */
            memcpy(&pDataParams->bSessionKey[12], bRndB, 4); /* PRQA S 3200 */
        }
        else 
        {
            memcpy(&pDataParams->bSessionKey[8], &bRndA[4], 4); /* PRQA S 3200 */
            memcpy(&pDataParams->bSessionKey[12], &bRndB[4], 4); /* PRQA S 3200 */
        }
        pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_2K3DES;
    }
    if (wKeyType == PH_KEYSTORE_KEY_TYPE_3K3DES)
    {
        memcpy(&pDataParams->bSessionKey[8], &bRndA[6], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[12], &bRndB[6], 4); /* PRQA S 3200 */

        memcpy(&pDataParams->bSessionKey[16], &bRndA[12], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[20], &bRndB[12], 4); /* PRQA S 3200 */
        pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_3K3DES;
    }

    /* Session key is generated */
    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
    pDataParams->bKeyNo = bKeyNoCard;

    /* IV is reset to zero as per the impl. hints document */
    memset(pDataParams->bIv, 0x00, sizeof(pDataParams->bIv)); /* PRQA S 3200 */

    /* Load the Session key which is valid for this authentication */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bSessionKey,
        pDataParams->bCryptoMethod
        ));

    /* Need to set the IV on */
    return phCryptoSym_SetConfig(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        PH_CRYPTOSYM_VALUE_KEEP_IV_ON);
}

phStatus_t phalMfdf_Sw_AuthenticateAES(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVer,
                                       uint8_t bKeyNoCard,
                                       uint8_t * pDivInput,
                                       uint8_t bDivLen
                                       )
{
    /**
    * The key type can be AES only.
    * Random numbers are 16 bytes long
    * Init vector is 16 bytes long
    * Session key size is 16 bytes.
    *
    */
    uint8_t     PH_MEMLOC_REM bRndA[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE+1];
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmdBuff[64];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[64];
    uint16_t    PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM bSessKeySize;
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint8_t     PH_MEMLOC_REM bIv_bak[PH_CRYPTOSYM_AES128_KEY_SIZE];

    /* Set the current authentication status to NOT AUTHENTICATED i.e., invalid key number */
    pDataParams->bKeyNo = 0xFF;

    if (bKeyNoCard > 0x0D)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_DESFIRE) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen > 31))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        PH_CRYPTOSYM_AES128_KEY_SIZE,
        bKey,
        &wKeyType
        ));

    /* Invalid key type at wKeyNo and wKeyVer */
    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        /* Key is diversified and put back in bKey */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
            pDataParams->pCryptoDataParamsEnc,
            wOption,
            bKey,
            wKeyType,
            pDivInput,
            bDivLen,
            bKey
            ));  
    }

    bRndLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    bSessKeySize = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;

    /* load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bKey,
        wKeyType));

    memset(bIv_bak, 0x00, bIvLen); /* PRQA S 3200 */

    /* Send the cmd and receive the encrypted RndB */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_AUTHENTICATE_AES;
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
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        else
        {
            return status;
        }
    }
    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Store the unencrypted RndB */
    memcpy(bRndB, bWorkBuffer, bRndLen); /* PRQA S 3200 */

    /* Store the IV to be used for encryption later */
    memcpy(bIv_bak, &bRndB[bRndLen - bIvLen], bIvLen); /* PRQA S 3200 */

    /* Reset IV for the first crypto operation */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Load Iv.*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Decrypt the RndB received from PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        bRndB,
        bRndLen,
        bRndB
        ));

    /* Generate RndA */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Seed(pDataParams->pCryptoRngDataParams, bRndB, bRndLen));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndLen, bRndA));

    /* Concat RndA and RndB' */
    bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    memcpy(&bCmdBuff[1], bRndA, bRndLen); /* PRQA S 3200 */
    memcpy(&bCmdBuff[bRndLen + 1], &bRndB[1], bRndLen - 1); /* PRQA S 3200 */
    bCmdBuff[2 * bRndLen] = bRndB[0]; /* RndB left shifted by 8 bits */

    memcpy(pDataParams->bIv, bIv_bak, bIvLen); /* PRQA S 3200 */

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Encrypt RndA + RndB' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        &bCmdBuff[1],
        2 * bRndLen,
        &bCmdBuff[1]
    ));

    wCmdLen = 2 * bRndLen + 1;

    /* Update Iv */
    memcpy(pDataParams->bIv, &bCmdBuff[wCmdLen - bIvLen], bIvLen); /* PRQA S 3200 */

    /* Get the encrypted RndA' into bWorkBuffer */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        ));
    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Decrypt the received RndA' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Decrypt RndA'*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        bWorkBuffer,
        bRndLen,
        &bCmdBuff[1]
    ));

    bCmdBuff[0] = bCmdBuff[bRndLen]; /* right shift to get back RndA */

    /* Compare RndA and buff */
    if (memcmp(bCmdBuff, bRndA, bRndLen) != 0)
    {
        /* Authentication failed */
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDF);
    }

    /* Generate the session key */
    memcpy(pDataParams->bSessionKey, bRndA, 4); /* PRQA S 3200 */
    memcpy(&pDataParams->bSessionKey[4], bRndB, 4); /* PRQA S 3200 */
    memcpy(&pDataParams->bSessionKey[8], &bRndA[12], 4); /* PRQA S 3200 */
    memcpy(&pDataParams->bSessionKey[12], &bRndB[12], 4); /* PRQA S 3200 */

    /* Session key is generated. IV is stored for further crypto operations */
    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES;
    pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_AES128;
    pDataParams->bKeyNo = bKeyNoCard;

    /* IV is reset to zero as per the impl. hints document */
    memset(pDataParams->bIv, 0x00, sizeof(pDataParams->bIv)); /* PRQA S 3200 */

    /* Load the session key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bSessionKey,
        pDataParams->bCryptoMethod
        ));

    /* Set the keep Iv ON */
    return phCryptoSym_SetConfig(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        PH_CRYPTOSYM_VALUE_KEEP_IV_ON
        );
}

phStatus_t phalMfdf_Sw_ChangeKeySettings(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bKeySettings
    )
{
    /**
    * This  function will handle all the three authentication modes: 0x0A, 1A and AA.
    * and all crypto modes i.e., DES, 3DES, 3K3DES, AES
    * The previous authentication status including key number and session key is
    * present in the params  structure.
    * Successful auth. with PICC master key is required if AID = 0x00 else
    * an auth. with the application master key is required.
    */
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CHANGE_KEY_SETTINGS;
    bCmdBuff[wCmdLen++] = bKeySettings;

    /* COMMUNICATION IS Encrypted */
    return phalMfdf_Sw_Int_Write_Enc(pDataParams,
        bCmdBuff,
        0x0001,
        PH_CRYPTOSYM_PADDING_MODE_1,
        &bCmdBuff[1],
        0x0001
        );
}

phStatus_t phalMfdf_Sw_GetKeySettings(
                                      phalMfdf_Sw_DataParams_t * pDataParams,
                                      uint8_t * pResponse
                                      )
{
    /**
    * This command can be issued without valid authentication
    */
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_KEY_SETTINGS;
    wCmdLen = 1;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 0x02)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_ChangeKey(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint16_t wOption,
                                 uint16_t wOldKeyNo,
                                 uint16_t wOldKeyVer,
                                 uint16_t wNewKeyNo,
                                 uint16_t wNewKeyVer,
                                 uint8_t bKeyNoCard,
                                 uint8_t * pDivInput,
                                 uint8_t bDivLen
                                 )
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[42];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[42];
    uint8_t     PH_MEMLOC_REM bOldKey[24];
    uint8_t     PH_MEMLOC_REM bNewKey[24];
    uint8_t     PH_MEMLOC_REM bNewKeyLen= 0;
    uint8_t     PH_MEMLOC_REM bIndex;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wOldKeyType;
    uint16_t    PH_MEMLOC_REM wNewKeyType;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bMacLen;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint32_t    PH_MEMLOC_REM dwCrc;
    uint16_t    PH_MEMLOC_REM wCrc;
    uint16_t    PH_MEMLOC_REM wTmp;
    uint16_t    PH_MEMLOC_REM wTmpOption = 0x0000;
    uint8_t     PH_MEMLOC_REM bAppId[3] = { 0x00, 0x00, 0x00 };

    if ((memcmp(pDataParams->pAid, bAppId, 3) == 0x00) && ((bKeyNoCard & 0x3FU) == 0x00)) /* Card Master key */
    {
        /* Only if seleted Aid is 0x000000, and card key number is X0, then 
        it is likely to be the PICC master key that has to be changed. */
        if ((bKeyNoCard != 0x80) && (bKeyNoCard != 0x40) && (bKeyNoCard != 0x00))
        {
            /* Invalid card key number supplied */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    else
    {
        if (bKeyNoCard > 0x0D)
        {
            /* Invalid application key specified */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    if ((wOption == 0x0000) || (bDivLen > 31))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
    }

    memset(bWorkBuffer, 0x00, 42); /* PRQA S 3200 */
    memset(bCmdBuff, 0x00, 42); /* PRQA S 3200 */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CHANGE_KEY;
    bCmdBuff[wCmdLen++] = bKeyNoCard;

    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wOldKeyNo,
        wOldKeyVer,
        sizeof(bOldKey),
        bOldKey,
        &wOldKeyType
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wNewKeyNo,
        wNewKeyVer,
        sizeof(bNewKey),
        bNewKey,
        &wNewKeyType
        ));

    if (wOldKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        wOldKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        memcpy(&bOldKey[8], bOldKey, 8); /* PRQA S 3200 */
    }
    if (wNewKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        wNewKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        memcpy(&bNewKey[8], bNewKey, 8); /* PRQA S 3200 */
    }
    /* 
    It is allowed to change a key type for PICC master key. 
    Old key may not be diversified but new key can be. 
    Old key may be diversified with one round but new key can
    be diversified with two rounds. 

    Key diversification method (DESFire or MFPlus) cannot be changed 
    between old and new key.

    It is assumed that the diversification input specified is the same
    for both old key and new key
    */

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        if (wOption & PHAL_MFDF_CHGKEY_DIV_NEW_KEY )
        {
            if (wOption & PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC)
            {
                wTmpOption = PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS;
            }
            else
            {
                wTmpOption = PH_CRYPTOSYM_DIV_MODE_DESFIRE;
                if (wOption & PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND)
                {
                    wTmpOption |= PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF;
                }
            }
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
                pDataParams->pCryptoDataParamsEnc,
                wTmpOption,
                bNewKey,
                wNewKeyType,
                pDivInput,
                bDivLen,
                bNewKey
                )); 
        }
        if (wOption & PHAL_MFDF_CHGKEY_DIV_OLD_KEY )
        {
            if (wOption & PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC)
            {
                wTmpOption |= PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS;
            }
            else
            {
                wTmpOption |= PH_CRYPTOSYM_DIV_MODE_DESFIRE;
                if (wOption & PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND)
                {
                    wTmpOption |= PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF;
                }
            }
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
                pDataParams->pCryptoDataParamsEnc,
                wTmpOption,
                bOldKey,
                wOldKeyType,
                pDivInput,
                bDivLen,
                bOldKey
                ));  
        }

        /* Reload the IV and key since the diversify function has invalidated the key */
        /* Load the Session key which is valid for this authentication */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bSessionKey,
            pDataParams->bCryptoMethod
            ));

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            bIvLen
            ));

        /* Need to check whether this is required for 0x0A mode also*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_SetConfig(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CONFIG_KEEP_IV,
            PH_CRYPTOSYM_VALUE_KEEP_IV_ON
            ));
    }

    switch(wNewKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        bNewKeyLen = PH_CRYPTOSYM_AES128_KEY_SIZE;
        break;

    case PH_CRYPTOSYM_KEY_TYPE_DES:
        bNewKeyLen = PH_CRYPTOSYM_DES_KEY_SIZE;
        break;

    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        bNewKeyLen = PH_CRYPTOSYM_2K3DES_KEY_SIZE;
        break;

    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        bNewKeyLen = PH_CRYPTOSYM_3K3DES_KEY_SIZE;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE)
    {
        /* bKeyNo for PICC masterkey will have key type in the two MS bits
        Should check for : bKeyNo & 3F != bKeyNoCard & 3F
        */
        if ( (pDataParams->bKeyNo & 0x3FU) != (bKeyNoCard & 0x3FU))
        {
            /* Get the key from the key store using wKeyNo and version */
            /* xored_Data = pNewKey ^ wKey */
            /* xored_Data + CRC16 (xord data) + CRC16(new key)+padding */
            /* Decrypt the above, Cmd+keyno+decrypted data is then sent to card */
            /* Get the key from the key store using wKeyNo and version */

            /* xored_Data = pNewKey ^ wKey */
            for(bIndex = 0; bIndex < bNewKeyLen; bIndex++)
            {
                bWorkBuffer[bIndex] = bOldKey[bIndex] ^ bNewKey[bIndex];
            }
            /*
            Key version is irrelevant here since in this case, we are
            changing a key different from the one authenticated with.
            In Native authentication mode only DES and 2K3DES are possible.
            AES is not possible and hence the key version that is only applicable
            to AES keys.
            */

            /* Copy the XORd data to the command buffer */
            memcpy(&bCmdBuff[2], bWorkBuffer, bIndex); /* PRQA S 3200 */
            wCmdLen = wCmdLen + bIndex;

            /* Calculate CRC16 over XORddata */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC16_PRESET_ISO14443A,
                PH_TOOLS_CRC16_POLY_ISO14443,
                bWorkBuffer,
                bIndex,
                &wCrc
                ));
            /* Append CRC16 of the XORd data */
            memcpy(&bCmdBuff[wCmdLen], &wCrc, 2); /* PRQA S 3200 */

            /* Update Cmd length */
            wCmdLen += 2;

            /* Calculate CRC for the new key */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC16_PRESET_ISO14443A,
                PH_TOOLS_CRC16_POLY_ISO14443,
                bNewKey,
                bNewKeyLen,
                &wCrc
                ));
            /* Add CRC16 */
            memcpy(&bCmdBuff[wCmdLen], &wCrc, 2); /* PRQA S 3200 */
            wCmdLen += 2; /* comm sett + access rights (2b) + 2B CRC */
        }
        else
        {
            memcpy(&bCmdBuff[wCmdLen], bNewKey, bNewKeyLen); /* PRQA S 3200 */
            wCmdLen = wCmdLen + bNewKeyLen;
            /* Also check if it is PICC master key and is an AES key. If so then
            key version also needs to be appended to the new key before calculating
            CRC16 */
            if (bKeyNoCard == 0x80)
            {
                /* PICC master key is being changed to AES key. Version is relevant */
                bCmdBuff[wCmdLen++] = (uint8_t)wNewKeyVer;
            }
            /* Calculate CRC for the new key and the key version if any */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC16_PRESET_ISO14443A,
                PH_TOOLS_CRC16_POLY_ISO14443,
                &bCmdBuff[2],
                wCmdLen-2,
                &wCrc
                ));
            /* Add CRC16 */
            memcpy(&bCmdBuff[wCmdLen], &wCrc, 2); /* PRQA S 3200 */
            wCmdLen += 2; /* comm sett + access rights (2b) + 2B CRC */
        }

        /* Apply padding */
        wTmp = wCmdLen - 2;
        if (wTmp % bIvLen)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
                PH_CRYPTOSYM_PADDING_MODE_1,
                &bCmdBuff[2],
                wTmp,
                bIvLen,
                sizeof(bWorkBuffer),
                bWorkBuffer,
                &wTmp
                ));
        }

        /* DF4 Decrypt */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4 | PH_EXCHANGE_BUFFER_CONT,
            bWorkBuffer,
            wTmp,
            bWorkBuffer
            ));
        /* form the complete command */
        memcpy(&bCmdBuff[2], bWorkBuffer, wTmp); /* PRQA S 3200 */

        /* Update Cmd len */
        wCmdLen = wTmp + 2;
    }
    else if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO))
    {
        if ((pDataParams->bKeyNo & 0x3FU) != (bKeyNoCard & 0x3FU))
        {
            /* xored_Data = pNewKey ^ wKey */
            for (bIndex = 0; bIndex < bNewKeyLen; bIndex++)
            {
                bWorkBuffer[bIndex] = bOldKey[bIndex] ^ bNewKey[bIndex];
            }
            /* xored_Data+ [AES key version] + CRC32 (all prev. data) + CRC32(new key)+padding */
            if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
            {
                bWorkBuffer[bIndex++] = (uint8_t)wNewKeyVer;
            }
            memcpy(&bCmdBuff[2], bWorkBuffer, bIndex); /* PRQA S 3200 */
            wCmdLen = wCmdLen + bIndex;

            /* Calculate CRC32 over cmd+header+XORddata+[keyversion] */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC32_PRESET_DF8,
                PH_TOOLS_CRC32_POLY_DF8,
                bCmdBuff,
                wCmdLen,
                &dwCrc
                ));

            memcpy(&bCmdBuff[wCmdLen], &dwCrc, 4); /* PRQA S 3200 */
            wCmdLen += 4;

            /* Calculate CRC32 over the new key  */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC32_PRESET_DF8,
                PH_TOOLS_CRC32_POLY_DF8,
                bNewKey,
                bNewKeyLen,
                &dwCrc
                ));
            memcpy(&bCmdBuff[wCmdLen], &dwCrc, 4); /* PRQA S 3200 */
            wCmdLen += 4;

            wTmp = wCmdLen - 2;
            if (wTmp % bIvLen)
            {
                /* Apply padding */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
                    PH_CRYPTOSYM_PADDING_MODE_1,
                    &bCmdBuff[2],
                    wTmp,
                    bIvLen,
                    sizeof(bCmdBuff)-2,
                    &bCmdBuff[2],
                    &wTmp
                    ));
            }

            /* Encrypt */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                &bCmdBuff[2],
                wTmp,
                &bCmdBuff[2]
            ));
            wCmdLen = wTmp + 2;
        }
        else
        {
            memcpy(&bCmdBuff[wCmdLen], bNewKey, bNewKeyLen); /* PRQA S 3200 */
            wCmdLen = wCmdLen + bNewKeyLen;

            /* Also check if it is PICC master key and is an AES key is to be written.
            If so then key version also needs to be appended to the new key before
            calculating CRC */
            if (memcmp(pDataParams->pAid, bAppId, 3) == 0x00)
            {
                if (bKeyNoCard == 0x80)
                {
                    /* PICC master key is being changed to AES key. Version is relevant */
                    bCmdBuff[wCmdLen++] = (uint8_t)wNewKeyVer;
                }
            }
            else 
            {
                if (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES)
                {
                    /* Implies that AES key is to be written. Version becomes relevant */
                    bCmdBuff[wCmdLen++] = (uint8_t)wNewKeyVer;
                }
            }

            /*  Calculate CRC32 on the Key data */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC32_PRESET_DF8,
                PH_TOOLS_CRC32_POLY_DF8,
                bCmdBuff,
                wCmdLen,
                &dwCrc
                ));

            /* Add CRC32 */
            memcpy(&bCmdBuff[wCmdLen], &dwCrc, 4); /* PRQA S 3200 */
            wCmdLen += 4; /* comm sett + access rights (2b) + 4B CRC */

            wTmp = wCmdLen-2;
            /* Apply padding */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
                PH_CRYPTOSYM_PADDING_MODE_1,
                &bCmdBuff[2],
                wTmp,
                bIvLen,
                sizeof(bCmdBuff) - 2,
                &bCmdBuff[2],
                &wTmp
                ));

            /* Encrypt */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                &bCmdBuff[2],
                wTmp,
                &bCmdBuff[2]
            ));
            /* Update Cmd len */
            wCmdLen = wTmp + 2;
        }

        /* Update IV */
        memcpy(pDataParams->bIv, &bCmdBuff[wCmdLen - bIvLen], bIvLen); /* PRQA S 3200 */
    }
    else
    {
        /* ERROR: NOT_AUTHENTICATED */
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDF);
    }

    /* Send the command */
    statusTmp = phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        );
    if (statusTmp != PH_ERR_SUCCESS)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
        }
        return statusTmp;
    }

    /* Verify the MAC */
    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
    {
        /* If Master key of the currently selected app is changed, the authentication
        is invalidated. So no CMAC is returned */
        if (wRxlen > 0 )
        {
            /* copy CMAC received from card */
            memcpy(bCMacCard, &bWorkBuffer[wRxlen - 8], 8); /* PRQA S 3200 */
            wRxlen -= 8;

            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsEnc,
                pDataParams->bIv,
                bIvLen
                ));

            bWorkBuffer[wRxlen] = 0x00; /* Status ok */

            /* Calculate CMAC */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsEnc,
                (PH_CRYPTOSYM_MAC_MODE_CMAC),
                bWorkBuffer,
                wRxlen + 1,
                bCMAC,
                &bMacLen
                ));

            if (memcmp(bCMAC, bCMacCard, 8) != 0)
            {
                /* CMAC validation failed */
                return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFDF);
            }

            /* Update IV to be used for next commands */
            memcpy(pDataParams->bIv, bCMAC, bIvLen); /* PRQA S 3200 */
        }

        /* Reset authentication status only if the key authenticated with 
         *  is changed.
         */
        if ((pDataParams->bKeyNo & 0x3FU) == (bKeyNoCard & 0x3FU))
        {
            phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
        }
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_GetKeyVersion(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t bKeyNo,
                                     uint8_t * pResponse
                                     )
{
    /**
    * This command can be issued without valid authentication
    */
    uint16_t  PH_MEMLOC_REM statusTmp;
    uint8_t PH_MEMLOC_REM bCmdBuff[20];
    uint16_t PH_MEMLOC_REM wRxlen;
    uint16_t PH_MEMLOC_REM wCmdLen = 0;
    uint8_t PH_MEMLOC_REM *pRecv;

    if (bKeyNo > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_KEY_VERSION;
    bCmdBuff[wCmdLen++] = bKeyNo;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 0x01)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

/**
* PICC level commands
*/
phStatus_t phalMfdf_Sw_CreateApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pAid,
    uint8_t bKeySettings1,
    uint8_t bKeySettings2,
    uint8_t * pISOFileId,
    uint8_t * pISODFName,
    uint8_t bISODFNameLen
    )
{
    /*
    If (bKeySettings2 & 0x03)== 00 [DES, 3DES] then pDataParams->bAuthMode can be either
    0x0A or 0x1A.
    If (bKeySettings2 & 0x03)== 01 [3K3DES] then pDataParams->bAuthMode can only be 0x1A.
    If (bKeySettings2 & 0x03)== 10 [AES] then pDataParams->bAuthMode can only be 0xAA.
    */
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bISODFNameLen > 16) || (bOption > 0x03) || (bOption == 0x02))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_APPLN;

    memcpy(&bCmdBuff[wCmdLen], pAid, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    bCmdBuff[wCmdLen++] = bKeySettings1;
    bCmdBuff[wCmdLen++] = bKeySettings2;

    if (bOption & 0x01)
    {
        /* wISOFileId is present */
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }
    if (bOption & 0x02)
    {
        /* pISODFName is present */
        memcpy(&bCmdBuff[wCmdLen], pISODFName, bISODFNameLen); /* PRQA S 3200 */
        wCmdLen = wCmdLen + bISODFNameLen;
    }

    return phalMfdf_Sw_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_DeleteApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pAppId
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[10];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_DELETE_APPLN;
    memcpy(&bCmdBuff[1], pAppId, PHAL_MFDF_DFAPPID_SIZE); /* PRQA S 3200 */
    wCmdLen += PHAL_MFDF_DFAPPID_SIZE;

    return phalMfdf_Sw_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_GetApplicationIDs(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pResponse,
    uint8_t * pNumAIDs
    )
{
    /**
    A PICC can store upto 28 applications. PICC will return all
    AIDs (3 byte/aid) in single response if the number of applications <= 20
    else, it will send the  remaining AIDs in the second transmission. The
    first response sent by PICC will have 0xAF in the status byte.
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* Initialization */
    *pNumAIDs = 0;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_APPLN_IDS;
    wCmdLen = 1;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pNumAids and return  */
    *pNumAIDs = (uint8_t)(wRxlen / 3);

    /* Length should be a multiple of 3. Else return error */
    if (wRxlen % 3)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_GetDFNames(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t  bOption,
                                  uint8_t * pDFBuffer,
                                  uint8_t * pDFInfoLen
                                  )
{
    /**
    Returns AID(3B), FID(2B), DF-Name(1..16B) in one frame.

    Will return PH_EXCHANGE_RXCHAINING if more DF names are present.
    The caller has to call the function with option PH_EXCHANGE_RXCHAINING

    Will not work if authenticated in standard TDES or AES modes as per the
    Implementation Hints document.
    */
    uint16_t    PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[24];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen;

    /* form the command */
    if (bOption == PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if (bOption == PH_EXCHANGE_DEFAULT)
    {
        bCmdBuff[0] = PHAL_MFDF_CMD_GET_DF_NAMES;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    wCmdLen = 1;

    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
    {
        /* Should return, invalid scenario error. Card will be disabled 
        in case this command is sent in these modes */
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
    }

    /* Send the command */
    status = phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,                          
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        );

    /* check for protocol errors */
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) &&
        (wRxlen == 0))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* Should return either zero bytes or more than 4 bytes. Anything inbetween
      is an error */
    if ((status == PH_ERR_SUCCESS) && (wRxlen != 0) && (wRxlen < 5))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        && (wRxlen < 5))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) || (status == PH_ERR_SUCCESS))
    {
        memcpy(pDFBuffer, bWorkBuffer, wRxlen); /* PRQA S 3200 */
        *pDFInfoLen = (uint8_t)wRxlen;
    }
    return status;
}

phStatus_t phalMfdf_Sw_SelectApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pAppId
    )
{
    uint16_t  PH_MEMLOC_REM statusTmp;
    uint8_t   PH_MEMLOC_REM bCmdBuff[10];
    uint8_t   PH_MEMLOC_REM bWorkBuffer[8];
    uint16_t  PH_MEMLOC_REM wRxlen;
    uint16_t  PH_MEMLOC_REM wCmdLen;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_SELECT_APPLN;
    memcpy(&bCmdBuff[1], pAppId, PHAL_MFDF_DFAPPID_SIZE); /* PRQA S 3200 */
    wCmdLen = PHAL_MFDF_DFAPPID_SIZE + 1;

    /* Reset Authentication Status here */
    phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);

    /* Send the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,                                        
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        ));

    /* Store the currently selected application Id */
    memcpy(pDataParams->pAid, pAppId, 3);  /* PRQA S 3200 */

    /* Select command never returns CMAC */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_FormatPICC(
                                  phalMfdf_Sw_DataParams_t * pDataParams
                                  )
{
    uint8_t PH_MEMLOC_REM bCmdBuff[8];

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_FORMAT_PICC;
    return phalMfdf_Sw_Int_Write_Plain(pDataParams, bCmdBuff, 0x0001, PHAL_MFDF_COMMUNICATION_PLAIN, NULL, 0x0000);
}

phStatus_t phalMfdf_Sw_GetVersion(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t * pResponse
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_VERSION;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    if (wRxlen != 28)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_FreeMem(
                               phalMfdf_Sw_DataParams_t * pDataParams,
                               uint8_t * pResponse
                               )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_FREE_MEM;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    if (wRxlen != 3)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_SetConfiguration(
                                        phalMfdf_Sw_DataParams_t * pDataParams,
                                        uint8_t bOption,
                                        uint8_t * pData,
                                        uint8_t bDataLen
                                        )
{
    uint8_t  PH_MEMLOC_REM bCmdBuff[8];
    uint16_t PH_MEMLOC_REM wCmdLen = 0;
    uint8_t  PH_MEMLOC_REM bPaddingMethod = PH_CRYPTOSYM_PADDING_MODE_1;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_SET_CONFIG;
    bCmdBuff[wCmdLen++] = bOption;
    switch(bOption)
    {
    case PHAL_MFDF_SET_CONFIG_OPTION1:
        /* Data = 1B configuration data */
        bPaddingMethod = PH_CRYPTOSYM_PADDING_MODE_1;
        break;

    case PHAL_MFDF_SET_CONFIG_OPTION2:
        /* Data =  KEY || 1BYTE KEY VERSION    Key data is 25 bytes */
        if (bDataLen != 25)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bPaddingMethod = PH_CRYPTOSYM_PADDING_MODE_1;
        break;

    case PHAL_MFDF_SET_CONFIG_OPTION3:
        /* User defined ATS */
        if (bDataLen > 20)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bPaddingMethod = PH_CRYPTOSYM_PADDING_MODE_2;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    return phalMfdf_Sw_Int_Write_Enc(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        bPaddingMethod,
        pData,
        (uint16_t)bDataLen
        );
}

phStatus_t phalMfdf_Sw_GetCardUID(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t * pResponse
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_CARD_UID;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Enc(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_ENC,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    if (wRxlen != 7)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_GetFileIDs(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t * pResponse,
                                  uint8_t * bNumFIDs
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM * pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_FILE_IDS;
    wCmdLen = 1;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    *bNumFIDs = (uint8_t)wRxlen;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_GetISOFileIDs(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t * pResponse,
                                     uint8_t * pNumFIDs
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen;
    uint8_t     PH_MEMLOC_REM * pRecv;

    /* Initialization */
    *pNumFIDs = 0;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_ISO_FILE_IDS;
    wCmdLen = 1;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    /* Length should be multiple of 2 */
    if ((wRxlen != 0) && (wRxlen % 2))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pNumAids and return  */
    *pNumFIDs = (uint8_t)(wRxlen / 2);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_GetFileSettings(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bFileNo,
                                       uint8_t * pResponse,
                                       uint8_t * pBufferLen
                                       )
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_FILE_SETTINGS;
    bCmdBuff[wCmdLen++] = bFileNo;    

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    /* 7 => Data files, 17 => value files, 13 => record files */
    if ((wRxlen != 7) && (wRxlen != 17) && (wRxlen != 13))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pBufferLen and return  */
    *pBufferLen = (uint8_t)wRxlen;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_ChangeFileSettings(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t *pAccessRights
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CHANGE_FILE_SETTINGS;
    bCmdBuff[wCmdLen++] = bFileNo;

    bCmdBuff[wCmdLen++] = bCommSett >> 4;
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_Sw_Int_Write_Enc(pDataParams,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            &bCmdBuff[2],
            wCmdLen - 2
            );
    }
    else if (((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD) ||
        ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_PLAIN))
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_Sw_Int_Write_Plain(pDataParams,
            bCmdBuff,
            wCmdLen,
            PHAL_MFDF_COMMUNICATION_PLAIN,
            NULL,
            0x0000
            );
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
}

phStatus_t phalMfdf_Sw_CreateStdDataFile(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    )
{
    /*
    If (bKeySettings2 & 0x03)== 00 [DES, 3DES] then pDataParams->bAuthMode can be either
    0x0A or 0x1A.
    If (bKeySettings2 & 0x03)== 01 [3K3DES] then pDataParams->bAuthMode can only be 0x1A.
    If (bKeySettings2 & 0x03)== 10 [AES] then pDataParams->bAuthMode can only be 0xAA.
    */
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_STD_DATAFILE;

    /* File Number */
    bCmdBuff[wCmdLen++] = bFileNo;

    /* Copy ISO Filed ID if present */
    if (bOption == 0x01)
    {
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }

    /* Copy communication settings. communication settings in the first nibble so right shifting */
    bCmdBuff[wCmdLen++] = bCommSett >> 4;

    /* Copy Access rights */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    /* Copy File size supplied by the user */
    memcpy(&bCmdBuff[wCmdLen], pFileSize, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_CreateBackupDataFile(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_BKUP_DATAFILE;

    /* File Number */
    bCmdBuff[wCmdLen++] = bFileNo;

    /* Copy ISO Filed ID if present */
    if (bOption == 0x01)
    {
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }

    /* Copy communication settings */
    bCmdBuff[wCmdLen++] = bCommSett >> 4;

    /* Copy Access rights */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    /* Copy File size supplied by the user */
    memcpy(&bCmdBuff[wCmdLen], pFileSize, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_CreateValueFile(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bFileNo,
                                       uint8_t bCommSett,
                                       uint8_t * pAccessRights,
                                       uint8_t * pLowerLmit,
                                       uint8_t * pUpperLmit,
                                       uint8_t * pValue,
                                       uint8_t bLimitedCredit
                                       )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_VALUE_FILE;
    bCmdBuff[wCmdLen++] = bFileNo;
    bCmdBuff[wCmdLen++] = bCommSett >> 4;

    memcpy(bCmdBuff+wCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 2;

    memcpy(bCmdBuff+wCmdLen, pLowerLmit, 4); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 4;

    memcpy(bCmdBuff+wCmdLen, pUpperLmit, 4); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 4;

    memcpy(bCmdBuff+wCmdLen, pValue, 4); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 4;

    bCmdBuff[wCmdLen++] = bLimitedCredit;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_CreateLinearRecordFile(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_LINEAR_RECFILE;

    /* Copy the value information supplied by the user */
    bCmdBuff[wCmdLen++] = bFileNo;

    if (bOption == 1)
    {
        bCmdBuff[wCmdLen++] = pIsoFileId[0];
        bCmdBuff[wCmdLen++] = pIsoFileId[1];
    }
    bCmdBuff[wCmdLen++] = bCommSett>>4;
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    bCmdBuff[wCmdLen++] = pRecordSize[0];
    bCmdBuff[wCmdLen++] = pRecordSize[1];
    bCmdBuff[wCmdLen++] = pRecordSize[2];

    bCmdBuff[wCmdLen++] = pMaxNoOfRec[0];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[1];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[2];

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_CreateCyclicRecordFile(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_CYCLIC_RECFILE;

    /* Copy the value information supplied by the user */
    bCmdBuff[wCmdLen++] = bFileNo;

    if (bOption == 1)
    {
        bCmdBuff[wCmdLen++] = pIsoFileId[0];
        bCmdBuff[wCmdLen++] = pIsoFileId[1];
    }
    bCmdBuff[wCmdLen++] = bCommSett >> 4;
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    bCmdBuff[wCmdLen++] = pRecordSize[0];
    bCmdBuff[wCmdLen++] = pRecordSize[1];
    bCmdBuff[wCmdLen++] = pRecordSize[2];

    bCmdBuff[wCmdLen++] = pMaxNoOfRec[0];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[1];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[2];

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_DeleteFile(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t bFileNo
                                  )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[10];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_DELETE_FILE;
    bCmdBuff[wCmdLen++] = bFileNo;    

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

/** @} */

/**
* \name Data Manipulation Commands
*/

phStatus_t phalMfdf_Sw_ReadData(
                                phalMfdf_Sw_DataParams_t * pDataParams,
                                uint8_t bOption,
                                uint8_t bFileNo,
                                uint8_t * pOffset,
                                uint8_t * pLength,
                                uint8_t ** ppRxdata,
                                uint16_t * pRxdataLen
                                )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wDataLen = 0;
    phStatus_t  PH_MEMLOC_REM status;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption & 0x0FU) == PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if ((bOption & 0x0FU) == PH_EXCHANGE_DEFAULT)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_READ_DATA;
        bCmdBuff[wCmdLen++] = bFileNo;
        memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
        wCmdLen += 3;
        memcpy(&bCmdBuff[wCmdLen], pLength, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        wDataLen = (uint16_t)pLength[1]; /* MSB */
        wDataLen <<= 8;
        wDataLen |= pLength[0]; /* LSB */
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        status = phalMfdf_Sw_Int_ReadData_Enc(
            pDataParams,
            bOption,
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else if (((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD) ||
        ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_PLAIN))
    {
        status = phalMfdf_Sw_Int_ReadData_Plain(
            pDataParams,
            bOption,
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((status == PH_ERR_SUCCESS) && (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
    {
        if ((wDataLen != *pRxdataLen) && (wDataLen != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
    }
    return status;
}

phStatus_t phalMfdf_Sw_WriteData(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint8_t bCommOption,
                                 uint8_t bFileNo,
                                 uint8_t * pOffset,
                                 uint8_t * pData,
                                 uint8_t * pDataLen
                                 )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wDataLen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_WRITE_DATA;
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* Assuming here that the size can never go beyond FFFF. 
    In fact it can never go beyond 8092 (1F9C) bytes */
    wDataLen = (uint16_t)pDataLen[1];
    wDataLen = wDataLen << 8;
    wDataLen |= pDataLen[0];

    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDF_COMMUNICATION_PLAIN;
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_Sw_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            PH_CRYPTOSYM_PADDING_MODE_1,
            pData,
            wDataLen
            );
    }
    else
    {
        return phalMfdf_Sw_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            bCommOption,
            pData,
            wDataLen
            );
    }
}

phStatus_t phalMfdf_Sw_GetValue(
                                phalMfdf_Sw_DataParams_t * pDataParams,
                                uint8_t bCommOption,
                                uint8_t bFileNo,
                                uint8_t * pValue
                                )
{

    /* If not authenticated, send the data and get the value in plain.
    Else use the mode dictated by the caller of this API
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_VALUE;
    bCmdBuff[wCmdLen++] = bFileNo;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Enc(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            &wRxlen
            ));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            &wRxlen
            ));
    }

    if (wRxlen != 4)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pValue, pRecv, wRxlen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_Credit(
                              phalMfdf_Sw_DataParams_t * pDataParams,
                              uint8_t bCommOption,
                              uint8_t bFileNo,
                              uint8_t * pValue
                              )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREDIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_Sw_Int_Write_Enc(pDataParams,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            &bCmdBuff[2],
            0x0004
            );
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_Sw_Int_Write_Plain(pDataParams,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
}

phStatus_t phalMfdf_Sw_Debit(
                             phalMfdf_Sw_DataParams_t * pDataParams,
                             uint8_t bCommOption,
                             uint8_t bFileNo,
                             uint8_t * pValue
                             )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_DEBIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_Sw_Int_Write_Enc(pDataParams,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            &bCmdBuff[2],
            0x0004
            );
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_Sw_Int_Write_Plain(pDataParams,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
}

phStatus_t phalMfdf_Sw_LimitedCredit(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t bCommOption,
                                     uint8_t bFileNo,
                                     uint8_t * pValue
                                     )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_LIMITED_CREDIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_Sw_Int_Write_Enc(pDataParams,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            &bCmdBuff[2],
            0x0004
            );
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_Sw_Int_Write_Plain(pDataParams,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
}

phStatus_t phalMfdf_Sw_WriteRecord(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint8_t bCommOption,
                                   uint8_t bFileNo,
                                   uint8_t * pOffset,
                                   uint8_t * pData,
                                   uint8_t * pDataLen
                                   )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wDataLen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_WRITE_RECORD;
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* Assuming here that the size can never go beyond FFFF. In fact it can never go beyond 8092 (1F9C) bytes */
    wDataLen = (uint16_t)pDataLen[1];
    wDataLen = wDataLen << 8;
    wDataLen |= pDataLen[0];

    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDF_COMMUNICATION_PLAIN;
    }

    if ( (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)||
        (bCommOption == PHAL_MFDF_COMMUNICATION_MACD) ||
        (bCommOption == PHAL_MFDF_COMMUNICATION_PLAIN))
    {
        return phalMfdf_Sw_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            bCommOption,
            pData,
            wDataLen
            );
    }
    else
    {
        return phalMfdf_Sw_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            PH_CRYPTOSYM_PADDING_MODE_1,
            pData,
            wDataLen
            );
    }
}

phStatus_t phalMfdf_Sw_ReadRecords(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint8_t bCommOption,
                                   uint8_t bFileNo,
                                   uint8_t * pOffset,
                                   uint8_t * pNumRec,
                                   uint8_t * pRecSize,
                                   uint8_t ** ppRxdata,
                                   uint16_t * pRxdataLen
                                   )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wRecLen = 0;
    uint16_t    PH_MEMLOC_REM wNumRec;
    phStatus_t  PH_MEMLOC_REM status;

    PHAL_MFDF_UNUSED_ARRAY(pRecSize);
    /* Should also handle the scenario where 0xAF is returned by
    the PICC */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bCommOption & 0x0FU) ==  PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if ((bCommOption & 0x0FU) ==  PH_EXCHANGE_DEFAULT)
    {
        /* form the command */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_READ_RECORDS;
        bCmdBuff[wCmdLen++] = bFileNo;

        /* Offset */
        memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        /* Length */
        memcpy(&bCmdBuff[wCmdLen], pNumRec, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        wRecLen = (uint16_t)pRecSize[1]; /* MSB */
        wRecLen <<= 8;
        wRecLen |= pRecSize[0]; /* LSB */

        wNumRec = (uint16_t)pNumRec[1]; /* MSB */
        wNumRec <<= 8;
        wNumRec |= pNumRec[0]; /* LSB */
        /* Total number of bytes to read */
        wRecLen = (uint16_t)wRecLen * wNumRec;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        status = phalMfdf_Sw_Int_ReadData_Enc(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else if (((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_PLAIN) ||
        ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD))
    {
        status = phalMfdf_Sw_Int_ReadData_Plain(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((status == PH_ERR_SUCCESS) && (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
    {
        /* Can check this case if user has not given num of records
        as 0x000000. If 0x000000, then all records are read */
        if ((wRecLen != *pRxdataLen) && (wRecLen != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
    }
    return status;
}

phStatus_t phalMfdf_Sw_ClearRecordFile(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bFileNo
                                       )
{
    uint8_t PH_MEMLOC_REM bCmdBuff[20];
    uint16_t PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CLEAR_RECORDS_FILE;
    bCmdBuff[wCmdLen++] = bFileNo;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_CommitTransaction(
    phalMfdf_Sw_DataParams_t * pDataParams
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_COMMIT_TXN;    

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_AbortTransaction(
                                        phalMfdf_Sw_DataParams_t * pDataParams
                                        )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_ABORT_TXN;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_Sw_IsoSelectFile(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t bOption,
                                     uint8_t bSelector,
                                     uint8_t *pFid,
                                     uint8_t * pDFname,
                                     uint8_t bDFnameLen,
                                     uint8_t **ppRecv,
                                     uint16_t * pwRxlen
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bData[24];
    uint8_t     PH_MEMLOC_REM bLc = 0;

    if (bDFnameLen > 16)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bOption != 0x00) && (bOption != 0x0C))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    switch(bSelector)
    {
    case 0x00:
    case 0x02:
        /* Selection by EF Id*/
        /* Send MSB first to card */
        bData[0] = pFid[1];
        bData[1] = pFid[0];
        bLc = 2;
        break;

    case 0x04:
        /* Selection by DF Name */
        memcpy(bData, pDFname, bDFnameLen); /* PRQA S 3200 */
        bLc = bDFnameLen;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* Reset Authentication Status here */
    phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x03,
        PHAL_MFDF_CMD_ISO7816_SELECT_FILE,
        bSelector,
        bOption,
        bLc,
        bData,
        0x00,
        ppRecv,
        pwRxlen));

    /* ISO wrapped mode is on */
    pDataParams->bWrappedMode = 1;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_IsoReadBinary(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t bOffset,
                                     uint8_t bSfid,
                                     uint8_t bBytesToRead,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pBytesRead
                                     )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLe = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wOffset;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (wOption == PH_EXCHANGE_DEFAULT)
    {
        if (bSfid & 0x80)
        {
            /* Short file id is supplied */
            if ((bSfid & 0x7FU) > 0x1F)
            {
                /* Error condition */
                return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
            }
            bP1 = bSfid;
            bP2 = bOffset;
        }    
        else
        {
            /* P1 and P2 code the offset */
            wOffset = bP1 = bSfid;
            wOffset <<= 8; /* Left shift */
            wOffset |= bOffset;
            bP2 = bOffset;
        }
        bLe = bBytesToRead;
        bCmdBuff[wCmdLen++] = 0x00; /* Class */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_BINARY; /* Ins */
        bCmdBuff[wCmdLen++] = bP1;
        bCmdBuff[wCmdLen++] = bP2;
        bCmdBuff[wCmdLen++] = bBytesToRead; /* Le */
    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        wCmdLen = 0;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    return phalMfdf_Sw_Int_IsoRead(
        pDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );
}

phStatus_t phalMfdf_Sw_IsoUpdateBinary(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bOffset,
                                       uint8_t bSfid,
                                       uint8_t * pData,
                                       uint8_t bDataLen
                                       )

{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    uint16_t    PH_MEMLOC_REM wOffset;
    phStatus_t  PH_MEMLOC_REM status;

    if (bSfid & 0x80)
    {
        /* Short file id is supplied */
        if ((bSfid & 0x7FU) > 0x1F)
        {
            /* Error condition */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bP2 = bOffset;
    }    
    else
    {
        /* P1 and P2 code the offset */
        wOffset = bP1 = bSfid;
        wOffset <<= 8; /* Left shift */
        wOffset |= bOffset;
        bP2 = bOffset;
    }

    bLc = bDataLen;
    status = phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_UPDATE_BINARY,
        bP1,
        bP2,
        bLc,
        pData,
        0x00,
        NULL,
        NULL);
    if (status != PH_ERR_SUCCESS)
    {
        /* Reset authentication status */
        phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
    }
    return status;
}

phStatus_t phalMfdf_Sw_IsoReadRecords(
                                      phalMfdf_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t bRecNo,
                                      uint8_t bReadAllFromP1,
                                      uint8_t bSfid,
                                      uint8_t bBytesToRead,
                                      uint8_t ** ppRxBuffer,
                                      uint16_t * pBytesRead
                                      )

{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLe = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (wOption == PH_EXCHANGE_DEFAULT)
    {
        if (bSfid > 0x1F)
        {
            /* Invalid Short File Id */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        else
        {
            /* Valid Sfid */
            bP1 = bRecNo;
            bP2 = bSfid;
            bP2 <<= 3; /* left shift by 3 bits to move SFID to bits 7 to 3 */
            if (bReadAllFromP1)
            {
                bP2 |= 0x05; /* Last three bits of P2 = 101 */
            }
            else
            {
                bP2 |= 0x04; /* Last three bits of P2 = 100. Read only record P1 */
            }
        }
        bLe = bBytesToRead;
        bCmdBuff[wCmdLen++] = 0x00; /* Class */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_RECORDS; /* Ins */
        bCmdBuff[wCmdLen++] = bP1;
        bCmdBuff[wCmdLen++] = bP2;
        bCmdBuff[wCmdLen++] = bBytesToRead; /* Le */
    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        wCmdLen = 0;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    return phalMfdf_Sw_Int_IsoRead(
        pDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );
}

phStatus_t phalMfdf_Sw_IsoAppendRecord(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bSfid,
                                       uint8_t * pData,
                                       uint8_t bDataLen
                                       )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    phStatus_t  PH_MEMLOC_REM status;

    if (bSfid > 0x1F)
    {
        /* Invalid Short File Id */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    bP2 = bSfid;
    bP2 <<= 3; /* left shift by 3 bits to move SFID to bits 7 to 3 */
    /* Last three bits of P2 = 000 */

    bLc = bDataLen;
    status = phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_APPEND_RECORD,
        bP1,
        bP2,
        bLc,
        pData,
        0x00,
        NULL,
        NULL);
    if (status != PH_ERR_SUCCESS)
    {
        /* Reset authentication status */
        phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
    }
    return status;
}

phStatus_t phalMfdf_Sw_IsoGetChallenge(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVer,
                                       uint8_t bLe,
                                       uint8_t * pRPICC1
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bData[24];
    uint16_t    PH_MEMLOC_REM wKeyType;

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        sizeof(bData),
        bData,
        &wKeyType));

    if ((wKeyType == PH_KEYSTORE_KEY_TYPE_AES128) || (wKeyType == PH_KEYSTORE_KEY_TYPE_3K3DES))
    {
        if (bLe != 0x10)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    else if ((wKeyType == PH_KEYSTORE_KEY_TYPE_DES) || (wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES))
    {
        if (bLe != 0x08)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x02,
        PHAL_MFDF_CMD_ISO7816_GET_CHALLENGE,
        0x00,
        0x00,
        0x00,
        NULL,
        bLe,
        &pRecv,
        &wRxlen));

    if (wRxlen != bLe)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    memcpy(pRPICC1, pRecv, wRxlen); /* PRQA S 3200 */

    /* Reset authentication status */
    phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_IsoExternalAuthenticate(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pInput,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_3K3DES_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bRndBuff[2 * PH_CRYPTOSYM_AES128_KEY_SIZE];

    uint8_t     PH_MEMLOC_REM bIndex = 0;
    uint8_t     PH_MEMLOC_REM bAlgo;
    uint8_t     PH_MEMLOC_REM bIsDFkey;
    uint8_t     PH_MEMLOC_REM bKeyNoCard;
    uint8_t     PH_MEMLOC_REM bRpicc1[16];
    uint8_t     PH_MEMLOC_REM bRpcd1[16];
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint16_t    PH_MEMLOC_REM wKeyNo;   
    uint16_t    PH_MEMLOC_REM wKeyVer;

    if ((bInputLen != 24) && (bInputLen != 40))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    bAlgo = pInput[bIndex++];
    bIsDFkey = pInput[bIndex++];
    bKeyNoCard = pInput[bIndex++];
    bRndLen = pInput[bIndex++];

    memcpy(bRpcd1, &pInput[bIndex], bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(bRpicc1, &pInput[bIndex], bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(&wKeyNo, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;

    memcpy(&wKeyVer, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;


    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* First get the key from key store */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        sizeof(bKey),
        bKey,
        &wKeyType
        ));

    if (wKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        wKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        memcpy(&bKey[8], bKey, 8); /* PRQA S 3200 */
    }

    /* Parameter validation. VAlidating bAlgo and bRndLen */
    switch(bAlgo)
    {
    case 0x00:
        /* Context defined algo. Based on key type */
        if ((wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES) && (bRndLen == PH_CRYPTOSYM_DES_KEY_SIZE))
        {
            bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        }
        else if ((wKeyType == PH_KEYSTORE_KEY_TYPE_3K3DES) && (bRndLen == PH_CRYPTOSYM_2K3DES_KEY_SIZE))
        {
            bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        }
        else if ((wKeyType == PH_KEYSTORE_KEY_TYPE_AES128) && (bRndLen == PH_CRYPTOSYM_AES128_KEY_SIZE))
        {
            bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        }
        else
        {
            /* Either key type or rndlen is invalid */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        break;

    case 0x02:
        /* 2K3DES */
        if ((wKeyType != PH_KEYSTORE_KEY_TYPE_2K3DES) || (bRndLen != PH_CRYPTOSYM_DES_BLOCK_SIZE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x04:
        /* 3K3DES */
        if ((wKeyType != PH_KEYSTORE_KEY_TYPE_3K3DES) || (bRndLen != 2*PH_CRYPTOSYM_DES_BLOCK_SIZE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x09:
        /* AES128 */
        if ((wKeyType != PH_KEYSTORE_KEY_TYPE_AES128) || (bRndLen != PH_CRYPTOSYM_AES_BLOCK_SIZE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        break;

    default:
        /* Invalid key type */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    pDataParams->bCryptoMethod = (uint8_t)wKeyType;

    /* Load Key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bKey,
        wKeyType
        ));
    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    memcpy(bRndBuff, bRpcd1, bRndLen); /* PRQA S 3200 */
    memcpy(&bRndBuff[bRndLen], bRpicc1, bRndLen); /* PRQA S 3200 */

    /* Encrypt RPCD1 + RPICC1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        bRndBuff,
        2 * bRndLen,
        bRndBuff
        ));

    /* Store IV */
    memcpy(pDataParams->bIv, &bRndBuff[2*bRndLen - bIvLen], bIvLen); /* PRQA S 3200 */

    *pOutLen = 0; /* Nothing returned in s/w mode of implementation */
    if (pDataOut)
    {
        pDataOut = 0;
    }

    /* Send the APDU */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_EXT_AUTHENTICATE,
        bAlgo,
        (bIsDFkey << 7) | bKeyNoCard,
        bRndLen * 2,
        bRndBuff,
        0x00,
        &pRecv,
        &wRxlen));

    if (wRxlen != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    return statusTmp;
}

phStatus_t phalMfdf_Sw_IsoInternalAuthenticate(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pInput,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_3K3DES_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bIndex = 0;
    uint8_t     PH_MEMLOC_REM bAlgo;
    uint8_t     PH_MEMLOC_REM bIsDFkey;
    uint8_t     PH_MEMLOC_REM bKeyNoCard;
    uint8_t     PH_MEMLOC_REM bRpcd2[16];
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint16_t    PH_MEMLOC_REM wKeyNo;   
    uint16_t    PH_MEMLOC_REM wKeyVer;


    if ((bInputLen != 16) && (bInputLen != 24))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    bAlgo = pInput[bIndex++];
    bIsDFkey = pInput[bIndex++];
    bKeyNoCard = pInput[bIndex++];
    bRndLen = pInput[bIndex++];

    memcpy(bRpcd2, &pInput[bIndex], bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(&wKeyNo, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;

    memcpy(&wKeyVer, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;

    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* First get the key from key store */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        sizeof(bKey),
        bKey,
        &wKeyType
        ));

    if (wKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        wKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        memcpy(&bKey[8], bKey, 8); /* PRQA S 3200 */
    }

    /* Parameter validation. VAlidating bAlgo and bRndLen */
    switch(bAlgo)
    {
    case 0x00:
        /* Context defined algo. Based on key type */
        if ((wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES) && (bRndLen == PH_CRYPTOSYM_DES_KEY_SIZE))
        {
            bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        }
        else if ((wKeyType == PH_KEYSTORE_KEY_TYPE_3K3DES) && (bRndLen == PH_CRYPTOSYM_2K3DES_KEY_SIZE))
        {
            bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        }
        else if ((wKeyType == PH_KEYSTORE_KEY_TYPE_AES128) && (bRndLen == PH_CRYPTOSYM_AES128_KEY_SIZE))
        {
            bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        }
        else
        {
            /* Either key type or rndlen is invalid */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        break;

    case 0x02:
        /* 2K3DES */
        if ((wKeyType != PH_KEYSTORE_KEY_TYPE_2K3DES) || (bRndLen != PH_CRYPTOSYM_DES_BLOCK_SIZE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x04:
        /* 3K3DES */
        if ((wKeyType != PH_KEYSTORE_KEY_TYPE_3K3DES) || (bRndLen != 2*PH_CRYPTOSYM_DES_BLOCK_SIZE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;

    case 0x09:
        /* AES128 */
        if ((wKeyType != PH_KEYSTORE_KEY_TYPE_AES128) || (bRndLen != PH_CRYPTOSYM_AES_BLOCK_SIZE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        break;

    default:
        /* Invalid key type */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    pDataParams->bCryptoMethod = (uint8_t)wKeyType;

    /* Send the APDU */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x03,
        PHAL_MFDF_CMD_ISO7816_INT_AUTHENTICATE,
        bAlgo,
        (bIsDFkey << 7) | bKeyNoCard,
        bRndLen,
        bRpcd2,
        2 * bRndLen,
        &pRecv,
        &wRxlen));

    if (wRxlen != (2 * bRndLen))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Load Key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bKey,
        wKeyType
        ));
    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Decrypt RPCD1 + RPICC1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
        pRecv,
        wRxlen,
        pDataOut
        ));

    *pOutLen = 2 * bRndLen;

    /* Reset IV */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Session key has to be calculated and this key has to be loaded by the caller */
    if (wKeyType == PH_KEYSTORE_KEY_TYPE_AES128) 
    {
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES;
    }  
    else /* (wKeyType == PH_KEYSTORE_KEY_TYPE_2K3DES)*/
    {
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
    }
    pDataParams->bKeyNo = bKeyNoCard;
    pDataParams->bCryptoMethod = (uint8_t)wKeyType;
    pDataParams->bWrappedMode = 1;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_Sw_IsoAuthenticate(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVer,
                                       uint8_t bKeyNoCard,
                                       uint8_t bIsPICCkey
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM bIndex = 0;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_3K3DES_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bAlgo;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[40];
    uint8_t     PH_MEMLOC_REM bRpcd1[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bRpcd2[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bRpicc1[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bRpicc2[PH_CRYPTOSYM_AES128_KEY_SIZE];

    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* First get the key from key store */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        sizeof(bKey),
        bKey,
        &wKeyType
        ));

    switch (wKeyType)
    {
    case PH_KEYSTORE_KEY_TYPE_2K3DES:
    case PH_KEYSTORE_KEY_TYPE_DES:
        bAlgo = 0x02;
        bRndLen = PH_CRYPTOSYM_DES_KEY_SIZE;
        break;

    case PH_KEYSTORE_KEY_TYPE_3K3DES:
        bAlgo = 0x04;
        bRndLen = PH_CRYPTOSYM_2K3DES_KEY_SIZE;
        break;

    case PH_KEYSTORE_KEY_TYPE_AES128:
        bAlgo = 0x09;
        bRndLen = PH_CRYPTOSYM_AES128_KEY_SIZE;
        break;

    default:
        /* Invalid key type */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_IsoGetChallenge(pDataParams, wKeyNo, wKeyVer, bRndLen, bRpicc1));

    /* Generate PCD1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Seed(pDataParams->pCryptoRngDataParams, bRpicc1, bRndLen));
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndLen, bRpcd1));

    bIndex = 0;
    bWorkBuffer[bIndex++] = bAlgo;
    bWorkBuffer[bIndex++] = !bIsPICCkey;
    bWorkBuffer[bIndex++] = bKeyNoCard;
    bWorkBuffer[bIndex++] = bRndLen;

    memcpy(&bWorkBuffer[bIndex], bRpcd1, bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(&bWorkBuffer[bIndex], bRpicc1, bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(&bWorkBuffer[bIndex], &wKeyNo, 2); /* PRQA S 3200 */
    bIndex += 2;

    memcpy(&bWorkBuffer[bIndex], &wKeyVer, 2); /* PRQA S 3200 */
    bIndex += 2;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_IsoExternalAuthenticate(
        pDataParams,
        bWorkBuffer,
        bIndex,
        NULL,
        &bIndex
        ));

    /* Generate PCD2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Seed(pDataParams->pCryptoRngDataParams, bRpcd1, bRndLen));
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndLen, bRpcd2));

    bIndex = 0;
    bWorkBuffer[bIndex++] = bAlgo;
    bWorkBuffer[bIndex++] = !bIsPICCkey;
    bWorkBuffer[bIndex++] = bKeyNoCard;
    bWorkBuffer[bIndex++] = bRndLen;

    memcpy(&bWorkBuffer[bIndex], bRpcd2, bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(&bWorkBuffer[bIndex], &wKeyNo, 2); /* PRQA S 3200 */
    bIndex += 2;

    memcpy(&bWorkBuffer[bIndex], &wKeyVer, 2); /* PRQA S 3200 */
    bIndex += 2;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_IsoInternalAuthenticate(
        pDataParams,
        bWorkBuffer,
        bIndex,
        bWorkBuffer,
        &bIndex
        ));

    /* Verify bRpcd2. Store bRpicc2. Generate session key */
    if (memcmp(&bWorkBuffer[bRndLen], bRpcd2, bRndLen) == 0)
    {
        memcpy(bRpicc2, bWorkBuffer, bRndLen); /* PRQA S 3200 */
    }
    else
    {
        /* return authentication error*/
        phalMfdf_Sw_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDF);
    }

    switch(wKeyType)
    {
    case PH_KEYSTORE_KEY_TYPE_DES:
        memcpy(pDataParams->bSessionKey, bRpcd1, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[4], bRpicc2, 4); /* PRQA S 3200 */
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
        break;

    case PH_KEYSTORE_KEY_TYPE_2K3DES:
        memcpy(pDataParams->bSessionKey, bRpcd1, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[4], bRpicc2, 4); /* PRQA S 3200 */
        if (memcmp(bKey, &bKey[8], 8) == 0)
        {
            memcpy(&pDataParams->bSessionKey[8], pDataParams->bSessionKey, 8); /* PRQA S 3200 */
        }
        else
        {
            memcpy(&pDataParams->bSessionKey[8], &bRpcd1[4], 4); /* PRQA S 3200 */
            memcpy(&pDataParams->bSessionKey[12], &bRpicc2[4], 4); /* PRQA S 3200 */
        }
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
        break;

    case PH_KEYSTORE_KEY_TYPE_3K3DES:
        memcpy(pDataParams->bSessionKey, bRpcd1, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[4], bRpicc2, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[8], &bRpcd1[6], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[12], &bRpicc2[6], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[16], &bRpcd1[12], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[20], &bRpicc2[12], 4); /* PRQA S 3200 */
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
        break;

    case PH_KEYSTORE_KEY_TYPE_AES128:
        memcpy(pDataParams->bSessionKey, bRpcd1, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[4], bRpicc2, 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[8], &bRpcd1[12], 4); /* PRQA S 3200 */
        memcpy(&pDataParams->bSessionKey[12], &bRpicc2[12], 4); /* PRQA S 3200 */
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES; /* PRQA S 3200 */
        break;

    default:
        /* Invalid key type. This code is not reachable */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    pDataParams->bKeyNo = bKeyNoCard;
    pDataParams->bCryptoMethod = (uint8_t)wKeyType;
    pDataParams->bWrappedMode = 1;

    /* Load session key */
    PH_CHECK_SUCCESS_FCT(status, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bSessionKey,
        pDataParams->bCryptoMethod
        ));

    /* Need to set config to keep the IV ON between CMAC calculation */
    return phCryptoSym_SetConfig(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        PH_CRYPTOSYM_VALUE_KEEP_IV_ON
        );
}

phStatus_t phalMfdf_Sw_GetConfig(
                                 phalMfdf_Sw_DataParams_t *pDataParams,
                                 uint16_t wConfig,
                                 uint16_t *pValue
                                 )
{
    switch (wConfig)
    {
    case PHAL_MFDF_ADDITIONAL_INFO:
        *pValue = pDataParams->wAdditionalInfo;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}
phStatus_t phalMfdf_Sw_SetConfig(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t wValue
                                 )
{
    switch (wConfig)
    {
    case PHAL_MFDF_ADDITIONAL_INFO:
        pDataParams->wAdditionalInfo = wValue;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);    
}

#endif /* NXPBUILD__PHAL_MFDF_SW */
