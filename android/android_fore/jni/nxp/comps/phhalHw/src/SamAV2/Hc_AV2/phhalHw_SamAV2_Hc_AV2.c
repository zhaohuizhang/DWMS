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
* $Revision: 1.18 $
* $Date: Fri Apr 15 09:26:42 2011 $
*
* History:
*  CHu: Generated 27. July 2009
*
*/

#include <ph_Status.h>
#include <phKeyStore.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_SAMAV2

#include <phhalHw_SamAV2_Cmd.h>
#include "phhalHw_SamAV2_Hc_AV2.h"
#include "phhalHw_SamAV2_Hc_AV2_Int.h"
#include "../phhalHw_SamAV2.h"
#include "../phhalHw_SamAV2_HcUtils.h"

/*
* Private constants
*/
static const uint8_t PH_MEMLOC_CONST_ROM phhalHw_SamAV2_Hc_AV2_FirstIv[PH_CRYPTOSYM_AES_BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

phStatus_t phhalHw_SamAV2_Hc_AV2_AuthenticateHost(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t  bAuthType,
    uint16_t wRdKeyRefNum,
    uint16_t wRdKeyRefVer,
    uint8_t  bSamKeyRefNum, 
    uint8_t  bSamKeyRefVer
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[9];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM aRnd2[16];
    uint8_t     PH_MEMLOC_REM aRnd1[16];
    uint8_t     PH_MEMLOC_REM aKey[24];
    uint8_t     PH_MEMLOC_REM aMac[16];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM aKxeKey[24];
    uint8_t     PH_MEMLOC_REM aRndAB[32];
    uint8_t     PH_MEMLOC_REM bKeyLength;
    uint8_t     PH_MEMLOC_REM bMacLength = 0;
    uint8_t     PH_MEMLOC_REM aDefaultLe[1];

    /* Set default LE byte */
    aDefaultLe[0] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    memset(aCmd, 0, sizeof(aCmd));  /* PRQA S 3200 */

    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wRdKeyRefNum,
        wRdKeyRefVer,
        sizeof(aKey),
        aKey,
        &wKeyType));

    if ((wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128) && (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES192))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    pDataParams->bAuthType = 0;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_HOST_INS;    /* INS */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 3;    
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1] = bSamKeyRefNum;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2] = bSamKeyRefVer;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3] = bAuthType;

    /* Exchange first part of the command */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        9,
        &pResponse,
        &wResponseLength);

    /* We expect chaining active error */
    if (status != (PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }
    if (wResponseLength != PHHAL_HW_SAMAV2_HOST_AUTH_RESP1_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }
    memcpy(aRnd2, pResponse, PHHAL_HW_SAMAV2_HOST_AUTH_RESP1_LENGTH);  /* PRQA S 3200 */

    /* now we start with processing of HC Part 2 */

    /* to prepare the 2nd part, we have to MAC: Rnd2 | HostMode | ZeroPadding, use temporarly aRnd2 */
    aRnd2[12] = bAuthType;
    memset(&aRnd2[13], 0, 3); /* PRQA S 3200 */

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(pDataParams->pMACCryptoDataParams, aKey, wKeyType));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pMACCryptoDataParams, 
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aRnd2, 
        16, 
        aMac, 
        &bMacLength));

    /* we have to truncate the MAC*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_TruncateMacBuffer(aMac, &bMacLength));

    /* Initialize the aRnd1 array*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, 12, aRnd1));

    /* prepare the 2nd part of Authenticate*/
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = /* MAC */ bMacLength + /* RND1 */ 12;    /*LC*/

    /* Exchange first part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        5,
        NULL,
        NULL));

    /* Exchange MAC part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aMac,
        bMacLength,
        NULL,
        NULL));

    /* Exchange RND1 part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aRnd1,
        12,
        NULL,
        NULL));

    /* Exchange LE part of the command */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* We expect chaining active error */
    if (status != (PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }

    if (wResponseLength != PHHAL_HW_SAMAV2_HOST_AUTH_RESP2_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* to prepare the 2nd part response, we have to MAC: Rnd1 | HostMode | ZeroPadding, use temporarly aRnd1 */
    aRnd1[12] = bAuthType;
    memset(&aRnd1[13], 0, 3); /* PRQA S 3200 */

    /* Key already loaded */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pMACCryptoDataParams, 
        PH_CRYPTOSYM_MAC_MODE_CMAC, 
        aRnd1, 
        16, 
        aMac, 
        &bMacLength));
    /* we have to truncate the MAC*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_TruncateMacBuffer(aMac, &bMacLength));
    /* check they are equal*/
    if (memcmp(aMac, pResponse, bMacLength) ) 
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
    }

    /* Derive the Kxe key from kx using Rnd1 and Rnd2 - note: Secret key needs to be loaded into MAC data params*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_GenerateAuthEncKey(
        pDataParams,
        (uint8_t)wKeyType, aRnd1, aRnd2, aKxeKey, &bKeyLength)); 

    /* From now on, we can use aRnd1 and aRnd2 to store RNDA (Rnd1) and RNDB (Rnd2) */

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(pDataParams->pENCCryptoDataParams, aKxeKey, wKeyType));

    /* Load initial IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        phhalHw_SamAV2_Hc_AV2_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* get the Encrypted(Kxe, RndB) in Rnd2*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams, 
        PH_CRYPTOSYM_CIPHER_MODE_CBC,
        &pResponse[8], 
        16, 
        aRnd2));

    /* Now we start with part 3 exchange */
    /* Initialize the RndA array*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, 16, aRnd1));
    memcpy(aRndAB, aRnd1, 16);  /* PRQA S 3200 */

    /* calculate RndB'*/
    memcpy(&aRndAB[16], &aRnd2[2], 14);  /* PRQA S 3200 */
    aRndAB[30] = aRnd2[0];
    aRndAB[31] = aRnd2[1];

    /* Encrypt aRndAB*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pENCCryptoDataParams, 
        PH_CRYPTOSYM_CIPHER_MODE_CBC, 
        aRndAB, 
        32, 
        aRndAB));

    /* prepare the buffer*/
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 32;    /*LC*/

    /* Exchange first part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        5,
        NULL,
        NULL));

    /* Exchange RNDAB part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aRndAB,
        32,
        NULL,
        NULL));

    /* Exchange LE part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != PHHAL_HW_SAMAV2_HOST_AUTH_RESP3_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Load initial IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        phhalHw_SamAV2_Hc_AV2_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* decrypt the Response3 and check RndA'*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams, 
        (PH_CRYPTOSYM_CIPHER_MODE_CBC),
        pResponse, 
        16, 
        aRndAB));
    if (memcmp(aRndAB, &aRnd1[2], 14) != 0)
    {
        /* the response for RndA is not equal to sent RndA, Authentication failed PH_ERR_AUTH_ERROR*/
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
    }

    if ((aRnd1[0] != aRndAB[14])||(aRnd1[1] != aRndAB[15]))
    {
        /* the response for RndA is not equal to sent RndA, Authentication failed PH_ERR_AUTH_ERROR*/
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
    }

    /* Host Authentication is successfully completed */
    /* Generate the current SessionKey for this HostAuthentication*/
    /* aRndAB, aKxeKey reused for session keys, secret key is still loaded in MAC data params */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_GenerateSessionKey(pDataParams,(uint8_t)wKeyType, aRnd1, aRnd2, aRndAB, aKxeKey, &bKeyLength));

    /* reset both pENCCryptoDataParams and pMACCryptoDataParams with Sessionkey*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(pDataParams->pENCCryptoDataParams, aRndAB, wKeyType));

    /* Load initial IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        phhalHw_SamAV2_Hc_AV2_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(pDataParams->pMACCryptoDataParams, aKxeKey, wKeyType));

    /* Load initial IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pMACCryptoDataParams,
        phhalHw_SamAV2_Hc_AV2_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* reset the Command counter */
    pDataParams->Cmd_Ctr = 0;
    pDataParams->bAuthType = bAuthType;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_Encrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBuffer,
    uint16_t wTxLength, 
    uint16_t wBufferSize,
    uint16_t * pTxLength,
    uint8_t bFirst,
    uint8_t bLast
    )
{
    phStatus_t statusTmp;
    uint8_t LePresent =  PH_OFF;
    uint8_t LcPresent =  PH_OFF;
    uint8_t bLc = 0;
    uint8_t bLe;
    uint8_t bTmpBuf[16];

    uint16_t wHelper;
    uint16_t wCurrentEncryptedDataSize;
    uint16_t wRemainingDataSize;

    *pTxLength = wTxLength;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_GetCheckLcLe(pBuffer, wTxLength, &LcPresent, &bLc, &LePresent));

    if (bFirst)
    {
        /* load the InitializationVector, because we start a new Encryption.
        This has to be done even if no data are send to the SAM. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_InitAndLoadIV(pDataParams, pDataParams->bPendingCmdIv, true));
    }
    else
    {
        /* Load encryption IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pENCCryptoDataParams,
            pDataParams->bPendingCmdIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Do we need encryption at all? */
    if (!bLc)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Check for overall size */
    if ((wTxLength) > (wBufferSize - 15))
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
    }

    /*/////////////////////////
    // Start Encryption Process
    ///////////////////////////
    // At this point, the encryption process can be started and processed
    // we only have to know if a frame chaining was running*/

    /* save Le byte */
    bLe = pBuffer[wTxLength - 1];

    if (bFirst)
    {
        /* Find all blocks but the last block */
        wRemainingDataSize = (bLc % 16);

        wCurrentEncryptedDataSize = ((uint16_t)bLc - wRemainingDataSize);

        if (wCurrentEncryptedDataSize)
        {
            /* Encrypt everything but the last block*/
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pENCCryptoDataParams, 
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH],
                wCurrentEncryptedDataSize,
                &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH]));
        }

        /* Recopy remaining part */
        memcpy(bTmpBuf, &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wCurrentEncryptedDataSize], wRemainingDataSize);  /* PRQA S 3200 */
    }
    else
    {
        /* How much data do we still have? - wHelper contains already consumed data out of pBuffer */
        wHelper = 16 - pDataParams->bPendingEncCmdDataLength;
        wCurrentEncryptedDataSize = 0;

        /* Do we have sufficient user Payload? */
        if (wHelper > bLc)
        {
            memcpy(bTmpBuf, pDataParams->bPendingEncCmdData, pDataParams->bPendingEncCmdDataLength);  /* PRQA S 3200 */ 
            memcpy(&bTmpBuf[pDataParams->bPendingEncCmdDataLength], &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH], bLc);  /* PRQA S 3200 */
            wRemainingDataSize = pDataParams->bPendingEncCmdDataLength + bLc;
        }
        else
        {
            memcpy(&pDataParams->bPendingEncCmdData[pDataParams->bPendingEncCmdDataLength], &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH], wHelper);  /* PRQA S 3200 */

            /* Encrypt first block*/
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pENCCryptoDataParams, 
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                pDataParams->bPendingEncCmdData,
                16, 
                pDataParams->bPendingEncCmdData));

            wRemainingDataSize = (((uint16_t)bLc - wHelper) % 16);

            /* Next blocks we can now encipher inline */
            wCurrentEncryptedDataSize = ((uint16_t)bLc - wRemainingDataSize - wHelper);

            if (wCurrentEncryptedDataSize)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                    pDataParams->pENCCryptoDataParams, 
                    PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                    &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wHelper],
                    wCurrentEncryptedDataSize,
                    &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wHelper]));

                /* Now move the data to the TmpBuf*/
                memcpy(bTmpBuf, &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wHelper + wCurrentEncryptedDataSize ], wRemainingDataSize);  /* PRQA S 3200 */

                /* Now move encrypted payload to the end */
                memmove(&pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 16], &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wHelper], (uint16_t)wCurrentEncryptedDataSize);  /* PRQA S 3200 */

            }else
            {
                /* Recopy remaining part */
                memcpy(bTmpBuf, &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wHelper], wRemainingDataSize);  /* PRQA S 3200 */
            }

            /* Now copy the stuff to the front */
            memcpy(&pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH], pDataParams->bPendingEncCmdData, 16);  /* PRQA S 3200 */
            wCurrentEncryptedDataSize = wCurrentEncryptedDataSize + 16;
        }
    }

    /* Is this the last command in a sequence? */
    if (bLast)
    {
        /* copy temporary buffer to the end of the Tx Buffer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
            PH_CRYPTOSYM_PADDING_MODE_2,
            bTmpBuf,
            wRemainingDataSize,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            16,
            bTmpBuf,
            &wRemainingDataSize));

        /* now encrypt the data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pENCCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_LAST,
            bTmpBuf,
            wRemainingDataSize, 
            &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wCurrentEncryptedDataSize]));

        wCurrentEncryptedDataSize = wCurrentEncryptedDataSize + 16;
    }
    else
    {
        if (wCurrentEncryptedDataSize >= 16)
        {
            /* Recopy last block of the encrypted data into the temporary IV space */
            memcpy(pDataParams->bPendingCmdIv, &pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wCurrentEncryptedDataSize - 16], sizeof(pDataParams->bPendingCmdIv));  /* PRQA S 3200 */
        }

        /* Copy data into pending data structure */
        memcpy(pDataParams->bPendingEncCmdData, bTmpBuf, wRemainingDataSize);  /* PRQA S 3200 */
        pDataParams->bPendingEncCmdDataLength = (uint8_t)(wRemainingDataSize);
    }

    /* Update Lc */
    pBuffer[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = (uint8_t)wCurrentEncryptedDataSize;

    /* Update overall length */
    if (bLc > wCurrentEncryptedDataSize)
    {
        bLc = (uint8_t)(bLc - wCurrentEncryptedDataSize);
        wTxLength = wTxLength - bLc;
    }
    else
    {
        bLc = (uint8_t)(wCurrentEncryptedDataSize - bLc);
        wTxLength = wTxLength + bLc;
    }

    /* Update Le */
    if (LePresent)
    {
        pBuffer[wTxLength - 1] = bLe;
    }

    *pTxLength = wTxLength;
    /* End Encryption Process */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_Decrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint8_t * pBuffer,
    uint16_t wRxLength, 
    uint16_t * pRxLength,
    uint8_t bFirst,
    uint8_t bLast
    )
{
    phStatus_t statusTmp;
    *pRxLength = 0;

    if (bFirst)
    {
        /* load the InitializationVector (phCryptoSym_CryptoPP_LoadIv), because we start a new decryption.
        This has to be done even if no data is returned by the SAM. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_InitAndLoadIV(
            pDataParams, 
            pDataParams->bPendingRespIv, 
            false)); 
    }
    else
    {
        /* Load decryption IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pENCCryptoDataParams,
            pDataParams->bPendingRespIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    if (wRxLength < (16 /* Data */ + PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH))
    {
        *pRxLength = wRxLength;
        /* Obviously, no data is available */
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    if (((wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH) % 16) != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    if (!bLast)
    {
        /* Recopy last block of the encrypted data into the temporary IV space */
        memcpy(pDataParams->bPendingRespIv, &pBuffer[wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH - 16], sizeof(pDataParams->bPendingRespIv));  /* PRQA S 3200 */
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams, 
        (PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT),
        pBuffer,
        (wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH),
        pBuffer));

    if (bLast)
    {
        /* remove padding in pPlainBuffer and Update the size of decrypted buffer*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_RemovePadding(
            PH_CRYPTOSYM_PADDING_MODE_2,
            pBuffer,
            wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH, 
            pBuffer,
            pRxLength));

        /* Reorder SW1 SW2 */
        pBuffer[(*pRxLength)++] = pBuffer[wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH];
        pBuffer[(*pRxLength)++] = pBuffer[wRxLength - 1];
    }
    else
    {
        /* Set response length only */
        *pRxLength = wRxLength;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_AppendMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBuffer,
    uint16_t wTxLength, 
    uint16_t wBufferSize,
    uint16_t * pTxLength,
    uint8_t bFirst,
    uint8_t bLast
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bLcPresent = PH_OFF;
    uint8_t     PH_MEMLOC_REM bLePresent = PH_OFF;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    uint8_t     PH_MEMLOC_REM bLeValue = 0;
    uint8_t     PH_MEMLOC_REM aTmpBuf[16];
    uint16_t    PH_MEMLOC_REM wHelper;
    uint16_t    PH_MEMLOC_REM wValidMacData = 0;
    uint8_t     PH_MEMLOC_REM bMacLength;

    *pTxLength = wTxLength;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_GetCheckLcLe(pBuffer, wTxLength, &bLcPresent, &bLc, &bLePresent));

    /* check the buffer size compare to the size of data to MAC*/
    if ((wTxLength) > (wBufferSize - 8))
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
    }

    /* In case of non-first command and LC == 0 (response chaining no MAC should be appended */
    if ((!bFirst) && (!bLcPresent))
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Remember Le */
    bLeValue = pBuffer[wTxLength - 1];

    if (bFirst)
    {
        pDataParams->bPendingMacCmdDataLength = 0;

        /* load the initial IV, because we start a new MAC calculation */        
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pMACCryptoDataParams,
            phhalHw_SamAV2_Hc_AV2_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* Also set the pending MAC to 0 */
        memset(pDataParams->bPendingCmdMac, 0, sizeof(pDataParams->bPendingCmdMac)); /* PRQA S 3200 */

        /* Build the buffer to MAC */
        aTmpBuf[wValidMacData++] = pBuffer[PHHAL_HW_SAMAV2_ISO7816_CLA_POS];
        aTmpBuf[wValidMacData++] = pBuffer[PHHAL_HW_SAMAV2_ISO7816_INS_POS];
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0xFF000000) >> 24);
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0x00FF0000) >> 16);
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0x0000FF00) >> 8);
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0x000000FF) >> 0);

        /* In case of chaining detected, we need to load 0x00 for LFI */
        if ((!bLast) &&(pBuffer[PHHAL_HW_SAMAV2_ISO7816_P1_POS] == 0xAF))
        {
            aTmpBuf[wValidMacData++] = 0x00;
        }
        else
        {
            aTmpBuf[wValidMacData++] = pBuffer[PHHAL_HW_SAMAV2_ISO7816_P1_POS];
        }

        if ((!bLast) &&(pBuffer[PHHAL_HW_SAMAV2_ISO7816_P2_POS] == 0xAF))
        {
            aTmpBuf[wValidMacData++] = 0x00;
        }
        else
        {
            aTmpBuf[wValidMacData++] = pBuffer[PHHAL_HW_SAMAV2_ISO7816_P2_POS];
        }

        /* Chained commands have a LC == 0 */
        if (bLast)
        {
            aTmpBuf[wValidMacData++] = bLc + 8;
            /* Also set updated LC in original buffer */
            pBuffer[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bLc + 8;

            /* As we have updated LC, we also need to update LE... */
            if ((!bLcPresent)&&(bLePresent))
            {
                aTmpBuf[wValidMacData++] = bLeValue;
            }
        }
        else
        {
            aTmpBuf[wValidMacData++] = 0;
            /* we definitively have had LC in here */
        }

        pDataParams->bPendingMacCmdDataLength = 0;
    }
    else
    {
        /* Update LC in case of Last frame */
        if (bLast)
        {
            pBuffer[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bLc + 8;
        }

        /* Copy pending data */
        memcpy(aTmpBuf, pDataParams->bPendingMacCmdData, (uint16_t)pDataParams->bPendingMacCmdDataLength);  /* PRQA S 3200 */
        wValidMacData = pDataParams->bPendingMacCmdDataLength;
        pDataParams->bPendingMacCmdDataLength = 0;

        /* Load pending command MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pMACCryptoDataParams,
            pDataParams->bPendingCmdMac,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Start MACing Process */
    /* Now recopy the remaining data into the aTmpBuf in case of we have at least 5 bytes in the buffer */
    wHelper = 16 - wValidMacData;

    /* The helper should not be bigger than bLc (also bLc = 0 is covered) */
    if (wHelper > bLc)
    {
        wHelper = bLc;
    }

    memcpy(&aTmpBuf[wValidMacData], &pBuffer[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1], (uint16_t)wHelper);  /* PRQA S 3200 */
    wValidMacData = wValidMacData + wHelper;

    /* If we have a complete pending block, we can always use it. */
    if (wValidMacData == 16)
    {
        /* Do we have remaining data? */
        if ((bLc > wHelper) || (bLePresent))
        {
            /* Switch to CMAC mode without padding */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pMACCryptoDataParams,
                PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT,
                aTmpBuf, 
                wValidMacData, 
                pDataParams->bPendingCmdMac, 
                &bMacLength));

            wValidMacData = 0;

            /* now we calculate all blocks but the last one*/
            wValidMacData = ((uint16_t)bLc - wHelper);
            /* calculate pending data of last block */
            bLc = (uint8_t)(wValidMacData % 16);
            wValidMacData = (wValidMacData - bLc);

            /* skip MACing of the last block. */
            if (bLc==0 && !bLePresent)
            {
                if (wValidMacData >= 16)
                {
                    wValidMacData -= 16;
                }
                bLc += 16;
            }

            /* If we have data, we can now MAC it */
            if (wValidMacData)
            {
                /* we have remaining data */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pMACCryptoDataParams, 
                    PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT,
                    &pBuffer[wHelper + PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH], 
                    wValidMacData, 
                    pDataParams->bPendingCmdMac, 
                    &bMacLength));
            }

            /* Recopy the last chunk into the tmp Buffer */
            memcpy(aTmpBuf, &pBuffer[wHelper + PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wValidMacData], (uint16_t)bLc);  /* PRQA S 3200 */
            wValidMacData = bLc;            
        }
    }

    /* Now let's distinguish, what to do with the pending data */
    if (bLast)
    {
        /* Do we need to append Le?*/
        if ((bLcPresent)&&(bLePresent))
        {
            if (wValidMacData >= 16)
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
            }
            aTmpBuf[wValidMacData++] = bLeValue;
        }

        /* Switch to CMAC mode with padding*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pMACCryptoDataParams,
            PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST,
            aTmpBuf, 
            wValidMacData, 
            pDataParams->bPendingCmdMac, 
            &bMacLength));

        /* we have to truncate the MAC*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_TruncateMacBuffer(pDataParams->bPendingCmdMac, &bMacLength));

        /* Append MAC at end of buffer */
        if ((bLcPresent) && (bLePresent))
        {
            /* Le is still on the correct position, so copy mac and move Le */    
            memcpy(&pBuffer[wTxLength - 1], pDataParams->bPendingCmdMac, (uint16_t)bMacLength);  /* PRQA S 3200 */
            wTxLength = wTxLength + bMacLength;
            pBuffer[wTxLength - 1] = bLeValue;
        }
        else if (bLcPresent)
        {
            /* Before, there was no Lc byte - this is newly introduced Le needs to be recopied*/
            memcpy(&pBuffer[wTxLength], pDataParams->bPendingCmdMac, (uint16_t)bMacLength);  /* PRQA S 3200 */
            wTxLength = wTxLength + bMacLength;
        }
        else if (bLePresent)
        {
            /* Le is still on the correct position, so copy mac and move Le */    
            memcpy(&pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH], pDataParams->bPendingCmdMac, (uint16_t)bMacLength);  /* PRQA S 3200 */
            wTxLength = PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + bMacLength + 1;
            pBuffer[wTxLength - 1] = bLeValue;
        }
        else
        {
            /* We do not have Le or Lc before, now we have Lc */
            memcpy(&pBuffer[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH], pDataParams->bPendingCmdMac, (uint16_t)bMacLength);  /* PRQA S 3200 */
            wTxLength = PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + bMacLength;
        }
    }
    else
    {
        /* Setup pending data*/
        memcpy(pDataParams->bPendingMacCmdData, aTmpBuf, (uint16_t)wValidMacData);  /* PRQA S 3200 */
        pDataParams->bPendingMacCmdDataLength = (uint8_t)wValidMacData;
    }

    *pTxLength = wTxLength;
    /* End MACing Process */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_VerifyRemoveMac(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint8_t * pBuffer,
    uint16_t wRxLength, 
    uint16_t * pRxLength,
    uint8_t bFirst,
    uint8_t bLast
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bMacLength;
    uint8_t     PH_MEMLOC_REM aTmpBuf[16];
    uint16_t    PH_MEMLOC_REM wValidMacData = 0;
    uint16_t    PH_MEMLOC_REM wHelper;
    uint16_t    PH_MEMLOC_REM wPayloadLength;
    uint8_t     PH_MEMLOC_REM bOldPendingRespDataLength = 0;

    /* Begin Checks */
    memset(aTmpBuf, 0, 16); /* PRQA S 3200 */

    /* In case of chaining the last 16 bytes of the rx data could be padding or MAC data. */
    if (wRxLength>2 && ((pDataParams->bResponseChaining == PHHAL_HW_SAMAV2_HC_AV2_CHAINING) || (!bFirst && bLast)))
    {
        if (bFirst)
        {
            pDataParams->bPendingRespDataLength = 0;
        }
        else
        {
            memcpy(aTmpBuf, pDataParams->bPendingRespData, pDataParams->bPendingRespDataLength);  /* PRQA S 3200 */
            bOldPendingRespDataLength = pDataParams->bPendingRespDataLength;
        }

        if (!bLast)
        {
            if ((wRxLength-2) >= 16) 
            {
                pDataParams->bPendingRespDataLength = 16;
            }
            else
            {
                pDataParams->bPendingRespDataLength=(uint8_t) (wRxLength-2);
            }

            /* if the response is only MACed skip MACing of the last 8 bytes if it is not the last frame */
            memcpy(pDataParams->bPendingRespData, &pBuffer[(wRxLength-2) - pDataParams->bPendingRespDataLength], pDataParams->bPendingRespDataLength);  /* PRQA S 3200 */
            memcpy(&pBuffer[(wRxLength-2) - pDataParams->bPendingRespDataLength], &pBuffer[(wRxLength-2)], PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH);  /* PRQA S 3200 */
            wRxLength = wRxLength - (uint16_t) pDataParams->bPendingRespDataLength;
        }

        if (!bFirst && bOldPendingRespDataLength)
        {
            memmove(&pBuffer[bOldPendingRespDataLength], pBuffer, wRxLength);   /* PRQA S 3200 */
            memcpy(pBuffer, aTmpBuf, bOldPendingRespDataLength);  /* PRQA S 3200 */
            wRxLength = wRxLength + (uint16_t) bOldPendingRespDataLength;
        }

        if (bLast)
        {
            pDataParams->bPendingRespDataLength = 0;
        }
    }

    *pRxLength = 0;

    if (bLast)
    {
        /* Received length needs to be at least 10 bytes or 2 bytes in case of Tx Chaining! */
        if (wRxLength < (PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH + 8 /*MAC*/))
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
        wPayloadLength  = (wRxLength - (PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH + 8 /*MAC*/));        
    }
    else
    {
        wPayloadLength  = (wRxLength - (PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH));
    }

    /* Start UnMACing Process */
    if (bFirst)
    {
        pDataParams->bPendingMacRespDataLength = 0;

        /* load the InitializationVector, because we start a new MAC calculation */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(pDataParams->pMACCryptoDataParams,
            phhalHw_SamAV2_Hc_AV2_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* Also set the pending MAC to 0 */
        memset(pDataParams->bPendingRespMac, 0, sizeof(pDataParams->bPendingRespMac)); /* PRQA S 3200 */

        /* calculate the MAC according to the response pMacedBuffer */
        aTmpBuf[wValidMacData++] = pBuffer[wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH];
        /* In case of chaining detected, we need to load 0x00 */
        if ((pBuffer[wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH] == 0x90) &&(pBuffer[wRxLength - 1] == 0xAF))
        {
            aTmpBuf[wValidMacData++] = 0x00;
        }else
        {
            aTmpBuf[wValidMacData++] =  pBuffer[wRxLength - 1];
        }
        aTmpBuf[wValidMacData++] =    (uint8_t)((pDataParams->Cmd_Ctr & 0xFF000000) >> 24);
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0x00FF0000) >> 16);
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0x0000FF00) >> 8);
        aTmpBuf[wValidMacData++] = (uint8_t)((pDataParams->Cmd_Ctr & 0x000000FF) >> 0);
    }
    else
    {
        /* Get pending data*/
        memcpy(aTmpBuf, pDataParams->bPendingMacRespData, (uint16_t)pDataParams->bPendingMacRespDataLength);  /* PRQA S 3200 */
        wValidMacData = pDataParams->bPendingMacRespDataLength;
        pDataParams->bPendingMacRespDataLength = 0;

        /* Load pending response MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pMACCryptoDataParams,
            pDataParams->bPendingRespMac,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));
    }

    /* Now recopy the remaining data into the aTmpBuf in case of we have user data */
    wHelper = 16 - wValidMacData;
    if (wPayloadLength)
    {
        if (wHelper > wPayloadLength)
        {
            wHelper = wPayloadLength;
            wPayloadLength = 0;
        }
        else
        {
            /* wHelper is ok */
            wPayloadLength = wPayloadLength - wHelper;
        }
    }
    else
    {
        wHelper = 0;
    }

    memcpy(&aTmpBuf[wValidMacData], pBuffer, (uint16_t)wHelper);  /* PRQA S 3200 */

    wValidMacData = wValidMacData + wHelper;

    if (wValidMacData == 16 && wPayloadLength != 0)
    {
        /* Switch to CMAC mode without padding*/        
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pMACCryptoDataParams,
            PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT,
            aTmpBuf, 
            wValidMacData, 
            pDataParams->bPendingRespMac, 
            &bMacLength));

        /* Now add everything but the last block */
        /* now we calculate all blocks but the last one*/
        wValidMacData = wPayloadLength;

        /* calculate pending data of last block */
        wPayloadLength = (wValidMacData % 16);
        if ((wValidMacData >= 16) && (wPayloadLength == 0))
        {
            wValidMacData = (wValidMacData - 16); 
            wPayloadLength = 16;
        }
        else
        {
            wValidMacData = (wValidMacData - wPayloadLength);
        }

        /* If we have data, we can now MAC it */
        if (wValidMacData)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pMACCryptoDataParams, 
                PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT,
                &pBuffer[wHelper], 
                wValidMacData, 
                pDataParams->bPendingRespMac, 
                &bMacLength));
        }

        /* Recopy the last chunk into the tmp Buffer */
        memcpy(aTmpBuf, &pBuffer[wHelper + wValidMacData], (uint16_t)wPayloadLength);  /* PRQA S 3200 */
        wValidMacData = wPayloadLength;            
    }

    /* Last block - Verify MAC */
    if (bLast)
    {
        pDataParams->bPendingMacRespDataLength  = 0;

        /* CMAC mode with padding*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pMACCryptoDataParams,
            PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST, 
            aTmpBuf, 
            wValidMacData, 
            pDataParams->bPendingRespMac, 
            &bMacLength));

        /* we have to truncate the MAC*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_TruncateMacBuffer(pDataParams->bPendingRespMac, &bMacLength));

        if ((bMacLength != 8) || (wRxLength < (PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH + bMacLength)))
        {
            return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
        }

        /* compare the MACed in response with the calculated MAC*/
        if (memcmp(pDataParams->bPendingRespMac, &pBuffer[wRxLength - (PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH + bMacLength)], bMacLength))
        {
            return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
        }

        /* now, we can remove the MAC*/
        *pRxLength = wRxLength - bMacLength;

        /* Reorder SW1 SW2 */
        pBuffer[wRxLength - (PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH + bMacLength)] = pBuffer[wRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH];
        pBuffer[wRxLength - (1 + bMacLength)] = pBuffer[wRxLength - 1];
    }
    else
    {
        /* Setup pending data*/
        memcpy(pDataParams->bPendingMacRespData, aTmpBuf, (uint16_t)wValidMacData);  /* PRQA S 3200 */
        pDataParams->bPendingMacRespDataLength = (uint8_t)wValidMacData;

        /* End UnMACing Process */
        *pRxLength = wRxLength;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_InitAndLoadIV(phhalHw_SamAV2_DataParams_t * pDataParams, uint8_t* pIV, uint8_t encryptionIV)
{
    phStatus_t statusTmp;
    /*    IV(16 bytes) = 0x1|0x1|0x1|0x1|cmd_ctr[0..3]|cmd_ctr[0..3]|cmd_ctr[0..3] */
    /* build the IV */
    uint8_t i;

    /* Load null keys to encrypt IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        phhalHw_SamAV2_Hc_AV2_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    if (encryptionIV)
    {
        for (i = 0; i < 4; i++)
            pIV[i] = 0x01;
        for ( i = 1; i < 4; i++)
        {
            pIV[4*i] = (uint8_t)((pDataParams->Cmd_Ctr & 0xFF000000) >> 24);
            pIV[4*i+1] = (uint8_t)((pDataParams->Cmd_Ctr & 0x00FF0000) >> 16);
            pIV[4*i+2] = (uint8_t)((pDataParams->Cmd_Ctr & 0x0000FF00) >> 8);
            pIV[4*i+3] = (uint8_t)((pDataParams->Cmd_Ctr & 0x000000FF) >> 0);
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
            pIV[i] = 0x02;
        for ( i = 1; i < 4; i++)
        {
            pIV[4*i] = (uint8_t)((pDataParams->Cmd_Ctr & 0xFF000000) >> 24);
            pIV[4*i+1] = (uint8_t)((pDataParams->Cmd_Ctr & 0x00FF0000) >> 16);
            pIV[4*i+2] = (uint8_t)((pDataParams->Cmd_Ctr & 0x0000FF00) >> 8);
            pIV[4*i+3] = (uint8_t)((pDataParams->Cmd_Ctr & 0x000000FF) >> 0);
        }
    }

    /* Encrypt IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pENCCryptoDataParams,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_FIRST,
        pIV,
        16,
        pIV));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GenerateAuthEncKey(phhalHw_SamAV2_DataParams_t * pDataParams,
                                                        uint8_t keyType,        /**< [In] the key type for the SamAV1 (DES, 3K3DES, AES128 or AES192)*/
                                                        uint8_t * pRnd1Data,    /**< [In] pointer to Rnd1 data*/                              
                                                        uint8_t * pRnd2Data,    /**< [In] pointer to Rnd2 data*/                              
                                                        uint8_t * pSessionKey,    /**< [Out] pointer to the Session Host key */                              
                                                        uint8_t * pKeyLength    /**< [Out] length of the Session Host key */                              
                                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM SV1a[16];
    uint8_t     PH_MEMLOC_REM SV1b[16];
    uint8_t     PH_MEMLOC_REM Kxea[16];
    uint8_t     PH_MEMLOC_REM Kxeb[16];
    uint8_t     PH_MEMLOC_COUNT count;

    memcpy(&SV1a[0], &pRnd1Data[7], 5);     /* PRQA S 3200 */
    memcpy(&SV1a[5], &pRnd2Data[7], 5);     /* PRQA S 3200 */
    memcpy(&SV1a[10], &pRnd1Data[0], 5);    /* PRQA S 3200 */

    for (count = 0; count <= 4; ++count)
    {
        SV1a[count + 10] ^= pRnd2Data[count];
    }

    if (keyType == PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        SV1a[15] = PHHAL_HW_SAMAV2_SECMSG_HC_AUTH_ENC_SV1A_AES128;

        /* Load initial IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pMACCryptoDataParams,
            phhalHw_SamAV2_Hc_AV2_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* Key already loaded externally */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV1a, 
            16, 
            pSessionKey));

        *pKeyLength = 16;
    }
    else if (keyType == PH_CRYPTOSYM_KEY_TYPE_AES192)
    {
        memcpy(&SV1b[0], &pRnd1Data[6], 5);     /* PRQA S 3200 */
        memcpy(&SV1b[5], &pRnd2Data[6], 5);     /* PRQA S 3200 */
        memcpy(&SV1b[10], &pRnd1Data[1], 5);    /* PRQA S 3200 */
        for (count = 1; count <= 5; ++count)
        {
            SV1b[count + 9] ^= pRnd2Data[count];  
        }
        SV1a[15] = PHHAL_HW_SAMAV2_SECMSG_HC_AUTH_ENC_SV1A_AES192;
        SV1b[15] = PHHAL_HW_SAMAV2_SECMSG_HC_AUTH_ENC_SV1B_AES192;

        /* Load initial IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pMACCryptoDataParams,
            phhalHw_SamAV2_Hc_AV2_FirstIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE));

        /* Key Loaded already externally */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV1a, 
            16, 
            Kxea));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC,  
            SV1b,  
            16, 
            Kxeb));

        memcpy(pSessionKey, Kxea, 16);  /* PRQA S 3200 */
        for (count = 0; count <= 7; ++count)
        {
            pSessionKey[count+8] ^= Kxeb[count];
        }
        memcpy(&pSessionKey[16], &Kxeb[8], 8); /* PRQA S 3200 */
        *pKeyLength = 24;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GenerateSessionKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,            
    uint8_t keyType,
    uint8_t * pRndAData,
    uint8_t * pRndBData,
    uint8_t * pEncSessionKey,
    uint8_t * pMacSessionKey,
    uint8_t * pKeyLength
    )
{
    phStatus_t statusTmp;
    uint8_t SV1a[16], SV2a[16];
    uint8_t SV1b[16], SV2b[16];
    uint8_t KaEnc[16], KbEnc[16];
    uint8_t KaMac[16], KbMac[16];
    uint8_t count;

    memcpy(&SV1a[0], &pRndAData[11], 5); /* PRQA S 3200 */
    memcpy(&SV1a[5], &pRndBData[11], 5); /* PRQA S 3200 */
    memcpy(&SV1a[10], &pRndAData[4], 5); /* PRQA S 3200 */
    for (count = 4; count <= 9; ++count)
    {
        SV1a[count + 6] ^= pRndBData[count];
    }

    memcpy(&SV2a[0], &pRndAData[7], 5); /* PRQA S 3200 */
    memcpy(&SV2a[5], &pRndBData[7], 5); /* PRQA S 3200 */
    memcpy(&SV2a[10], &pRndAData[0], 5); /* PRQA S 3200 */
    for (count = 0; count <= 5; ++count)
    {
        SV2a[count + 10] ^= pRndBData[count];
    }

    /* Load initial IV and do not change it after CBC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pMACCryptoDataParams,
        phhalHw_SamAV2_Hc_AV2_FirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* (Key already loaded) */
    if (keyType == PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        SV1a[15] = 0x81;
        SV2a[15] = 0x82;

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV1a,  
            16, 
            pEncSessionKey));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV2a, 
            16, 
            pMacSessionKey));

        *pKeyLength = 16;
    }
    else if (keyType == PH_CRYPTOSYM_KEY_TYPE_AES192)
    {
        SV1a[15] = 0x83;
        SV2a[15] = 0x85;
        memcpy(&SV1b[0], &pRndAData[10], 5); /* PRQA S 3200 */
        memcpy(&SV1b[5], &pRndBData[10], 5); /* PRQA S 3200 */
        memcpy(&SV1b[10], &pRndAData[5], 5); /* PRQA S 3200 */
        for (count = 5; count <= 10; ++count)
        {
            SV1b[count + 5] ^= pRndBData[count];
        }

        SV1b[15] = 0x84;
        memcpy(&SV2b[0], &pRndAData[6], 5); /* PRQA S 3200 */
        memcpy(&SV2b[5], &pRndBData[6], 5); /* PRQA S 3200 */
        memcpy(&SV2b[10], &pRndAData[1], 5); /* PRQA S 3200 */
        for (count = 1; count <= 6; ++count)
        {
            SV2b[count + 9] ^= pRndBData[count];
        }
        SV2b[15] = 0x86;

        *pKeyLength = 24;

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV1a, 
            16, 
            KaEnc));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV1b, 
            16, 
            KbEnc));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV2a, 
            16, 
            KaMac));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pMACCryptoDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC, 
            SV2b, 
            16, 
            KbMac));

        memcpy(pEncSessionKey, KaEnc, 16); /* PRQA S 3200 */
        for (count = 0; count <= 7; ++count)
        {
            pEncSessionKey[count + 8] ^= KbEnc[count];
        }
        memcpy(&pEncSessionKey[16], &KbEnc[8], 8); /* PRQA S 3200 */

        memcpy(pMacSessionKey, KaMac, 16); /* PRQA S 3200 */
        for (count = 0; count <= 7; ++count)
        {
            pMacSessionKey[count + 8] ^= KbMac[count];
        }
        memcpy(&pMacSessionKey[16], &KbMac[8], 8); /* PRQA S 3200 */
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GetFirstLastCommand(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t aCmd, 
    uint8_t p1, 
    uint8_t p2,
    uint8_t * bFirstCmd,
    uint8_t * bLastCmd)
{
    *bFirstCmd = PH_ON;
    *bLastCmd = PH_ON;

    /* Reset CommandChaining per default */
    if (pDataParams->bCommandChaining == PHHAL_HW_SAMAV2_HC_AV2_CHAINING)
    {
        *bFirstCmd = PH_OFF;
    }

    /* In case of response chaining no CMD counter increment is allowed as well */
    if (pDataParams->bResponseChaining == PHHAL_HW_SAMAV2_HC_AV2_CHAINING)
    {
        *bFirstCmd = PH_OFF;

        /* DESFire ReadX command needs special treatment */
        if (aCmd == PHHAL_HW_SAMAV2_CMD_DESFIRE_READ_X_INS)
        {
            pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
            *bLastCmd = PH_OFF;
        }
    }

    pDataParams->bCommandChaining = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;

    /* Is the current command a chained one or not? */
    if (p1 == PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME)
    {
        switch (aCmd)
        {
        case PHHAL_HW_SAMAV2_CMD_DECIPHER_OFFLINE_DATA_INS:
        case PHHAL_HW_SAMAV2_CMD_ENCIPHER_OFFLINE_DATA_INS:
        case PHHAL_HW_SAMAV2_CMD_GENERATE_MAC_INS:
        case PHHAL_HW_SAMAV2_CMD_VERIFY_MAC_INS:
        case PHHAL_HW_SAMAV2_CMD_DESFIRE_WRITE_X_INS:
        case PHHAL_HW_SAMAV2_CMD_EXCHANGE_INS:
        case PHHAL_HW_SAMAV2_CMD_ENCIPHER_DATA_INS:
        case PHHAL_HW_SAMAV2_CMD_DECIPHER_DATA_INS:
            pDataParams->bCommandChaining = PHHAL_HW_SAMAV2_HC_AV2_CHAINING;
            *bLastCmd = PH_OFF;
            break;
        default:
            break;
        }
    }

    if (p2 == PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME)
    {
        switch (aCmd)
        {
        case PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_INS:
        case PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_INS:
        case PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_INS:
        case PHHAL_HW_SAMAV2_CMD_PKI_VERIFY_SIGNATURE_INS:
        case PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_INS:
        case PHHAL_HW_SAMAV2_CMD_READ_MFP:
            pDataParams->bCommandChaining = PHHAL_HW_SAMAV2_HC_AV2_CHAINING;
            *bLastCmd = PH_OFF;
            break;
        default:
            break;
        }
    }

    /*  Do we need special treatment? */
    if (*bFirstCmd)
    {    
        if (pDataParams->bAuthType == 0x01)
        {
            pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_MAC;
            pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_MAC;
        }else if (pDataParams->bAuthType == 0x02)
        {
            pDataParams->bCmdSM = (PHHAL_HW_SAMAV2_HC_AV2_MAC | PHHAL_HW_SAMAV2_HC_AV2_ENC);
            pDataParams->bRespSM = (PHHAL_HW_SAMAV2_HC_AV2_MAC | PHHAL_HW_SAMAV2_HC_AV2_ENC);
        }else
        {
            pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
            pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
        }
        switch (aCmd)
        {
        case PHHAL_HW_SAMAV2_CMD_READ_MFP:
            /* Check the CMDRESP option */
            if (p1 == PHHAL_HW_SAMAV2_CMD_READ_MFP_COMMAND)
            {
                /* Note: In SamAV2, the response of this command is neither MACed nor encrypted */
                pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            }
            else if (p1 == PHHAL_HW_SAMAV2_CMD_READ_MFP_RESPONSE)
            {   
                /* Note: In SamAV2, the command of this command is neither MACed nor encrypted */
                pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            }
            else
            {
                /* Else, nothing changed */
            }
            break;
        case PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_INS:
        case PHHAL_HW_SAMAV2_CMD_WRITE_MFP:
            if (p1 & 0x01)
            {
                /* Note: In SamAV2, the response of this command is neither MACed nor Encrypted */
                pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;                
            }
            else
            {
                /* Note: In SamAV2, the commmand of this command is neither MACed nor Encrypted */
                pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            }
            break;
        case PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_MFP:
        case PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_INS:
        case PHHAL_HW_SAMAV2_CMD_VERIFY_PROXIMITY_CHECK:
        case PHHAL_HW_SAMAV2_CMD_SELECT_VIRTUAL_CARD:
        case PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_INS:
        case PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_INS:
            if (pDataParams->bResponseChaining == PHHAL_HW_SAMAV2_HC_AV2_CHAINING_NO_SM)
            {
                /* Note: In SamAV2, the response of this command is neither MACed nor Encrypted */
                pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            }else
            {
                /* Note: In SamAV2, the commmand of this command is neither MACed nor Encrypted */
                pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            }
            break;
        case PHHAL_HW_SAMAV2_CMD_ENCIPHER_DATA_INS:
        case PHHAL_HW_SAMAV2_CMD_ENCIPHER_OFFLINE_DATA_INS:
            /* Note: In SamAV2, the response of this command is MACed only */
            pDataParams->bRespSM &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_HC_AV2_ENC;
            break;
        case PHHAL_HW_SAMAV2_CMD_DECIPHER_DATA_INS:
        case PHHAL_HW_SAMAV2_CMD_DECIPHER_OFFLINE_DATA_INS:
            /* Note: In SamAV2, the command of this command is MACed only */
            pDataParams->bCmdSM &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_HC_AV2_ENC;
            break;
        case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_INS:
            pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            break;
        case PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_INS:
            /* allow encryption for key dump in case of MAC protection. */
            if ((p2 & 0x01) && (pDataParams->bAuthType == 0x01))
            {
                pDataParams->bRespSM |= PHHAL_HW_SAMAV2_HC_AV2_ENC;
            }
            break;
        case PHHAL_HW_SAMAV2_CMD_DUMP_SESSIONKEY_INS:
        case PHHAL_HW_SAMAV2_CMD_DUMP_SECRETKEY_INS:
            /* allow encryption for key dump in case of MAC protection. */
            if ((p1 & 0x01) && (pDataParams->bAuthType == 0x01))
            {
                pDataParams->bRespSM |= PHHAL_HW_SAMAV2_HC_AV2_ENC;
            }
            break;
        case PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_PICC_INS:
            /* Note: In SamAV2, the response of this command is neither MACed nor Encrypted */
            pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            break;
        default:
            break;
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GetFirstLastResponse(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t sw1,
    uint8_t sw2,
    uint8_t * bFirstResponse,
    uint8_t * bLastResponse)
{
    *bFirstResponse = PH_ON;
    *bLastResponse = PH_ON;

    /* Reset ResponseChaining per default */
    if (pDataParams->bResponseChaining == PHHAL_HW_SAMAV2_HC_AV2_CHAINING)
    {
        *bFirstResponse = PH_OFF;
    }

    pDataParams->bResponseChaining = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;

    /* Check for 0x90AF - in case of MACing of response is enabled, we should set the chaining option*/
    if ((sw1 != 0x90) || (sw2 != 0x00))
    {
        if ((sw1 == 0x90) && (sw2 == 0xAF))
        {
            if (pDataParams->bRespSM != PHHAL_HW_SAMAV2_HC_AV2_NO_SM)
            {
                pDataParams->bResponseChaining = PHHAL_HW_SAMAV2_HC_AV2_CHAINING;
            }else
            {
                pDataParams->bResponseChaining = PHHAL_HW_SAMAV2_HC_AV2_CHAINING_NO_SM;
            }   
            *bLastResponse = PH_OFF;           
        }
        else
        {
            *bFirstResponse = PH_ON;
            pDataParams->bCommandChaining = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
