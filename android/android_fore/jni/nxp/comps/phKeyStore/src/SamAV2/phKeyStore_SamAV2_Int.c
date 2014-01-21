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
* Wait Routines for SAMAV2 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:27:27 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phhalHw.h>

#ifdef NXPBUILD__PH_KEYSTORE_SAMAV2

#include <phhalHw_SamAV2_Cmd.h>
#include "phKeyStore_SamAV2.h"
#include "phKeyStore_SamAV2_Int.h"
#include "../../../phhalHw/src/SamAV2/phhalHw_SamAV2.h"

phStatus_t phKeyStore_SamAV2_Int_GetKeyEntry(
    phKeyStore_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNumber,
    phKeyStore_SamAV2_KeyEntry_t * pKeyEntry
    )
{
    phStatus_t                      PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wHostMode;
    uint8_t     PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE];
    uint8_t     PH_MEMLOC_REM bKeyEntryLength;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams->pHalDataParams,
        bKeyNumber,
        pKeyEntryBuffer,
        &bKeyEntryLength
        ));

    /* get host mode */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_SAMAV2_CONFIG_HOSTMODE, &wHostMode));

    /* check for AV1 mode */
    if (wHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {                          
        if (bKeyEntryLength == PHHAL_HW_SAMAV2_CMD_AV1_KEY_ENTRY_RESPONSE_NO_VC_LENGTH)
        {
            pKeyEntry->bVersionKeyA = pKeyEntryBuffer[0];
            pKeyEntry->bVersionKeyB = pKeyEntryBuffer[1];
            pKeyEntry->bVersionKeyC = 0x00;
            pKeyEntry->pDFAid[0]    = pKeyEntryBuffer[2];
            pKeyEntry->pDFAid[1]    = pKeyEntryBuffer[3];
            pKeyEntry->pDFAid[2]    = pKeyEntryBuffer[4];
            pKeyEntry->bDFKeyNo        = pKeyEntryBuffer[5];
            pKeyEntry->bKeyNoCEK    = pKeyEntryBuffer[6];
            pKeyEntry->bKeyVCEK        = pKeyEntryBuffer[7];
            pKeyEntry->bRefNoKUC    = pKeyEntryBuffer[8];
            pKeyEntry->pSet[0]        = pKeyEntryBuffer[9];
            pKeyEntry->pSet[1]        = pKeyEntryBuffer[10];
            pKeyEntry->bExtSet        = 0x00;
            pKeyEntry->bVersionKeyCValid = 0x00;
        }
        else if (bKeyEntryLength == PHHAL_HW_SAMAV2_CMD_AV1_KEY_ENTRY_RESPONSE_VC_LENGTH)
        {
            pKeyEntry->bVersionKeyA = pKeyEntryBuffer[0];
            pKeyEntry->bVersionKeyB = pKeyEntryBuffer[1];
            pKeyEntry->bVersionKeyC = pKeyEntryBuffer[2];
            pKeyEntry->pDFAid[0]    = pKeyEntryBuffer[3];
            pKeyEntry->pDFAid[1]    = pKeyEntryBuffer[4];
            pKeyEntry->pDFAid[2]    = pKeyEntryBuffer[5];
            pKeyEntry->bDFKeyNo        = pKeyEntryBuffer[6];
            pKeyEntry->bKeyNoCEK    = pKeyEntryBuffer[7];
            pKeyEntry->bKeyVCEK        = pKeyEntryBuffer[8];
            pKeyEntry->bRefNoKUC    = pKeyEntryBuffer[9];
            pKeyEntry->pSet[0]        = pKeyEntryBuffer[10];
            pKeyEntry->pSet[1]        = pKeyEntryBuffer[11];
            pKeyEntry->bExtSet        = 0x00;
            pKeyEntry->bVersionKeyCValid = 0x01;
        }
        else
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_KEYSTORE);
    }
    /* AV2 mode */
    else
    {
        if (bKeyEntryLength == PHHAL_HW_SAMAV2_CMD_AV2_KEY_ENTRY_RESPONSE_NO_VC_LENGTH)
        {
            pKeyEntry->bVersionKeyA = pKeyEntryBuffer[0];
            pKeyEntry->bVersionKeyB = pKeyEntryBuffer[1];
            pKeyEntry->bVersionKeyC = 0x00;
            pKeyEntry->pDFAid[0]    = pKeyEntryBuffer[2];
            pKeyEntry->pDFAid[1]    = pKeyEntryBuffer[3];
            pKeyEntry->pDFAid[2]    = pKeyEntryBuffer[4];
            pKeyEntry->bDFKeyNo        = pKeyEntryBuffer[5];
            pKeyEntry->bKeyNoCEK    = pKeyEntryBuffer[6];
            pKeyEntry->bKeyVCEK        = pKeyEntryBuffer[7];
            pKeyEntry->bRefNoKUC    = pKeyEntryBuffer[8];
            pKeyEntry->pSet[0]        = pKeyEntryBuffer[9];
            pKeyEntry->pSet[1]        = pKeyEntryBuffer[10];
            pKeyEntry->bExtSet        = pKeyEntryBuffer[11];
            pKeyEntry->bVersionKeyCValid = 0x00;
            
        }
        else if (bKeyEntryLength == PHHAL_HW_SAMAV2_CMD_AV2_KEY_ENTRY_RESPONSE_VC_LENGTH)
        {
            pKeyEntry->bVersionKeyA = pKeyEntryBuffer[0];
            pKeyEntry->bVersionKeyB = pKeyEntryBuffer[1];
            pKeyEntry->bVersionKeyC = pKeyEntryBuffer[2];
            pKeyEntry->pDFAid[0]    = pKeyEntryBuffer[3];
            pKeyEntry->pDFAid[1]    = pKeyEntryBuffer[4];
            pKeyEntry->pDFAid[2]    = pKeyEntryBuffer[5];
            pKeyEntry->bDFKeyNo        = pKeyEntryBuffer[6];
            pKeyEntry->bKeyNoCEK    = pKeyEntryBuffer[7];
            pKeyEntry->bKeyVCEK        = pKeyEntryBuffer[8];
            pKeyEntry->bRefNoKUC    = pKeyEntryBuffer[9];
            pKeyEntry->pSet[0]        = pKeyEntryBuffer[10];
            pKeyEntry->pSet[1]        = pKeyEntryBuffer[11];
            pKeyEntry->bExtSet        = pKeyEntryBuffer[12];
            pKeyEntry->bVersionKeyCValid = 0x01;
        }
        else
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_KEYSTORE);
    }    

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
    phKeyStore_SamAV2_KeyEntry_t * pKeyEntry,            /**< [In] Pointer to the KeyStore KeyEntry structure. */    
    uint8_t * pKeyA,                                /**< [In] Pointer to KeyA . */    
    uint8_t * pKeyB,                                /**< [In] Pointer to KeyB . */    
    uint8_t * pKeyC,                                /**< [In] Pointer to KeyC . */
    uint8_t * pKeyEntryBuffer                       /**< [Out] Pointer to the key buffer . */
    )
{
    /* init buffer */
    memset(pKeyEntryBuffer, 0x00, PHHAL_HW_SAMAV2_KEYENTRY_SIZE); /* PRQA S 3200 */ 

    /* copy keys */
    if (pKeyEntry->bVersionKeyCValid == PH_ON)
    {
        if (pKeyA != NULL)
            memcpy(pKeyEntryBuffer, pKeyA, 16);          /* PRQA S 3200 */

        if (pKeyB != NULL)
            memcpy(&pKeyEntryBuffer[16], pKeyB, 16);     /* PRQA S 3200 */

        if (pKeyC != NULL)
            memcpy(&pKeyEntryBuffer[32], pKeyC, 16);     /* PRQA S 3200 */
    }
    else
    {
        if (pKeyA != NULL)
            memcpy(pKeyEntryBuffer, pKeyA, 24);          /* PRQA S 3200 */

        if (pKeyB != NULL)
            memcpy(&pKeyEntryBuffer[24], pKeyB, 24);     /* PRQA S 3200 */
    }

    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_DESFIRE_AID_POS]       = pKeyEntry->pDFAid[0];
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_DESFIRE_AID_POS + 1]   = pKeyEntry->pDFAid[1];
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_DESFIRE_AID_POS + 2]   = pKeyEntry->pDFAid[2];
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_DESFIRE_KEYNUM_POS]    = pKeyEntry->bDFKeyNo;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_REFNUM_CEK_POS]        = pKeyEntry->bKeyNoCEK;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_KEYVER_CEK_POS]        = pKeyEntry->bKeyVCEK;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_REFNUM_KUC_POS]        = pKeyEntry->bRefNoKUC;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_CONFIG_SET_POS]        = pKeyEntry->pSet[0];
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_CONFIG_SET_POS + 1]    = pKeyEntry->pSet[1];
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_KEY_A_VERSION_POS]     = pKeyEntry->bVersionKeyA;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_KEY_B_VERSION_POS]     = pKeyEntry->bVersionKeyB;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_KEY_C_VERSION_POS]     = pKeyEntry->bVersionKeyC;
    pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_CONFIG_SET2_POS]       = pKeyEntry->bExtSet;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}


#endif /* NXPBUILD__PH_KEYSTORE_SAMAV2 */
