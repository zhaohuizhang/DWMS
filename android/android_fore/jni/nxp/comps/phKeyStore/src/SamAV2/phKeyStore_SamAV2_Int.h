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
* Internal definitions for SamAV2 KeyStore Component of Basic Function Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:27 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHKEYSTORE_SAMAV2_INT_H
#define PHKEYSTORE_SAMAV2_INT_H

#include <ph_Status.h>
#include <phKeyStore.h>

/** \addtogroup ph_Private
* @{
*/

#define PH_KEYSTORE_SAMAV2_CMD_3DESDF4_MASK            0x00U
#define PH_KEYSTORE_SAMAV2_CMD_2K3DES_MASK             0x01U
#define PH_KEYSTORE_SAMAV2_CMD_MIFARE_MASK             0x02U
#define PH_KEYSTORE_SAMAV2_CMD_3K3DES_MASK             0x03U
#define PH_KEYSTORE_SAMAV2_CMD_AES128_MASK             0x04U
#define PH_KEYSTORE_SAMAV2_CMD_AES192_MASK             0x05U
#define PH_KEYSTORE_SAMAV2_CMD_2K3DESDF8_MASK          0x06U

#define PH_KEYSTORE_SAMAV2_SET0_ALLOW_DUMP_SESSION_KEY        0x01U
#define PH_KEYSTORE_SAMAV2_SET0_ALLOW_CRYPTO_SECRET_KEY        0x02U
#define PH_KEYSTORE_SAMAV2_SET0_KEEP_IV                        0x04U

#define PH_KEYSTORE_SAMAV2_SET1_ENABLE_HIGH_SEC_LEVEL            0x01U
#define PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH                0x01U
#define PH_KEYSTORE_SAMAV2_SET1_DISABLE_KEY_ENTRY                0x02U
#define PH_KEYSTORE_SAMAV2_SET1_ENABLE_HOST_AUTH_AFTER_RESET    0x04U
#define PH_KEYSTORE_SAMAV2_SET1_ENABLE_LOCK_UNLOCK                0x04U
#define PH_KEYSTORE_SAMAV2_SET1_DISABLE_CHANGE_KEY_PICC            0x08U
#define PH_KEYSTORE_SAMAV2_SET1_DISABLE_DECRYPTION                0x10U
#define PH_KEYSTORE_SAMAV2_SET1_DISABLE_ENCRYPTION                0x20U
#define PH_KEYSTORE_SAMAV2_SET1_DISABLE_VERIFY_MAC                0x40U
#define PH_KEYSTORE_SAMAV2_SET1_DISABLE_GENERATE_MAC            0x80U

#define PH_KEYSTORE_SAMAV2_EXTSET_ALLOW_DUMP_SECRET_KEY            0x08U
#define PH_KEYSTORE_SAMAV2_EXTSET_MANDATE_KEY_DIVERSIFICATION    0x10U

#define PH_KEYSTORE_SAMAV2_CMD_KEYTYPE_MASK                0x38U
#define PH_KEYSTORE_SAMAV2_KEYCLASS_MASK                0x07U

phStatus_t phKeyStore_SamAV2_Int_GetKeyEntry(
                                phKeyStore_SamAV2_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */    
                                uint8_t bKeyNumber,                              /**< [In] number of the key entry to be returned (00h to 7Fh)*/
                                phKeyStore_SamAV2_KeyEntry_t * pKeyEntry         /**< [Out] Key entry structure array containing Key Entry*/
                                );
phStatus_t phKeyStore_SamAV2_Int_ConvertKeyEntryToBuffer(
                                phKeyStore_SamAV2_KeyEntry_t * pKeyEntry,            /**< [In] Pointer to the KeyStore KeyEntry structure. */    
                                uint8_t * pKeyA,                                /**< [In] Pointer to KeyA . */    
                                uint8_t * pKeyB,                                /**< [In] Pointer to KeyB . */    
                                uint8_t * pKeyC,                                /**< [In] Pointer to KeyC . */
                                uint8_t * pKeyEntryBuffer                       /**< [Out] Pointer to the key buffer . */
                                );
                                       
/** @}
* end of ph_Private group
*/

#endif /* PHKEYSTORE_SAMAV2_INT_H */
