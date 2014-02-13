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
* SamAV2 KeyStore Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:27:27 2011 $
*
* History:
*  CHu: Generated 29. May 2009
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phKeyStore.h>
#include <phhalHw.h>

#ifdef NXPBUILD__PH_KEYSTORE_SAMAV2

#include "phKeyStore_SamAV2.h"
#include "phKeyStore_SamAV2_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phKeyStore_SamAV2_Init(
                               phKeyStore_SamAV2_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams, 
                               phhalHw_SamAV2_DataParams_t * pHalDataParams
                               )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    if (sizeof(phKeyStore_SamAV2_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_KEYSTORE);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pHalDataParams);

    /* Init private data */
    pDataParams->wId = PH_COMP_KEYSTORE| PH_KEYSTORE_SAMAV2_ID;
    pDataParams->pHalDataParams = pHalDataParams;

    /* set defaults */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_SetConfig(
        pDataParams,
        PH_KEYSTORE_CONFIG_SET_DEFAULT, PH_ON));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_FormatKeyEntry(
    phKeyStore_SamAV2_DataParams_t * pDataParams,           
    uint16_t wKeyNo,
    uint16_t wNewKeyType
    )
{
    phStatus_t statusTmp;
    uint8_t bCurrentKeyNoCEK ;
    uint8_t bProMas ;
    phKeyStore_SamAV2_KeyEntry_t pKeyEntry;
    uint8_t    pNullKey[24];
    uint8_t * pKeyA;
    uint8_t * pKeyB;
    uint8_t * pKeyC;
    uint8_t PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE + PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE];
    uint8_t PH_MEMLOC_REM bKeyEntryLength;
    uint8_t bOption = PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_DEFAULT;

    memset(pNullKey, 0x00, 24); /* PRQA S 3200 */

    if (wNewKeyType == PH_KEYSTORE_KEY_TYPE_MIFARE)
    {
        pNullKey[6] = pDataParams->bKeyNoMfDivA;
        pNullKey[7] = pDataParams->bKeyVMfDivA;
        pNullKey[14] = pDataParams->bKeyNoMfDivB;
        pNullKey[15] = pDataParams->bKeyVMfDivB;
    }

    /* Get the current KeyEntry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    /* Save Current CEK */
    bCurrentKeyNoCEK = pKeyEntry.bKeyNoCEK ;

    /* Set pKeys to NULL */
    pKeyA = pNullKey;
    pKeyB = pNullKey;
    pKeyC = pNullKey;

    /* Set Set Values */
    pKeyEntry.pSet[0] = pDataParams->pSet[0];
    pKeyEntry.pSet[1] = pDataParams->pSet[1];
    pKeyEntry.bExtSet = pDataParams->bExtSet;

    /* RESET old key entry setting */
    pKeyEntry.pSet[0] &= (uint8_t)~(uint8_t)PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK;

    /* define new Key type and Key C validity*/
    switch(wNewKeyType)
    {
    case PH_KEYSTORE_KEY_TYPE_AES128:
        pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_AES128_MASK << 3);
        pKeyEntry.bVersionKeyCValid = 0x01;
        break;

    case PH_KEYSTORE_KEY_TYPE_AES192:
        pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_AES192_MASK << 3);
        pKeyEntry.bVersionKeyCValid = 0x00;
        break;

    case PH_KEYSTORE_KEY_TYPE_2K3DES:
        if (pDataParams->b2K3DESOption == PH_KEYSTORE_SAMAV2_DES_OPTION_DESFIRE4)
        {
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK << 3);   
        }
        else if (pDataParams->b2K3DESOption == PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC16)
        {
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK << 3);   
        }
        else if (pDataParams->b2K3DESOption == PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC32)
        {
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK << 3);   
        }
        else
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
        }
        pKeyEntry.bVersionKeyCValid = 0x01;
        break;

    case PH_KEYSTORE_KEY_TYPE_3K3DES:
        pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK << 3); 
        pKeyEntry.bVersionKeyCValid = 0x00;
        break;
    case PH_KEYSTORE_KEY_TYPE_MIFARE:
        pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK << 3);
        pKeyEntry.bVersionKeyCValid = 0x01;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* Reset Key version */
    pKeyEntry.bVersionKeyA = 0x00;
    pKeyEntry.bVersionKeyB = 0x00;
    pKeyEntry.bVersionKeyC = 0x00;

    memcpy(pKeyEntry.pDFAid, pDataParams->pDFAid, 3); /* PRQA S 3200 */
    pKeyEntry.bDFKeyNo    = pDataParams->bDFKeyNo;

    pKeyEntry.bKeyNoCEK = pDataParams->bKeyNoCEK;
    pKeyEntry.bKeyVCEK    = pDataParams->bKeyVCEK;
    pKeyEntry.bRefNoKUC = pDataParams->bRefNoKUC;

    /* Fill the KeyEntry of SamAV2_Cmd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
        &pKeyEntry,
        pKeyA,
        pKeyB,
        pKeyC,
        pKeyEntryBuffer
        ));

    if (pKeyEntry.bVersionKeyCValid == PH_ON)
        bProMas = 0xFF;
    else
        bProMas = 0xDF; /* Do not ask to update KeyC */

    /* in AV1 and plain mode the UID has to be appended to the key data */ 
    if (pDataParams->pHalDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1;
        if (pDataParams->bChangeKeyPlain == PH_ON)
        {
            memcpy(pKeyEntryBuffer + PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1, pDataParams->pHalDataParams->bUid, PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);     /* PRQA S 3200 */
            bKeyEntryLength += PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE;
        }
    }
    else
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE;
    }

    if (pDataParams->bChangeKeyPlain == PH_ON)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNo,
        bProMas,
        pKeyEntryBuffer,
        bKeyEntryLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_SetKey(
                                    phKeyStore_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVersion,
                                    uint16_t wKeyType,
                                    uint8_t * pNewKey,
                                    uint16_t wNewKeyVersion
                                    )
{
    phStatus_t statusTmp;
    uint8_t * pKeyA;
    uint8_t * pKeyB;
    uint8_t * pKeyC;
    uint8_t bCurrentKeyType;
    uint8_t bProMas = 0x01; /* Key version included by default */
    phKeyStore_SamAV2_KeyEntry_t pKeyEntry;
    uint8_t bKeyStorage[16];
    uint8_t * pKeyPtr = pNewKey;
    uint8_t PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE + PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE];
    uint8_t PH_MEMLOC_REM bKeyEntryLength;
    uint8_t PH_MEMLOC_REM bOption = PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_DEFAULT;

    /* Mifare keys need to be realigned */
    if (wKeyType == PH_KEYSTORE_KEY_TYPE_MIFARE)
    {   
        memset(bKeyStorage, 0, 16);             /* PRQA S 3200 */
        memcpy(bKeyStorage, pNewKey, 6);           /* PRQA S 3200 */
        memcpy(&bKeyStorage[8], &pNewKey[6], 6);   /* PRQA S 3200 */

        bKeyStorage[6] = pDataParams->bKeyNoMfDivA;
        bKeyStorage[7] = pDataParams->bKeyVMfDivA;
        bKeyStorage[14] = pDataParams->bKeyNoMfDivB;
        bKeyStorage[15] = pDataParams->bKeyVMfDivB;

        pKeyPtr = bKeyStorage;
    }

    /* Get the current KeyEntry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    pKeyA = NULL;
    pKeyB = NULL;
    pKeyC = NULL;

    /* Check if the KeyType is matching with the current one */
    switch (((pKeyEntry.pSet[0] & PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK) >> 3))
    {
    case PH_KEYSTORE_SAMAV2_CMD_AES128_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_AES128;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_AES192_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_AES192;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_3K3DES; 
        break;
    case PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE; 
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* the Key Type to be loaded must match with the current keytype format */
    if (bCurrentKeyType != wKeyType)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* check if keys should be updated */
    if (wKeyNo != PH_KEYSTORE_INVALID_ID)
    {
        /* Check key entry version and change corresponding Key and version*/
        if (pKeyEntry.bVersionKeyA == (uint8_t)wKeyVersion)
        {
            pKeyA = pKeyPtr;
            pKeyEntry.bVersionKeyA = (uint8_t)wNewKeyVersion;
            bProMas |= 0x80U;
        }
        else if (pKeyEntry.bVersionKeyB == (uint8_t)wKeyVersion)
        {
            pKeyB = pKeyPtr;
            pKeyEntry.bVersionKeyB = (uint8_t)wNewKeyVersion;
            bProMas |= 0x40U;

        }
        else if ((pKeyEntry.bVersionKeyC == (uint8_t)wKeyVersion) && (pKeyEntry.bVersionKeyCValid == PH_ON))
        {    
            pKeyC = pKeyPtr;
            pKeyEntry.bVersionKeyC = (uint8_t)wNewKeyVersion;
            bProMas |= 0x20U;
        }
        else
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
        }
    }

    /* Set Set Values */
    bProMas |= 0x02U;
    pKeyEntry.pSet[0] &= PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK;
    pKeyEntry.pSet[0] |= pDataParams->pSet[0];
    pKeyEntry.pSet[1] = pDataParams->pSet[1];
    pKeyEntry.bExtSet = pDataParams->bExtSet;

    /* update CEK and Ref No KUC */
    bProMas |= 0x08U;
    pKeyEntry.bKeyNoCEK = pDataParams->bKeyNoCEK;
    pKeyEntry.bKeyVCEK = pDataParams->bKeyVCEK;

    /* update DFAid and DFKeyNo */
    bProMas |= 0x10U;
    memcpy(pKeyEntry.pDFAid, pDataParams->pDFAid, 3); /* PRQA S 3200 */
    pKeyEntry.bDFKeyNo    = pDataParams->bDFKeyNo;

    /* update KUC reference number */
    bProMas |= 0x04U;
    pKeyEntry.bRefNoKUC = pDataParams->bRefNoKUC;

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
        &pKeyEntry,
        pKeyA,
        pKeyB,
        pKeyC,
        pKeyEntryBuffer
        ));

    /* in AV1 and plain mode the UID has to be appended to the key data */ 
    if (pDataParams->pHalDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1;
        if (pDataParams->bChangeKeyPlain == PH_ON)
        {
            memcpy(pKeyEntryBuffer + PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1, pDataParams->pHalDataParams->bUid, PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);     /* PRQA S 3200 */
            bKeyEntryLength += PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE;
        }
    }
    else
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE;
    }

    if (pDataParams->bChangeKeyPlain == PH_ON)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNo,
        bProMas,
        pKeyEntryBuffer,
        bKeyEntryLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);

}

phStatus_t phKeyStore_SamAV2_SetKeyAtPos(
    phKeyStore_SamAV2_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wPos,
    uint16_t wKeyType,
    uint8_t * pNewKey,
    uint16_t wNewKeyVersion
    )
{
    phStatus_t statusTmp;
    uint8_t * pKeyA;
    uint8_t * pKeyB;
    uint8_t * pKeyC;
    uint8_t bCurrentKeyType ;
    uint8_t bKeyStorage[16];
    uint8_t * pKeyPtr = pNewKey;
    uint8_t bProMas = 0x01;
    phKeyStore_SamAV2_KeyEntry_t pKeyEntry;
    uint8_t PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE + PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE];
    uint8_t PH_MEMLOC_REM bKeyEntryLength;
    uint8_t bOption = PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_DEFAULT;

    /* Mifare keys need to be realigned */
    if (wKeyType == PH_KEYSTORE_KEY_TYPE_MIFARE)
    {   
        memset(bKeyStorage, 0, 16);             /* PRQA S 3200 */
        memcpy(bKeyStorage, pNewKey, 6);           /* PRQA S 3200 */
        memcpy(&bKeyStorage[8], &pNewKey[6], 6);   /* PRQA S 3200 */

        bKeyStorage[6] = pDataParams->bKeyNoMfDivA;
        bKeyStorage[7] = pDataParams->bKeyVMfDivA;
        bKeyStorage[14] = pDataParams->bKeyNoMfDivB;
        bKeyStorage[15] = pDataParams->bKeyVMfDivB;

        pKeyPtr = bKeyStorage;
    }

    /* Get the current KeyEntry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    pKeyA = NULL;
    pKeyB = NULL;
    pKeyC = NULL;

    /* check if keys should be updated */
    if (wKeyNo != PH_KEYSTORE_INVALID_ID)
    {
        if (wPos == 0)
        {
            pKeyA = pKeyPtr;
            pKeyEntry.bVersionKeyA = (uint8_t)wNewKeyVersion;
            bProMas |= 0x80U;
        }
        else if (wPos == 1)
        {
            pKeyB = pKeyPtr;
            pKeyEntry.bVersionKeyB = (uint8_t)wNewKeyVersion;
            bProMas |= 0x40U;
        }
        else if ((wPos == 2) && (pKeyEntry.bVersionKeyCValid == PH_ON))
        {
            pKeyC = pKeyPtr;
            pKeyEntry.bVersionKeyC = (uint8_t)wNewKeyVersion;
            bProMas |= 0x20U;
        }
        else
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);          
        }
    }

    /* Check if the KeyType is matching with the current one */
    switch (((pKeyEntry.pSet[0] & PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK) >> 3))
    {
    case PH_KEYSTORE_SAMAV2_CMD_AES128_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_AES128;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_AES192_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_AES192;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_3K3DES; 
        break;
    case PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE; 
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* the Key Type to be loaded must match with the current keytype format */
    if (bCurrentKeyType != wKeyType)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* update set */
    bProMas |= 0x02U;
    pKeyEntry.pSet[0] &= PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK;
    pKeyEntry.pSet[0] |= pDataParams->pSet[0];
    pKeyEntry.pSet[1] = pDataParams->pSet[1];
    pKeyEntry.bExtSet = pDataParams->bExtSet;

    /* update CEK and Ref No KUC */
    bProMas |= 0x08U;
    pKeyEntry.bKeyNoCEK = pDataParams->bKeyNoCEK;
    pKeyEntry.bKeyVCEK = pDataParams->bKeyVCEK;

    /* update DFAid and DFKeyNo */
    bProMas |= 0x10U;
    memcpy(pKeyEntry.pDFAid, pDataParams->pDFAid, 3); /* PRQA S 3200 */
    pKeyEntry.bDFKeyNo    = pDataParams->bDFKeyNo;

    /* update KUC reference number */
    bProMas |= 0x04U;
    pKeyEntry.bRefNoKUC = pDataParams->bRefNoKUC;

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
        &pKeyEntry,
        pKeyA,
        pKeyB,
        pKeyC,
        pKeyEntryBuffer
        ));

    /* in AV1 and plain mode the UID has to be appended to the key data */ 
    if (pDataParams->pHalDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1;
        if (pDataParams->bChangeKeyPlain == PH_ON)
        {
            memcpy(pKeyEntryBuffer + PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1, pDataParams->pHalDataParams->bUid, PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);     /* PRQA S 3200 */
            bKeyEntryLength += PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE;
        }
    }
    else
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE;
    }

    if (pDataParams->bChangeKeyPlain == PH_ON)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNo,
        bProMas,
        pKeyEntryBuffer,
        bKeyEntryLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_SetKUC(
                                 phKeyStore_SamAV2_DataParams_t * pDataParams,
                                 uint16_t wKeyNo,
                                 uint16_t wRefNoKUC
                                 )
{
    phStatus_t statusTmp;
    uint8_t * pKeyA;
    uint8_t * pKeyB;
    uint8_t * pKeyC;
    uint8_t bCurrentKeyNoCEK ;
    uint8_t bProMas ;
    phKeyStore_SamAV2_KeyEntry_t pKeyEntry;
    uint8_t PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE + PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE];
    uint8_t PH_MEMLOC_REM bKeyEntryLength;
    uint8_t bOption = PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_DEFAULT;

    pKeyA = NULL;
    pKeyB = NULL;
    pKeyC = NULL;

    /* Get the current KeyEntry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    /* Save the current KeyNoCEK */
    bCurrentKeyNoCEK = pKeyEntry.bKeyNoCEK;

    /* Update the KUC number */
    pKeyEntry.bRefNoKUC = (uint8_t)wRefNoKUC;

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
        &pKeyEntry,
        pKeyA,
        pKeyB,
        pKeyC,
        pKeyEntryBuffer
        ));

    bProMas = 0x05;

    /* in AV1 and plain mode the UID has to be appended to the key data */ 
    if (pDataParams->pHalDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1;
        if (pDataParams->bChangeKeyPlain == PH_ON)
        {
            memcpy(pKeyEntryBuffer + PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1, pDataParams->pHalDataParams->bUid, PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);     /* PRQA S 3200 */
            bKeyEntryLength += PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE;
        }
    }
    else
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE;
    }

    if (pDataParams->bChangeKeyPlain == PH_ON)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNo,
        bProMas,
        pKeyEntryBuffer,
        bKeyEntryLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_SetFullKeyEntry(
    phKeyStore_SamAV2_DataParams_t * pDataParams,
    uint16_t wNoOfKeys,
    uint16_t wKeyNo,
    uint16_t wNewRefNoKUC,
    uint16_t wNewKeyType,
    uint8_t * pNewKeys,
    uint16_t * pNewKeyVersionList
    )
{
    phStatus_t statusTmp;
    uint8_t * pKeyA;
    uint8_t * pKeyB;
    uint8_t * pKeyC;
    uint8_t bCurrentKeyNoCEK;
    uint8_t bCurrentKeyType;
    uint8_t bKeyLength;
    uint8_t bProMas;
    phKeyStore_SamAV2_KeyEntry_t pKeyEntry;
    uint8_t PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE + PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE];
    uint8_t PH_MEMLOC_REM bKeyEntryLength;
    uint8_t bOption = PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_DEFAULT;
    uint8_t PH_MEMLOC_REM pMfKeyVA[16];
    uint8_t PH_MEMLOC_REM pMfKeyVB[16];
    uint8_t PH_MEMLOC_REM pMfKeyVC[16];

    /* only 3 keys are allowed */
    if (wNoOfKeys > 3)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* Get the current KeyEntry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    /* Save the current KeyNoCEK */
    bCurrentKeyNoCEK = pKeyEntry.bKeyNoCEK;

    pKeyA = NULL;
    pKeyB = NULL;
    pKeyC = NULL;

    switch ((pKeyEntry.pSet[0] & PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK) >> 3)
    {
    case PH_KEYSTORE_SAMAV2_CMD_AES128_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_AES128;
        bKeyLength = 16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_AES192_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_AES192;
        bKeyLength = 24;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        bKeyLength = 16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_3K3DES; 
        bKeyLength = 24;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE; 
        bKeyLength = 12;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        bKeyLength = 16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK:
        bCurrentKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        bKeyLength = 16;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    if (bCurrentKeyType != wNewKeyType)
    {
        /* different key types are only allowed with also changing the keys. */
        if (wNoOfKeys == 0)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
        }

        /* Reset the KeyType */
        pKeyEntry.pSet[0] &= (uint8_t)~(uint8_t)PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK;
        switch(wNewKeyType)
        {
        case PH_KEYSTORE_KEY_TYPE_AES128:
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_AES128_MASK << 3);
            bKeyLength = 16;
            break;
        case PH_KEYSTORE_KEY_TYPE_AES192:
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_AES192_MASK << 3); 
            bKeyLength = 24;
            break;
        case PH_KEYSTORE_KEY_TYPE_2K3DES:
            if (pDataParams->b2K3DESOption == PH_KEYSTORE_SAMAV2_DES_OPTION_DESFIRE4)
            {
                pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK << 3);   
            }
            else if (pDataParams->b2K3DESOption == PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC16)
            {
                pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK << 3);   
            }
            else if (pDataParams->b2K3DESOption == PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC32)
            {
                pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK << 3);   
            }
            else
            {
                return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
            }
            bKeyLength = 16;
            break;
        case PH_KEYSTORE_KEY_TYPE_3K3DES:
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK << 3); 
            bKeyLength = 24;
            break;
        case PH_KEYSTORE_KEY_TYPE_MIFARE:
            pKeyEntry.pSet[0] |= (uint8_t)(PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK << 3);
            bKeyLength = 12;
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
        }
    }

    /* update set */
    pKeyEntry.pSet[0] &= PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK;
    pKeyEntry.pSet[0] |= pDataParams->pSet[0];
    pKeyEntry.pSet[1] = pDataParams->pSet[1];
    pKeyEntry.bExtSet = pDataParams->bExtSet;

    /* check if keys should be updated */
    if (wNoOfKeys > 0)
    {
        /* in case of MIFARE keys the div key pointer has to be updated in each key */
        if (wNewKeyType == PH_KEYSTORE_KEY_TYPE_MIFARE)
        {
            memset(pMfKeyVA, 0, 16);             /* PRQA S 3200 */
            memcpy(pMfKeyVA, pNewKeys, 6);           /* PRQA S 3200 */
            memcpy(&pMfKeyVA[8], &pNewKeys[6], 6);   /* PRQA S 3200 */

            pMfKeyVA[6] = pDataParams->bKeyNoMfDivA;
            pMfKeyVA[7] = pDataParams->bKeyVMfDivA;
            pMfKeyVA[14] = pDataParams->bKeyNoMfDivB;
            pMfKeyVA[15] = pDataParams->bKeyVMfDivB;

            pKeyA = pMfKeyVA;
            pKeyEntry.bVersionKeyA = (uint8_t) pNewKeyVersionList[0];
            pKeyEntry.bVersionKeyCValid = PH_OFF;

            if (wNoOfKeys > 1)
            {
                memset(pMfKeyVB, 0, 16);                        /* PRQA S 3200 */
                memcpy(pMfKeyVB, &pNewKeys[bKeyLength], 6);        /* PRQA S 3200 */
                memcpy(&pMfKeyVB[8], &pNewKeys[bKeyLength+6], 6);  /* PRQA S 3200 */

                pMfKeyVB[6] = pDataParams->bKeyNoMfDivA;
                pMfKeyVB[7] = pDataParams->bKeyVMfDivA;
                pMfKeyVB[14] = pDataParams->bKeyNoMfDivB;
                pMfKeyVB[15] = pDataParams->bKeyVMfDivB;

                pKeyB = pMfKeyVB;
                pKeyEntry.bVersionKeyB = (uint8_t) pNewKeyVersionList[1];
            }

            if (wNoOfKeys > 2)
            {
                memset(pMfKeyVC, 0, 16);                            /* PRQA S 3200 */
                memcpy(pMfKeyVC, &pNewKeys[2*bKeyLength], 6);            /* PRQA S 3200 */
                memcpy(&pMfKeyVC[8], &pNewKeys[2*bKeyLength+6], 6);    /* PRQA S 3200 */

                pMfKeyVC[6] = pDataParams->bKeyNoMfDivA;
                pMfKeyVC[7] = pDataParams->bKeyVMfDivA;
                pMfKeyVC[14] = pDataParams->bKeyNoMfDivB;
                pMfKeyVC[15] = pDataParams->bKeyVMfDivB;

                pKeyC = pMfKeyVC;
                pKeyEntry.bVersionKeyC = (uint8_t) pNewKeyVersionList[2];
                pKeyEntry.bVersionKeyCValid = PH_ON;
            }
        }
        else
        {
            pKeyA = pNewKeys;
            pKeyEntry.bVersionKeyA = (uint8_t) pNewKeyVersionList[0];
            pKeyEntry.bVersionKeyCValid = PH_OFF;

            if (wNoOfKeys > 1)
            {
                pKeyB = &pNewKeys[bKeyLength];
                pKeyEntry.bVersionKeyB = (uint8_t) pNewKeyVersionList[1];
            }

            if (wNoOfKeys > 2)
            {
                if ((wNewKeyType == PH_KEYSTORE_KEY_TYPE_AES192) ||
                    (wNewKeyType == PH_KEYSTORE_KEY_TYPE_3K3DES))
                {
                    return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
                }
                else
                {
                    pKeyC = &pNewKeys[2*bKeyLength];
                    pKeyEntry.bVersionKeyC = (uint8_t) pNewKeyVersionList[2];
                    pKeyEntry.bVersionKeyCValid = PH_ON;
                }
            }
        }
    }

    /* update CEK and Ref No KUC */
    pKeyEntry.bKeyNoCEK = pDataParams->bKeyNoCEK;
    pKeyEntry.bKeyVCEK = pDataParams->bKeyVCEK;
    pKeyEntry.bRefNoKUC = (uint8_t)wNewRefNoKUC;

    /* update DFAid and DFKeyNo */
    memcpy(pKeyEntry.pDFAid, pDataParams->pDFAid, 3); /* PRQA S 3200 */
    pKeyEntry.bDFKeyNo    = pDataParams->bDFKeyNo;

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
        &pKeyEntry,
        pKeyA,
        pKeyB,
        pKeyC,
        pKeyEntryBuffer
        ));
    
    if (pKeyEntry.bVersionKeyCValid == PH_ON)
    {
        bProMas = 0xFF;
    }
    else
    {
        bProMas = 0xDF; /* Do not ask to update KeyC */
    }

    /* if number of keys are zero we do not want to update the keys and versions. */
    if (wNoOfKeys == 0)
    {
        bProMas &= (uint8_t)~(uint8_t)0xE1U;
    }

    /* in AV1 and plain mode the UID has to be appended to the key data */ 
    if (pDataParams->pHalDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1;
        if (pDataParams->bChangeKeyPlain == PH_ON)
        {
            memcpy(pKeyEntryBuffer + PHHAL_HW_SAMAV2_KEYENTRY_SIZE_AV1, pDataParams->pHalDataParams->bUid, PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);     /* PRQA S 3200 */
            bKeyEntryLength += PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE;
        }
    }
    else
    {
        bKeyEntryLength = PHHAL_HW_SAMAV2_KEYENTRY_SIZE;
    }

    if (pDataParams->bChangeKeyPlain == PH_ON)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
        pDataParams->pHalDataParams,
        bOption,
        (uint8_t)wKeyNo,
        bProMas,
        pKeyEntryBuffer,
        bKeyEntryLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_GetKeyEntry(
                                      phKeyStore_SamAV2_DataParams_t * pDataParams,    
                                      uint16_t wKeyNo,
                                      uint16_t wKeyVersionBufSize,
                                      uint16_t * wKeyVersion,
                                      uint16_t * wKeyVersionLength,
                                      uint16_t * pKeyType
                                      )
{
    phKeyStore_SamAV2_KeyEntry_t PH_MEMLOC_REM pKeyEntry;
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint16_t wHostMode;

    *wKeyVersionLength = 0x00;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_SAMAV2_CONFIG_HOSTMODE, &wHostMode));

    /* First try to find the correct key position */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    /* Checks */
    if ((pKeyEntry.bVersionKeyCValid && (wKeyVersionBufSize < 6)) || (!pKeyEntry.bVersionKeyCValid && (wKeyVersionBufSize < 4)))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    wKeyVersion[0] = pKeyEntry.bVersionKeyA;
    wKeyVersion[1] = pKeyEntry.bVersionKeyB;
    *wKeyVersionLength = 2;
    if (pKeyEntry.bVersionKeyCValid != 0x00)
    {
        wKeyVersion[2] = pKeyEntry.bVersionKeyC;
        *wKeyVersionLength = 3;
    }

    switch(( (pKeyEntry.pSet[0]& PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK) >> 3))
    {
    case PH_KEYSTORE_SAMAV2_CMD_AES128_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_AES128;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_AES192_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_AES192;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES; 
        pDataParams->b2K3DESOption = PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_3K3DES; 
        break;
    case PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE; 
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        pDataParams->b2K3DESOption = PH_KEYSTORE_SAMAV2_DES_OPTION_DESFIRE4;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        pDataParams->b2K3DESOption = PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC32;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* update set and exset */
    pDataParams->pSet[0] = (uint8_t) (pKeyEntry.pSet[0] & ~PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK);
    pDataParams->pSet[1] = pKeyEntry.pSet[1];
    if (wHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        pDataParams->bExtSet = 0;
    }
    else
    {
        pDataParams->bExtSet = pKeyEntry.bExtSet;
    }

    pDataParams->bKeyNoCEK = pKeyEntry.bKeyNoCEK;
    pDataParams->bKeyVCEK  = pKeyEntry.bKeyVCEK;
    memcpy(pDataParams->pDFAid, pKeyEntry.pDFAid, 3); /* PRQA S 3200 */
    pDataParams->bDFKeyNo = pKeyEntry.bDFKeyNo;
    pDataParams->bRefNoKUC = pKeyEntry.bRefNoKUC;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_GetKey(
                                    phKeyStore_SamAV2_DataParams_t * pDataParams,    
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVersion,
                                    uint8_t bKeyBufSize,
                                    uint8_t * pKey,
                                    uint16_t * pKeyType
                                    )
{
    phKeyStore_SamAV2_KeyEntry_t PH_MEMLOC_REM pKeyEntry;
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint16_t PH_MEMLOC_REM wHostMode;
    uint8_t PH_MEMLOC_REM bKeyLength;
    uint8_t PH_MEMLOC_REM bIsMifare = 0;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_SAMAV2_CONFIG_HOSTMODE, &wHostMode));

    /* Get the current KeyEntry */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_SamAV2_Int_GetKeyEntry(
        pDataParams,
        (uint8_t)wKeyNo,
        &pKeyEntry
        ));

    switch ((pKeyEntry.pSet[0] & PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK) >> 3)
    {
    case PH_KEYSTORE_SAMAV2_CMD_AES128_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_AES128;
        bKeyLength = 16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_AES192_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_AES192;
        bKeyLength = 24;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        bKeyLength = 16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_3K3DES; 
        bKeyLength = 24;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE; 
        bKeyLength = 6;
        bIsMifare = 1;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;  
        bKeyLength = 16;
        break;
    case PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK:
        *pKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        bKeyLength = 16;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }
    

    if (bIsMifare)
    {
        /* the allow dump MIFARE key flag has to be set. */
        if (!(pKeyEntry.pSet[0] & 0x01))
        {
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
        }
    }
    else
    {
        if (wHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
        {
            /* in AV1 only MIFARE keys can be dumped. */
            return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
        }
        else
        {
            /* check if secret key dump is allowed */
            if (!(pKeyEntry.bExtSet & 0x08))
            {
                return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
            }
        }
    }
    
    if (bKeyBufSize < bKeyLength)
    {
        return PH_ADD_COMPCODE(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_KEYSTORE);
    }

    if (pKeyEntry.bVersionKeyA != (uint8_t) wKeyVersion && pKeyEntry.bVersionKeyB != (uint8_t) wKeyVersion && pKeyEntry.bVersionKeyC != (uint8_t) wKeyVersion)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    if (bIsMifare)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyMifare(
            pDataParams->pHalDataParams,
            PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_RETRIEVE_KEY,
            (uint8_t) wKeyNo,
            (uint8_t) wKeyVersion,
            0x0A,
            NULL,
            NULL,
            0,
            pKey,
            &bKeyLength));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyMifare(
            pDataParams->pHalDataParams,
            PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_RETRIEVE_KEY,
            (uint8_t) wKeyNo,
            (uint8_t) wKeyVersion,
            0x0B,
            NULL,
            NULL,
            0,
            &pKey[6],
            &bKeyLength));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_DumpSecretKey(
            pDataParams->pHalDataParams,
            0,
            (uint8_t) wKeyNo,
            (uint8_t) wKeyVersion,
            NULL,
            0,
            pKey,
            &bKeyLength));
    }
    
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}


phStatus_t phKeyStore_SamAV2_SetConfig(
                                    phKeyStore_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    )
{
    switch(wConfig)
    {
    case PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_DUMP_SESSION_KEY:
        if (wValue)
            pDataParams->pSet[0] |= PH_KEYSTORE_SAMAV2_SET0_ALLOW_DUMP_SESSION_KEY;
        else
            pDataParams->pSet[0] = (uint8_t) (pDataParams->pSet[0] & (~PH_KEYSTORE_SAMAV2_SET0_ALLOW_DUMP_SESSION_KEY));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_CRYPTO_SECRET_KEY:
        if (wValue)
            pDataParams->pSet[0] |= PH_KEYSTORE_SAMAV2_SET0_ALLOW_CRYPTO_SECRET_KEY;
        else
            pDataParams->pSet[0] = (uint8_t) (pDataParams->pSet[0] & (~PH_KEYSTORE_SAMAV2_SET0_ALLOW_CRYPTO_SECRET_KEY));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEEP_IV:
        if (wValue)
            pDataParams->pSet[0] |= PH_KEYSTORE_SAMAV2_SET0_KEEP_IV;
        else
            pDataParams->pSet[0] = (uint8_t) (pDataParams->pSet[0] & (~PH_KEYSTORE_SAMAV2_SET0_KEEP_IV));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HIGH_SEC_LEVEL:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_ENABLE_HIGH_SEC_LEVEL;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_ENABLE_HIGH_SEC_LEVEL));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HOST_AUTH:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_KEY_ENTRY:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_DISABLE_KEY_ENTRY;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_DISABLE_KEY_ENTRY));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HOST_AUTH_AFTER_RESET:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH_AFTER_RESET;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH_AFTER_RESET));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_LOCK_UNLOCK:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_ENABLE_LOCK_UNLOCK;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_ENABLE_LOCK_UNLOCK));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_CHANGE_KEY_PICC:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_DISABLE_CHANGE_KEY_PICC;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_DISABLE_CHANGE_KEY_PICC));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_DECRYPTION:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_DISABLE_DECRYPTION;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_DISABLE_DECRYPTION));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_ENCRYPTION:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_DISABLE_ENCRYPTION;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_DISABLE_ENCRYPTION));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_VERIFY_MAC:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_DISABLE_VERIFY_MAC;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_DISABLE_VERIFY_MAC));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_GENERATE_MAC:
        if (wValue)
            pDataParams->pSet[1] |= PH_KEYSTORE_SAMAV2_SET1_DISABLE_GENERATE_MAC;
        else
            pDataParams->pSet[1] = (uint8_t) (pDataParams->pSet[1] & (~PH_KEYSTORE_SAMAV2_SET1_DISABLE_GENERATE_MAC));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_DUMP_SECRET_KEY:
        if (wValue)
            pDataParams->bExtSet |= PH_KEYSTORE_SAMAV2_EXTSET_ALLOW_DUMP_SECRET_KEY;
        else
            pDataParams->bExtSet = (uint8_t) (pDataParams->bExtSet & (~PH_KEYSTORE_SAMAV2_EXTSET_ALLOW_DUMP_SECRET_KEY));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_MANDATE_KEY_DIVERSIFICATION:
        if (wValue)
            pDataParams->bExtSet |= PH_KEYSTORE_SAMAV2_EXTSET_MANDATE_KEY_DIVERSIFICATION;
        else
            pDataParams->bExtSet = (uint8_t) (pDataParams->bExtSet & (~PH_KEYSTORE_SAMAV2_EXTSET_MANDATE_KEY_DIVERSIFICATION));
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_CHANGE_KEY_PLAIN:
        pDataParams->bChangeKeyPlain = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYCLASS:
        pDataParams->bExtSet = (uint8_t) (pDataParams->bExtSet & (~PH_KEYSTORE_SAMAV2_KEYCLASS_MASK));
        pDataParams->bExtSet |= (uint8_t) wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DF_KEY_NO:
        pDataParams->bDFKeyNo = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_CEK:
        pDataParams->bKeyNoCEK = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_CEK:
        pDataParams->bKeyVCEK = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_REF_NO_KUC:
        pDataParams->bRefNoKUC = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_CKUC:
        pDataParams->bKeyNoCKUC = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_CKUC:
        pDataParams->bKeyVCKUC = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DES_KEY_OPTION:
        pDataParams->b2K3DESOption = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_MF_DIV_A:
        pDataParams->bKeyNoMfDivA = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_MF_DIV_A:
        pDataParams->bKeyVMfDivA = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_MF_DIV_B:
        pDataParams->bKeyNoMfDivB = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_MF_DIV_B:
        pDataParams->bKeyVMfDivB = (uint8_t)wValue;
        break;
    case PH_KEYSTORE_CONFIG_SET_DEFAULT:
        pDataParams->pSet[0] = 0x00;
        pDataParams->pSet[1] = 0x00;
        pDataParams->bExtSet = 0x00;
        pDataParams->bChangeKeyPlain = PH_OFF;
        pDataParams->bDFKeyNo = 0x00;
        pDataParams->bKeyNoCEK = 0x00;
        pDataParams->bKeyVCEK = 0x00;
        pDataParams->bRefNoKUC = 0xFF;
        pDataParams->bKeyNoCKUC = 0x00;
        pDataParams->bKeyVCKUC = 0x00;
        pDataParams->bKeyNoMfDivA = 0x00;
        pDataParams->bKeyVMfDivA = 0x00;
        pDataParams->bKeyNoMfDivB = 0x00;
        pDataParams->bKeyVMfDivB = 0x00;
        memset(pDataParams->pDFAid, 0, 3);  /* PRQA S 3200 */
        
        break;
    

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);

    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}


phStatus_t phKeyStore_SamAV2_SetConfigStr(
    phKeyStore_SamAV2_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint8_t *pBuffer,
    uint16_t wBufferLength    
    )
{
    switch(wConfig)
    {
    case PH_KEYSTORE_SAMAV2_CONFIG_DF_AID:
        if (wBufferLength != 3)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
        }
        memcpy(pDataParams->pDFAid, pBuffer, 3);   /* PRQA S 3200 */
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}


phStatus_t phKeyStore_SamAV2_GetConfig(
                                    phKeyStore_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    )
{

    switch(wConfig)
    {
    case PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_DUMP_SESSION_KEY:
        *pValue = (uint16_t)((pDataParams->pSet[0] & PH_KEYSTORE_SAMAV2_SET0_ALLOW_DUMP_SESSION_KEY) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_CRYPTO_SECRET_KEY:
        *pValue = (uint16_t)((pDataParams->pSet[0] & PH_KEYSTORE_SAMAV2_SET0_ALLOW_CRYPTO_SECRET_KEY) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEEP_IV:
        *pValue = (uint16_t)((pDataParams->pSet[0] & PH_KEYSTORE_SAMAV2_SET0_KEEP_IV) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HIGH_SEC_LEVEL:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_ENABLE_HIGH_SEC_LEVEL) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HOST_AUTH:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_KEY_ENTRY:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_DISABLE_KEY_ENTRY) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HOST_AUTH_AFTER_RESET:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH_AFTER_RESET) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_LOCK_UNLOCK:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_ENABLE_LOCK_UNLOCK) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_CHANGE_KEY_PICC:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_DISABLE_CHANGE_KEY_PICC) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_DECRYPTION:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_DISABLE_DECRYPTION) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_ENCRYPTION:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_DISABLE_ENCRYPTION) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_VERIFY_MAC:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_DISABLE_VERIFY_MAC) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_GENERATE_MAC:
        *pValue = (uint16_t)((pDataParams->pSet[1] & PH_KEYSTORE_SAMAV2_SET1_DISABLE_GENERATE_MAC) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_DUMP_SECRET_KEY:
        *pValue = (uint16_t)((pDataParams->bExtSet & PH_KEYSTORE_SAMAV2_EXTSET_ALLOW_DUMP_SECRET_KEY) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_MANDATE_KEY_DIVERSIFICATION:
        *pValue = (uint16_t)((pDataParams->bExtSet & PH_KEYSTORE_SAMAV2_EXTSET_MANDATE_KEY_DIVERSIFICATION) ? 1 : 0);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYCLASS:
        *pValue = (uint16_t)(pDataParams->bExtSet & PH_KEYSTORE_SAMAV2_KEYCLASS_MASK);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_CHANGE_KEY_PLAIN:
        *pValue = (uint16_t)(pDataParams->bChangeKeyPlain);
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DF_KEY_NO:
        *pValue = (uint16_t) pDataParams->bDFKeyNo;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_CEK:
        *pValue = (uint16_t)pDataParams->bKeyNoCEK;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_CEK:
        *pValue = (uint16_t)pDataParams->bKeyVCEK;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_REF_NO_KUC:
        *pValue = (uint16_t)pDataParams->bRefNoKUC;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_CKUC:
        *pValue = (uint16_t)pDataParams->bKeyNoCKUC;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_CKUC:
        *pValue = (uint16_t)pDataParams->bKeyVCKUC;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_DES_KEY_OPTION:
        *pValue = (uint16_t)pDataParams->b2K3DESOption;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_MF_DIV_A:
        *pValue = (uint16_t)pDataParams->bKeyNoMfDivA;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_MF_DIV_A:
        *pValue = (uint16_t)pDataParams->bKeyVMfDivA;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_MF_DIV_B:
        *pValue = (uint16_t)pDataParams->bKeyNoMfDivB;
        break;
    case PH_KEYSTORE_SAMAV2_CONFIG_KEYV_MF_DIV_B:
        *pValue = (uint16_t)pDataParams->bKeyVMfDivB;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_GetConfigStr(
    phKeyStore_SamAV2_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint8_t ** ppBuffer,
    uint16_t * pBufferLength
    )
{
    switch(wConfig)
    {
    case PH_KEYSTORE_SAMAV2_CONFIG_DF_AID:
        *ppBuffer = pDataParams->pDFAid;
        *pBufferLength = 3;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_ChangeKUC(
                                    phKeyStore_SamAV2_DataParams_t * pDataParams,                    
                                    uint16_t wRefNoKUC,
                                    uint32_t dwLimit
                                    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t PH_MEMLOC_REM bProMas = 0x00;
    uint8_t PH_MEMLOC_REM pKucData[0x06 + PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE];
    uint8_t PH_MEMLOC_REM bKucDataLength;
    uint8_t PH_MEMLOC_REM bOption = PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_DEFAULT;

    bProMas |= PHHAL_HW_SAMAV2_CMD_UPDATE_LIMIT_MASK;
    pKucData[0] = (uint8_t)(dwLimit >> 0);
    pKucData[1] = (uint8_t)(dwLimit >> 8);
    pKucData[2] = (uint8_t)(dwLimit >> 16);
    pKucData[3] = (uint8_t)(dwLimit >> 24);
    
    if (pDataParams->bKeyNoCKUC != (PH_KEYSTORE_INVALID_ID & 0xFF))
    {
        bProMas |= PHHAL_HW_SAMAV2_CMD_UPDATE_KEYNO_CKUC_MASK;
        pKucData[4] = pDataParams->bKeyNoCKUC;
    }
    if (pDataParams->bKeyVCKUC != (PH_KEYSTORE_INVALID_ID & 0xFF))
    {
        bProMas |= PHHAL_HW_SAMAV2_CMD_UPDATE_KEY_VCKUC_MASK;
        pKucData[5] = pDataParams->bKeyVCKUC;
    }

    bKucDataLength = 0x06;

    /* in AV1 and plain mode the UID has to be appended to the key data */ 
    if (pDataParams->pHalDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        if (pDataParams->bChangeKeyPlain == PH_ON)
        {
            memcpy(pKucData + 0x06, pDataParams->pHalDataParams->bUid, PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE);     /* PRQA S 3200 */
            bKucDataLength += PHHAL_HW_SAMAV2_HC_SAM_UID_SIZE;
        }
    }

    if (pDataParams->bChangeKeyPlain == PH_ON)
    {
        bOption |= PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_PLAIN;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKUCEntry(
        pDataParams->pHalDataParams, 
        bOption,
        (uint8_t)wRefNoKUC,
        bProMas,
        pKucData,
        bKucDataLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_GetKUC(
                                 phKeyStore_SamAV2_DataParams_t * pDataParams,
                                 uint16_t wRefNoKUC,
                                 uint32_t * pdwLimit,
                                 uint32_t * pdwCurVal
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t        PH_MEMLOC_REM pKucBuffer[10];

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetKUCEntry(
        pDataParams->pHalDataParams, 
        (uint8_t)wRefNoKUC,
        pKucBuffer              
        ));

    *pdwLimit = (uint32_t)((uint32_t)(pKucBuffer[3]) << 24);
    *pdwLimit |= (uint32_t)((uint32_t)(pKucBuffer[2]) << 16);
    *pdwLimit |= (uint32_t)((uint32_t)(pKucBuffer[1]) << 8);
    *pdwLimit |= (uint32_t)((uint32_t)(pKucBuffer[0]) << 0);
    pDataParams->bKeyNoCKUC = pKucBuffer[4];
    pDataParams->bKeyVCKUC = pKucBuffer[5];
    *pdwCurVal = (uint32_t)((uint32_t)(pKucBuffer[9]) << 24);
    *pdwCurVal |= (uint32_t)((uint32_t)(pKucBuffer[8]) << 16);
    *pdwCurVal |= (uint32_t)((uint32_t)(pKucBuffer[7]) << 8);
    *pdwCurVal |= (uint32_t)((uint32_t)(pKucBuffer[6]) << 0);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

#endif /* NXPBUILD__PH_KEYSTORE_SAMAV2 */
