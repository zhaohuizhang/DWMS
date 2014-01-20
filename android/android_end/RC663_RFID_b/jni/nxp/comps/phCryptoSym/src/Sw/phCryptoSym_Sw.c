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
* Software specific Crypto-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.10 $
* $Date: Fri Apr 15 09:27:28 2011 $
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#ifdef NXPBUILD__PH_KEYSTORE
#include <phKeyStore.h>
#endif /* #ifdef NXPBUILD__PH_KEYSTORE */
#include <phCryptoSym.h>

#ifdef NXPBUILD__PH_CRYPTOSYM_SW

#include "phCryptoSym_Sw.h"
#include "../phCryptoSym_Int.h"
#ifdef PH_CRYPTOSYM_SW_AES
#include "phCryptoSym_Sw_Aes.h"
#endif
#ifdef PH_CRYPTOSYM_SW_DES
#include "phCryptoSym_Sw_Des.h"
#endif
#include "phCryptoSym_Sw_Int.h"

phStatus_t phCryptoSym_Sw_Init(
                               phCryptoSym_Sw_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams, 
                               void * pKeyStoreDataParams
                               )
{
    phStatus_t statusTmp;
    if (sizeof(phCryptoSym_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
    }
    PH_ASSERT_NULL (pDataParams);

    /* Init. private data */
    pDataParams->wId                    = PH_COMP_CRYPTOSYM | PH_CRYPTOSYM_SW_ID;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;

    /* Invalidate keys */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_InvalidateKey(pDataParams));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Sw_InvalidateKey(
                                        phCryptoSym_Sw_DataParams_t * pDataParams
                                        )
{
    /* Reset all the key storage */
    memset(pDataParams->pKey, 0x00, sizeof(pDataParams->pKey));  /* PRQA S 3200 */
    memset(pDataParams->pIV, 0x00, sizeof(pDataParams->pIV));    /* PRQA S 3200 */
    pDataParams->wKeyType = PH_CRYPTOSYM_KEY_TYPE_INVALID;    
    pDataParams->wKeepIV = PH_CRYPTOSYM_VALUE_KEEP_IV_OFF;

#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    pDataParams->bCMACSubKeysInitialized = PH_OFF;
    memset(pDataParams->pCMACSubKey1, 0x00, sizeof(pDataParams->pCMACSubKey1)); /* PRQA S 3200 */
    memset(pDataParams->pCMACSubKey2, 0x00, sizeof(pDataParams->pCMACSubKey2)); /* PRQA S 3200 */
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Sw_Encrypt(
                                  phCryptoSym_Sw_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  const uint8_t * pPlainBuffer,
                                  uint16_t  wBufferLength,
                                  uint8_t * pEncryptedBuffer
                                  )
{ 
    phStatus_t statusTmp;
    uint16_t wBlockSize;
    uint16_t i;
    uint8_t j;
    uint8_t * pIv;
#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE             
    uint8_t PH_CRYTOSYM_SW_FAST_RAM pHelperBuffer[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];
#endif /* PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE */

    /* Get the block size of the currently loaded key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_GetConfig(pDataParams, PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, &wBlockSize));

    /* Check that the input buffer length is a multiple of the block size; */
    if (wBufferLength % wBlockSize)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    /* Set the IV to the iv specified in the private data params */
    pIv = pDataParams->pIV;

    /*Iterate over all blocks and perform the encryption*/
    i = 0;
    while (i < wBufferLength)
    {
        /* Is the output array the same as the input array? Else we need to recopy the plaintext upfronjt */
        if (pPlainBuffer != pEncryptedBuffer)
        {
            memcpy(&pEncryptedBuffer[i],&pPlainBuffer[i], wBlockSize ); /* PRQA S 3200 */
        }

        /* In case of CBC mode, we need to perform the XOR with the previous cipher block */
        switch((uint8_t)(wOption))
        {
        case PH_CRYPTOSYM_CIPHER_MODE_CBC:
        case PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4:
            for (j = 0; j < wBlockSize; j++)
            {
                pEncryptedBuffer[j + i] ^= pIv[j];
            }
            break;
        case PH_CRYPTOSYM_CIPHER_MODE_ECB:
            /* Nothing to do here */
            break;
        default:
            /* Add additional Modes of operation in here! */
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM);   
        }

#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE
        memcpy(pHelperBuffer, &pEncryptedBuffer[i], wBlockSize); /* PRQA S 3200 */
        if ((uint8_t)wOption == PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_DecryptBlock(pDataParams, pHelperBuffer));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, pHelperBuffer));
        }

        memcpy(&pEncryptedBuffer[i], pHelperBuffer, wBlockSize); /* PRQA S 3200 */
#else
        if ((uint8_t)wOption == PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_DecryptBlock(pDataParams, &pEncryptedBuffer[i])); 
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, &pEncryptedBuffer[i])); 
        }

#endif /* PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE */

        /* do the loop dependent post processing of the data according to the used mode of operation*/
        switch((uint8_t)(wOption))
        {
        case PH_CRYPTOSYM_CIPHER_MODE_CBC:
        case PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4:
            /* we should set the IV now to the old ciphertext... */        
            pIv = &pEncryptedBuffer[i];
            break;
        case PH_CRYPTOSYM_CIPHER_MODE_ECB:
            /* Nothing to do here */
            break;
        default:
            /* Add additional Modes of operation in here! */
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM); 
        }              

        /* update the loop counter */
        i = wBlockSize + i;
    } /* end of loop over all data blocks */

    /* do the final update of the IV according to the keep IV setting. */
    if ((pDataParams->wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON) || (wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        memcpy(pDataParams->pIV, pIv, wBlockSize); /* PRQA S 3200 */
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);   
}

phStatus_t phCryptoSym_Sw_Decrypt(
                                  phCryptoSym_Sw_DataParams_t * pDataParams,
                                  uint16_t wOption, 
                                  const uint8_t * pEncryptedBuffer,
                                  uint16_t  wBufferLength,
                                  uint8_t * pPlainBuffer
                                  )
{
    phStatus_t statusTmp;
    uint16_t wBlockSize;
    uint16_t i;
    uint8_t j;
    uint8_t pIv[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE]; /* AES block size is the maximum available block size */
    uint8_t pNextIv[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE]; /* AES block size is the maximum available block size */
#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE             
    uint8_t PH_CRYTOSYM_SW_FAST_RAM pHelperBuffer[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];
#endif /* PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE */

    /* Clear buffer for QAC */
    memset(pNextIv, 0, PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE); /* PRQA S 3200 */

    /* Get the block size of the currently loaded key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_GetConfig(pDataParams, PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, &wBlockSize));

    /* Get IV */
    memcpy(pIv, pDataParams->pIV, wBlockSize); /* PRQA S 3200 */    

    /* Check that the input buffer length is a multiple of the block size; */
    if (wBufferLength % wBlockSize)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    /*Iterate over all blocks and perform the decryption*/
    i = 0;
    while( i < wBufferLength)
    {           

        /* Is the output array the same as the input array? Else we need to recopy the encrypted data into the plaintext data buffer first. */
        if (pEncryptedBuffer != pPlainBuffer)
        {
            memcpy(&pPlainBuffer[i],&pEncryptedBuffer[i], wBlockSize ); /* PRQA S 3200 */
        } 

        /* perform pre processing of the data according to the selected mode of operation. */
        switch((uint8_t)(wOption))
        {
        case PH_CRYPTOSYM_CIPHER_MODE_CBC:
        case PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4:
            /* In case of CBC mode, we need to remember the previous encrytped block */
            memcpy(pNextIv,&pEncryptedBuffer[i], wBlockSize ); /* PRQA S 3200 */
            break;
        case PH_CRYPTOSYM_CIPHER_MODE_ECB:
            /* Nothing to do! */
            break;
        default:
            /* Add additional Modes of operation in here! */
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM); 
        }        

#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE
        memcpy(pHelperBuffer, &pPlainBuffer[i], wBlockSize); /* PRQA S 3200 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_DecryptBlock(pDataParams, pHelperBuffer));  
        memcpy(&pPlainBuffer[i], pHelperBuffer, wBlockSize); /* PRQA S 3200 */
#else
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_DecryptBlock(pDataParams,&pPlainBuffer[i]));

#endif /* PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE */        

        /* perform post processing of the data according to the selected mode of operation. */
        switch((uint8_t)(wOption))
        {
        case PH_CRYPTOSYM_CIPHER_MODE_CBC:
        case PH_CRYPTOSYM_CIPHER_MODE_CBC_DF4:
            /* In case of CBC mode, we need to perform the XOR with the previous cipher block */
            for (j=0; j < wBlockSize; ++j)
            {
                pPlainBuffer[j + i] ^= pIv[j];
            }
            /* Get IV */
            memcpy(pIv, pNextIv, wBlockSize ); /* PRQA S 3200 */
            break;
        case PH_CRYPTOSYM_CIPHER_MODE_ECB:
            /* Nothing to do here */
            break;
        default:
            /* Add additional Modes of operation in here! */
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM); 
        }           

        /* update the loop counter */
        i = wBlockSize + i;
    }

    if ((pDataParams->wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON) || (wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Update IV */
        memcpy(pDataParams->pIV, pIv, wBlockSize ); /* PRQA S 3200 */
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);   
}

phStatus_t phCryptoSym_Sw_CalculateMac(
                                       phCryptoSym_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       const uint8_t * pData,
                                       uint16_t  wDataLength,
                                       uint8_t * pMac,
                                       uint8_t * pMacLength
                                       )
{


    phStatus_t statusTmp;
    uint16_t wBlockSize;
    uint16_t i;
    uint8_t j;
    uint8_t bPaddingLength;
    uint8_t bLastBlock[16];
    uint8_t * pIv;
#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE             
    uint8_t PH_CRYTOSYM_SW_FAST_RAM pHelperBuffer[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];
#endif /* PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE */
#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    uint8_t * pSubKey1 = pDataParams->pCMACSubKey1;
    uint8_t * pSubKey2 = pDataParams->pCMACSubKey2;
#else
    uint8_t pSubKey1[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];
    uint8_t pSubKey2[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];

    memset(pSubKey1, 0x00, sizeof(pSubKey1)); /* PRQA S 3200 */
    memset(pSubKey2, 0x00, sizeof(pSubKey2)); /* PRQA S 3200 */
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */

    /* Clear MAC length */
    *pMacLength = 0;

    /* Clear the last block array */
    memset(bLastBlock, 0, sizeof(bLastBlock)); /* PRQA S 3200 */

    /* Get the block size of the currently loaded key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_GetConfig(pDataParams, PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, &wBlockSize));

    /* In case of a first block and in case of KEEP_IV is not set, the IV has to be cleared. */
    if ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT) ||
        (pDataParams->wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON))
    {
        /* better leave the IV */
    }else
    {
        memset(pDataParams->pIV, 0x00, wBlockSize); /* PRQA S 3200 */
    }

    /* Now we may start with  MAC calculation */

    /*Let's find out whether we should complete the MAC or if this is just an intermediate MAC calculation */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        /* This is just an intermediate MAC */

        /* In this case we do not allow incomplete blocks. */
        if (wDataLength % wBlockSize)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
    }
    else
    {
        switch((uint8_t)(wOption))
        {
        case PH_CRYPTOSYM_MAC_MODE_CMAC:
#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
            /* Do we need to compute the sub keys? */
            if (pDataParams->bCMACSubKeysInitialized == 0x00)
            {
                /* start with sub key generation */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_CMAC_GenerateK1K2(pDataParams, pSubKey1, pSubKey2));

            }
#else
            /* Always perform with sub key generation */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_CMAC_GenerateK1K2(pDataParams, pSubKey1, pSubKey2));
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */
            break;
        case PH_CRYPTOSYM_MAC_MODE_CBCMAC:
            /* Nothing to do! */
            break;
        default:
            /* Add additional Modes of operation in here! */
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM); 
        }

        /* Get number of bytes in last block */
        bPaddingLength = (uint8_t)(wDataLength % wBlockSize);

        /* do we have incomplete blocks? */
        if ((bPaddingLength) || (wDataLength == 0x0000))
        {
            /* Update wDataLength, last block is in other array */
            wDataLength = wDataLength - bPaddingLength; /* PRQA S 0291 */

            memcpy(bLastBlock, &pData[wDataLength], bPaddingLength); /* PRQA S 3200 */
            /* Apply padding byte*/
            bLastBlock[bPaddingLength] = 0x80;
            /* pad with zeros not necessary, memset done upfront*/

            if ((uint8_t)wOption == PH_CRYPTOSYM_MAC_MODE_CMAC)
            {

                /* XOR with K2, as we have an icomplete block */
                for (j=0;j<wBlockSize; j++)
                {
                    bLastBlock[j] ^= pSubKey2[j];
                }
            }
        }else
        {
            /* Update wDataLength, last block is in other array */
            wDataLength = wDataLength - wBlockSize;
            /* Copy whole block into bLastBlock */
            memcpy(bLastBlock, &pData[wDataLength], wBlockSize); /* PRQA S 3200 */

            if ((uint8_t)wOption == PH_CRYPTOSYM_MAC_MODE_CMAC)
            {
                /* XOR with K1, as we have a complete block */
                for (j=0;j<wBlockSize; j++)
                {
                    bLastBlock[j] ^= pSubKey1[j];
                }
            }
        }
    }


    /* Set the IV to the iv specified in the private data params */
    pIv = pDataParams->pIV;

    /*Iterate over all blocks and perform the CBC encryption*/
    i=0;
    while(i < wDataLength)
    {        
        /* perform the XOR with the previous cipher block */                
        for (j = 0; j < wBlockSize; j++)
        {
            /* Note: after one round pIv == pMac */
#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE
            pHelperBuffer[j] = pIv[j] ^ pData[i+j];
#else
            pMac[j] = pIv[j] ^ pData[i+j];
#endif
        }                               

#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, pHelperBuffer)); 
        memcpy(pMac, pHelperBuffer, wBlockSize); /* PRQA S 3200 */
#else
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, pMac));  
#endif

        /* set pIv to last cipher block*/
        pIv = pMac;      

        /* update the loop counter */
        i = wBlockSize + i;
    } /* end of loop over all data blocks */      

    /* If we have a complete MAC, lets encrypt the last block */
    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Encrypt last block. */
        /* perform the XOR with the previous cipher block */                
        for (j = 0; j < wBlockSize; j++)
        {
            /* Note: after one round pIv == pMac */
#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE
            pHelperBuffer[j] = pIv[j] ^ bLastBlock[j];
#else
            pMac[j] = pIv[j] ^ bLastBlock[j];
#endif
        }                               
#ifdef PH_CRYPTOSYM_SW_USE_8051_DATA_STORAGE
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, pHelperBuffer));
        memcpy(pMac, pHelperBuffer, wBlockSize ); /* PRQA S 3200 */  
#else
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, pMac));    
#endif        

    }    

    /* do the final update of the IV according to the settings */
    if ((pDataParams->wKeepIV == PH_CRYPTOSYM_VALUE_KEEP_IV_ON) || (wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        memcpy(pDataParams->pIV, pMac, wBlockSize ); /* PRQA S 3200 */
    }else
    {
        /* Clear the IV for security reasons */
        memset(pDataParams->pIV, 0, wBlockSize ); /* PRQA S 3200 */
    }

#ifdef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    /* Clear key arrays */
    memset(pSubKey1, 0x00, sizeof(pSubKey1)); /* PRQA S 3200 */
    memset(pSubKey2, 0x00, sizeof(pSubKey2)); /* PRQA S 3200 */
#endif

    *pMacLength = (uint8_t) wBlockSize;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);   
}

phStatus_t phCryptoSym_Sw_LoadIv(  
                                 phCryptoSym_Sw_DataParams_t * pDataParams,
                                 const uint8_t * pIV,   
                                 uint8_t bIVLength
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wBlockSize;

    /* Get the block size of the currently loaded key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_GetConfig(pDataParams, PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, &wBlockSize));

    /* Check block-size */
    if (bIVLength != wBlockSize)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM); 
    }

    /* Update IV */
    memcpy(pDataParams->pIV, pIV,wBlockSize); /* PRQA S 3200 */        

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Sw_LoadKey(  
                                  phCryptoSym_Sw_DataParams_t * pDataParams,
                                  uint16_t wKeyNo,
                                  uint16_t wKeyVersion,
                                  uint16_t wKeyType
                                  )
{
#ifdef NXPBUILD__PH_KEYSTORE
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF aKey[PH_CRYPTOSYM_AES256_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyTypeStorage;

    /* Not possible without keystore */
    if (pDataParams->pKeyStoreDataParams == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_CRYPTOSYM);
    }

    /* Retrieve key settings */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &wKeyTypeStorage));

    /* KeyType should match */
    if (wKeyType != wKeyTypeStorage)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    /* Finally load the key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_LoadKeyDirect(pDataParams, aKey, wKeyType));

    /* For security reasons */
    memset(aKey, 0x00, sizeof(aKey)); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
#else
    /* satisfy compiler */
    if (pDataParams || wKeyNo || wKeyVersion || wKeyType);
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_CRYPTOSYM);
#endif /* NXPBUILD__PH_KEYSTORE */   
}

phStatus_t phCryptoSym_Sw_LoadKeyDirect(  
                                        phCryptoSym_Sw_DataParams_t * pDataParams,
                                        const uint8_t * pKey,
                                        uint16_t wKeyType
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Clear existing key */
    memset(pDataParams->pKey, 0x00, sizeof(pDataParams->pKey)); /* PRQA S 3200 */

#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    /* Disable the CMAC calculated Flag */
    pDataParams->bCMACSubKeysInitialized = PH_OFF;
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */   

    switch(wKeyType)
    {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_KeyExpansion(
            pDataParams, 
            pKey,
            PH_CRYPTOSYM_AES128_KEY_SIZE >> 2, 
            (PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_128 + 1) << 2));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:        
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_KeyExpansion(
            pDataParams, 
            pKey,
            PH_CRYPTOSYM_AES192_KEY_SIZE >> 2, 
            (PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_192 + 1) << 2));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:        
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_KeyExpansion(
            pDataParams, 
            pKey,
            PH_CRYPTOSYM_AES256_KEY_SIZE >> 2, 
            (PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_256 + 1) << 2));
        break;
#endif /* PH_CRYPTOSYM_SW_AES */ 
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_KeyInit(pDataParams, pKey, 1));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_KeyInit(pDataParams, pKey, 2));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_KeyInit(pDataParams, pKey, 3));
        break;
#endif /* PH_CRYPTOSYM_SW_DES */ 
    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM); 
    }

    /* Update global KeyType */
    pDataParams->wKeyType = wKeyType;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Sw_DiversifyKey(  
                                       phCryptoSym_Sw_DataParams_t * pDataParams, 
                                       uint16_t wOption,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVersion,
                                       uint8_t * pDivInput,
                                       uint8_t  bLenDivInput, 
                                       uint8_t * pDiversifiedKey
                                       )
{
#ifdef NXPBUILD__PH_KEYSTORE
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aKey[PH_CRYPTOSYM_AES256_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;

    /* Not possible without keystore */
    if (pDataParams->pKeyStoreDataParams == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_CRYPTOSYM);
    }

    /* Parameter check */
    if (((wOption & PH_CRYPTOSYM_DIV_MODE_MASK) != PH_CRYPTOSYM_DIV_MODE_DESFIRE) &&
        ((wOption & PH_CRYPTOSYM_DIV_MODE_MASK) != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    /* Retrieve key from keystore */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &wKeyType));

    /* Perform diversification */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_DiversifyDirectKey(
        pDataParams,
        wOption,
        aKey,
        wKeyType,
        pDivInput,
        bLenDivInput,
        pDiversifiedKey));

    /* For security reasons */
    memset(aKey, 0x00, sizeof(aKey)); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
#else
    /* satisfy compiler */
    if (pDataParams || wKeyNo || wKeyVersion ||pDiversifiedKey || bLenDivInput ||pDivInput ||wOption);
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_CRYPTOSYM);
#endif  /* NXPBUILD__PH_KEYSTORE */ 
}

phStatus_t phCryptoSym_Sw_DiversifyDirectKey(  
    phCryptoSym_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pKey,    
    uint16_t wKeyType, 
    uint8_t * pDivInput, 
    uint8_t bLenDivInput,
    uint8_t * pDiversifiedKey
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF abDivData[PH_CRYPTOSYM_SW_KDIV_MFP_DIVLENGTH_AES_MAX + 1];
    uint8_t     PH_MEMLOC_BUF abMac[16];
    uint8_t     PH_MEMLOC_REM bTmpLength;
    uint16_t    PH_MEMLOC_REM wKeyVersion;

    /* Load the Key to diversify */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_LoadKeyDirect(pDataParams, pKey, wKeyType));

    /* Check diversification method */
    switch (wOption & PH_CRYPTOSYM_DIV_MODE_MASK)
    {
    case PH_CRYPTOSYM_DIV_MODE_DESFIRE:
        switch (wKeyType)
        {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        /* Parameter check */
        if (bLenDivInput != PH_CRYPTOSYM_AES128_KEY_SIZE)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }

        /* Use the DivData as IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_LoadIv(pDataParams, pDivInput, bLenDivInput));

        /* Retrieve KeySize */
        bLenDivInput = (uint8_t)phCryptoSym_GetKeySize(wKeyType);

        /* Copy Key to temporary diversification data */
        memcpy(abDivData, pKey, bLenDivInput); /* PRQA S 3200 */

        /* Apply padding if neccessary */
        if ((bLenDivInput % PH_CRYPTOSYM_AES_BLOCK_SIZE) != 0)
        {
            memset(&abDivData[PH_CRYPTOSYM_AES192_KEY_SIZE], 0x00, 8); /* PRQA S 3200 */
            bTmpLength = PH_CRYPTOSYM_AES192_KEY_SIZE + 8;
        }
        else
        {
            bTmpLength = bLenDivInput;
        }

        /* Perform Encryption */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Encrypt(
            pDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
            abDivData,
            (uint16_t)bTmpLength,
            abDivData));

        /* Copy diversified Key to output buffer */
        memcpy(pDiversifiedKey, abDivData, bLenDivInput); /* PRQA S 3200 */
        break;
#endif /* PH_CRYPTOSYM_SW_AES */
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        /* Parameter check */
        if (bLenDivInput != PH_CRYPTOSYM_DES_KEY_SIZE)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }

        /* Use the DivData as IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_LoadIv(pDataParams, pDivInput, bLenDivInput));

        /* Retrieve KeyVersion */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecodeVersion(pKey, &wKeyVersion));

        /* Half-Key Diversification */
        if (wOption & PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF)
        {
            /* Perform Encryption */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Encrypt(
                pDataParams, 
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                pKey,
                PH_CRYPTOSYM_DES_KEY_SIZE,
                pDiversifiedKey));

            /* half key diversification -> copy first part to second part */
            memcpy(&pDiversifiedKey[PH_CRYPTOSYM_DES_KEY_SIZE], &pDiversifiedKey[0], PH_CRYPTOSYM_DES_KEY_SIZE); /* PRQA S 3200 */
        }
        /* Full-Key Diversification */
        else
        {
            /* Perform Encryption */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Encrypt(
                pDataParams, 
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                pKey,
                PH_CRYPTOSYM_2K3DES_KEY_SIZE,
                pDiversifiedKey));
        }

        /* Re-Encode KeyVersion */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncodeVersion(pDiversifiedKey, wKeyVersion, wKeyType, pDiversifiedKey));

        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        /* Parameter check */
        if (bLenDivInput != PH_CRYPTOSYM_DES_KEY_SIZE)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }

        /* Use the DivData as IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_LoadIv(pDataParams, pDivInput, bLenDivInput));

        /* Retrieve KeyVersion */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecodeVersion(pKey, &wKeyVersion));

        /* Perform Encryption */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Encrypt(
            pDataParams, 
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
            pKey,
            PH_CRYPTOSYM_3K3DES_KEY_SIZE,
            pDiversifiedKey));

        /* Re-Encode KeyVersion */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncodeVersion(pDiversifiedKey, wKeyVersion, wKeyType, pDiversifiedKey));

        break;
#endif /* PH_CRYPTOSYM_SW_DES */
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
        break;
    case PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS:
        /* Parameter check */
        if (bLenDivInput > PH_CRYPTOSYM_SW_KDIV_MFP_DIVLENGTH_AES_MAX)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }

        /* Copy div. input */
        memcpy(&abDivData[1], pDivInput, bLenDivInput);  /* PRQA S 3200 */

        /* Increment div. input length */
        ++bLenDivInput;

        switch (wKeyType)
        {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        /* Set div. header */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_AES128_CONST;

        /* Perform MAC calculation */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            pDiversifiedKey,
            &bTmpLength));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        /* Set div. header for DiversifiedKeyA */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_AES192_CONST_1;

        /* Perform MAC calculation (KeyA) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            pDiversifiedKey,
            &bTmpLength));

        /* Set div. header for DiversifiedKeyB */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_AES192_CONST_2;

        /* Perform MAC calculation (KeyB) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            abMac,
            &bTmpLength));

        /* perform DiversifiedKeyA[8-15] ^ DiversifiedKeyB[0-7] */
        for (bTmpLength = 0; bTmpLength < 8; ++bTmpLength)
        {
            pDiversifiedKey[8 + bTmpLength] ^= abMac[bTmpLength];
        }

        /* copy DiversifiedKeyB[8-15] to DiversifiedKey[16-23] */
        memcpy(&pDiversifiedKey[16], &abMac[8], 8);  /* PRQA S 3200 */
        break;
#endif /* PH_CRYPTOSYM_SW_AES */
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        /* Set div. header for DiversifiedKeyA */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_3DES_CONST_1;

        /* Perform MAC calculation (KeyA) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            &pDiversifiedKey[0],
            &bTmpLength));

        /* Set div. header for DiversifiedKeyB */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_3DES_CONST_2;

        /* Perform MAC calculation (KeyB) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            &pDiversifiedKey[8],
            &bTmpLength));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        /* Set div. header for DiversifiedKeyA */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_3KEY3DES_CONST_1;

        /* Perform MAC calculation (KeyA) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            &pDiversifiedKey[0],
            &bTmpLength));

        /* Set div. header for DiversifiedKeyB */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_3KEY3DES_CONST_2;

        /* Perform MAC calculation (KeyB) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            &pDiversifiedKey[8],
            &bTmpLength));

        /* Set div. header for DiversifiedKeyC */
        abDivData[0] = PH_CRYPTOSYM_SW_KDIV_MFP_3KEY3DES_CONST_3;

        /* Perform MAC calculation (KeyC) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Diversify_CMAC(
            pDataParams, 
            abDivData, 
            (uint16_t)bLenDivInput, 
            &pDiversifiedKey[16],
            &bTmpLength));
        break;
#endif /* PH_CRYPTOSYM_SW_DES */
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Sw_SetConfig(
                                    phCryptoSym_Sw_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    )
{
    switch (wConfig)
    {
    case PH_CRYPTOSYM_CONFIG_KEEP_IV:
        /* parameter check */
        if ((wValue != PH_CRYPTOSYM_VALUE_KEEP_IV_OFF) &&
            (wValue != PH_CRYPTOSYM_VALUE_KEEP_IV_ON))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
        pDataParams->wKeepIV = wValue;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Sw_GetConfig(
                                    phCryptoSym_Sw_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    )
{
    switch(wConfig)
    {
    case PH_CRYPTOSYM_CONFIG_KEY_SIZE:
        switch(pDataParams->wKeyType)
        {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        *pValue = PH_CRYPTOSYM_AES128_KEY_SIZE;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        *pValue = PH_CRYPTOSYM_AES192_KEY_SIZE;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        *pValue = PH_CRYPTOSYM_AES256_KEY_SIZE;
        break;
#endif /* PH_CRYPTOSYM_SW_AES */
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
        *pValue = PH_CRYPTOSYM_DES_KEY_SIZE;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        *pValue = PH_CRYPTOSYM_2K3DES_KEY_SIZE;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        *pValue = PH_CRYPTOSYM_3K3DES_KEY_SIZE;
        break;
#endif /* PH_CRYPTOSYM_SW_DES */
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
        break;
    case PH_CRYPTOSYM_CONFIG_BLOCK_SIZE:
        switch(pDataParams->wKeyType)
        {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        *pValue = PH_CRYPTOSYM_AES_BLOCK_SIZE;
        break;
#endif /* PH_CRYPTOSYM_SW_AES */
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        *pValue = PH_CRYPTOSYM_DES_BLOCK_SIZE;
        break;
#endif /* PH_CRYPTOSYM_SW_DES */
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
        break;
    case PH_CRYPTOSYM_CONFIG_KEY_TYPE:
        *pValue = pDataParams->wKeyType;
        break;
    case PH_CRYPTOSYM_CONFIG_KEEP_IV:
        *pValue = pDataParams->wKeepIV;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}


#endif /* NXPBUILD__PH_CRYPTOSYM_SW */
