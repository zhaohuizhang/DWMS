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
* Generic KeyStore Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:19 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHKEYSTORE_H
#define PHKEYSTORE_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PH_KEYSTORE_SW

/** \defgroup phKeyStore_Sw Component : Software
* \brief Keystore implementation in Software.
* @{
*/

#define PH_KEYSTORE_SW_ID               0x01    /**< ID for Software KeyStore component. */
#define PH_KEYSTORE_SW_MAX_KEY_SIZE     32U     /**< Maximum size of a Key. */

/**
* \brief Sw KeyVersionPair structure
*/
typedef struct
{
    uint8_t pKey[PH_KEYSTORE_SW_MAX_KEY_SIZE];  /**< Array containing a Key  */    
    uint16_t wVersion;                          /**< Versions related to  the Key*/
} phKeyStore_Sw_KeyVersionPair_t;

/**
* \brief Sw KeyEntry structure
*/
typedef struct
{ 
    uint16_t wKeyType;      /**< Type of the keys in pKeys. */
    uint16_t wRefNoKUC;     /**< Key usage counter number of the keys in pKeys. */ 
} phKeyStore_Sw_KeyEntry_t;

/**
* \brief Sw KeyUsageCounter structure
*/
typedef struct
{ 
    uint32_t dwLimit;   /**< Limit of the Key Usage Counter. */
    uint32_t dwCurVal;  /**< Current Value of the KUC. */    
} phKeyStore_Sw_KUCEntry_t;

/**
* \brief Sw parameter structure
*/
typedef struct
{
    uint16_t  wId;                                      /**< Layer ID for this component, NEVER MODIFY! */    
    phKeyStore_Sw_KeyEntry_t * pKeyEntries;             /**< Key entry storage, size = sizeof(#phKeyStore_Sw_KeyEntry_t) * wNumKeyEntries. */
    phKeyStore_Sw_KeyVersionPair_t * pKeyVersionPairs;  /**< Key version pairs, size = sizeof(#phKeyStore_Sw_KeyVersionPair_t)* wNumKeyEntries * wNumVersions. */    
    uint16_t wNoOfKeyEntries;                           /**< Number of key entries in that storage. */
    uint16_t wNoOfVersions;                             /**< Number of versions in each key entry. */
    phKeyStore_Sw_KUCEntry_t * pKUCEntries;             /**< Key usage counter entry storage, size = sizeof(#phKeyStore_Sw_KUCEntry_t) * wNumKUCEntries. */
    uint16_t wNoOfKUCEntries;                           /**< Number of Key usage counter entries. */     
} phKeyStore_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phKeyStore_Sw_Init(
                              phKeyStore_Sw_DataParams_t * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wSizeOfDataParams,                           /**< [In] Specifies the size of the data parameter structure. */
                              phKeyStore_Sw_KeyEntry_t * pKeyEntries,               /**< [In] Pointer to a storage containing the key entries. */ 
                              uint16_t wNoOfKeyEntries,                             /**< [In] size of pKeyEntries. */ 
                              phKeyStore_Sw_KeyVersionPair_t * pKeyVersionPairs,    /**< [In] Pointer to a storage containing the key version pairs. */ 
                              uint16_t wNoOfVersionPairs,                           /**< [In] amount of key versions available in each key entry. */ 
                              phKeyStore_Sw_KUCEntry_t * pKUCEntries,               /**< [In] Key usage counter entry storage, size = sizeof(phKeyStore_Sw_KUCEntry_t) * wNumKUCEntries */
                              uint16_t wNoOfKUCEntries                              /**< [In] Number of Key usage counter entries. */  
                              );

/** @} */
#endif /* NXPBUILD__PH_KEYSTORE_SW */

#ifdef NXPBUILD__PH_KEYSTORE_RC663

#include <phhalHw.h>

/** \defgroup phKeyStore_Rc663 Component : Rc663
* @{
*/

#define PH_KEYSTORE_RC663_ID            0x02    /**< ID for Rc663 KeyStore component. */
#define PH_KEYSTORE_RC663_NUM_KEYS      0x80U   /**< Maximum number of keys storable in Rc663. */
#define PH_KEYSTORE_RC663_NUM_VERSIONS  01U     /**< Amount of versions for each key entry in the key store. */

/**
* \brief Rc663 parameter structure
*/
typedef struct
{
    uint16_t  wId;          /**< Layer ID for this component, NEVER MODIFY! */    
    void * pHalDataParams;  /**< Pointer to the parameter structure of the underlying layer.*/
} phKeyStore_Rc663_DataParams_t;


/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phKeyStore_Rc663_Init(
                                 phKeyStore_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                                 void * pHalDataParams                          /**< [In] Pointer to a HAL based on NXP RC663 IC. */
                                 );

/** @} */
#endif /* NXPBUILD__PH_KEYSTORE_RC663 */

#ifdef NXPBUILD__PH_KEYSTORE_RC632

#include <phhalHw.h>

/** \defgroup phKeyStore_Rc632 Component : Rc632
* @{
*/

#define PH_KEYSTORE_RC632_ID                0x03    /**< ID for Rc632 KeyStore component. */
#define PH_KEYSTORE_RC632_NUM_KEYS          16U     /**< Maximum number of keys storable in Rc632. */
#define PH_KEYSTORE_RC632_NUM_VERSIONS      01U     /**< Amount of versions for each key entry in the key store. */

/**
* \brief Rc632 parameter structure
*/
typedef struct
{
    uint16_t  wId;          /**< Layer ID for this component, NEVER MODIFY! */    
    void * pHalDataParams;  /**< Pointer to the parameter structure of the underlying layer.*/
} phKeyStore_Rc632_DataParams_t;


/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phKeyStore_Rc632_Init(
                                 phKeyStore_Rc632_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                                 void * pHalDataParams                          /**< [In] Pointer to a HAL based on NXP RC632 IC. */
                                 );

/** @} */
#endif /* NXPBUILD__PH_KEYSTORE_RC632 */

#ifdef NXPBUILD__PH_KEYSTORE_RD710

#include <phhalHw.h>

/** \defgroup phKeyStore_Rd710 Component : Rd710
* @{
*/

#define PH_KEYSTORE_RD710_ID        0x04    /**< ID for Rd710 KeyStore component. */

/**
* \brief Rd710 parameter structure
*/
typedef struct
{
    uint16_t  wId;                        /**< Layer ID for this component, NEVER MODIFY! */    
    phhalHw_Rd710_DataParams_t * pHalDataParams; /**< Pointer to the parameter structure of the underlying layer.*/
} phKeyStore_Rd710_DataParams_t;


/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phKeyStore_Rd710_Init(
                                phKeyStore_Rd710_DataParams_t * pDataParams,
                                uint16_t wSizeOfDataParams,
                                phhalHw_Rd710_DataParams_t * pHalDataParams 
                                );

/** @} */
#endif /* NXPBUILD__PH_KEYSTORE_RD710 */

#ifdef NXPBUILD__PH_KEYSTORE_SAMAV2

#include <phhalHw.h>

/** \defgroup phKeyStore_SamAV2 Component : SamAV2
* \brief SamAV2 implementation of the phSam interface
* @{
*/

#define PH_KEYSTORE_SAMAV2_ID               0x05    /**< ID for SamAV2 KeyStore component. */

#define PH_KEYSTORE_SAMAV2_AV1_MODE         0x01    /**< Define the AV1 mode of the KeyStore */
#define PH_KEYSTORE_SAMAV2_AV2_MODE         0x02    /**< Define the AV2 mode of the KeyStore */

/**
* \name Key Classes
*/
/*@{*/
#define PH_KEYSTORE_SAMAV2_KEY_CLASS_HOST                0x00U    /**< Host key: has to be either AES128 or AES192. */
#define PH_KEYSTORE_SAMAV2_KEY_CLASS_PICC                0x01U    /**< PICC key: has to be either AES128, single DES, 2 Key Triple DES, 3 Key Triple DES or MIFARE. */
#define PH_KEYSTORE_SAMAV2_KEY_CLASS_OFFLINE_CHANGE      0x02U    /**< Offline change key: has to be either AES128 or AES192. */
#define PH_KEYSTORE_SAMAV2_KEY_CLASS_OFFLINE_CRYPTO      0x04U    /**< Offline crypto key: has to be either AES128, AES192, single DES with CRC32 protection, 2 Key Triple DES with CRC32 protection or 3 Key Triple DES. */
/*@}*/

/**
* \name DES Key Options
*/
/*@{*/
#define PH_KEYSTORE_SAMAV2_DES_OPTION_DESFIRE4            0x00    /**< DESFire 4 compatibility mode. */
#define PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC16           0x01    /**< ISO 10116 mode with CRC16 protection and 4 bytes MAC. */
#define PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC32           0x02    /**< ISO 10116 mode with CRC32 protection and 8 bytes MAC. */
/*@}*/

/**
* \name KeyStore Configs
*/
/*@{*/
#define PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_DUMP_SESSION_KEY        0x0000U     /**< Enable or Disable SAM_ChangeKeyMIFARE and SAM_DumpSessionKey command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_CRYPTO_SECRET_KEY        0x0001U     /**< Enable or Disable crypto based on the secret key for AV1 mode. */
#define PH_KEYSTORE_SAMAV2_CONFIG_ALLOW_DUMP_SECRET_KEY            0x0002U     /**< Enable or Disable SAM_DumpSecretKey command for AV2 mode. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEEP_IV                        0x0003U     /**< Enable or Disable the reset of init vector after a crypto command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HIGH_SEC_LEVEL            0x0004U     /**< Enable or Disable Higher Command Security Level for AV1 Master Key. */
#define PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HOST_AUTH                0x0005U     /**< Enable or Disable Host Authentication for AV2 mode. */
#define PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_HOST_AUTH_AFTER_RESET    0x0006U     /**< Enable or Disable Host Authentication after reset for AV1 Master Key. */
#define PH_KEYSTORE_SAMAV2_CONFIG_ENABLE_LOCK_UNLOCK            0x0007U     /**< Enable or Disable LockUnlock for AV2 mode. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_CHANGE_KEY_PICC        0x0008U     /**< Enable or Disable SAM_ChangeKeyPICC command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_ENCRYPTION            0x0009U     /**< Enable or Disable SAM_DecipherData command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_DECRYPTION            0x000AU     /**< Enable or Disable SAM_EncipherData command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_VERIFY_MAC            0x000BU     /**< Enable or Disable SAM_VerifyMAC command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_GENERATE_MAC            0x000CU     /**< Enable or Disable SAM_GenerateMAC command. */
#define PH_KEYSTORE_SAMAV2_CONFIG_MANDATE_KEY_DIVERSIFICATION    0x000DU     /**< Mandate or not key diversification. */
#define PH_KEYSTORE_SAMAV2_CONFIG_CHANGE_KEY_PLAIN                0x000EU     /**< Enable plain exchange of ChangeKey functions for AV1 mode. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYCLASS                        0x000FU     /**< Key Class used in key store. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_CEK                        0x0010U     /**< Key Number of Change Entry key. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYV_CEK                        0x0011U     /**< Key Version of Change Entry key. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_CKUC                    0x0012U     /**< Key Number of Change  KUC. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYV_CKUC                        0x0013U     /**< Key Version of Change KUC. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DISABLE_KEY_ENTRY                0x0014U     /**< Enable or Disable Key Entry. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DF_AID                        0x0015U        /**< DESFire application ID. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DF_KEY_NO                        0x0016U        /**< DESFire key number. */
#define PH_KEYSTORE_SAMAV2_CONFIG_REF_NO_KUC                    0x0017U        /**< Reference number of key usage counter. */
#define PH_KEYSTORE_SAMAV2_CONFIG_DES_KEY_OPTION                0x0018U        /**< Option for single DES and 2 Key Triple DES keys. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_MF_DIV_A                0x0019U        /**< Key Number used for MIFARE key A diversification. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYV_MF_DIV_A                    0x001AU        /**< Key Version used for MIFARE key A diversification. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYNO_MF_DIV_B                0x001BU        /**< Key Number used for MIFARE key B diversification. */
#define PH_KEYSTORE_SAMAV2_CONFIG_KEYV_MF_DIV_B                    0x001CU        /**< Key Version used for MIFARE key B diversification. */
/*@}*/

/**
* \brief Sam parameter structure
*/
typedef struct
{
    uint16_t  wId;                        /**< Layer ID for this component, NEVER MODIFY! */    
    phhalHw_SamAV2_DataParams_t * pHalDataParams;                /**< Pointer to the parameter structure of the underlying layer.*/
    uint8_t pSet[2];                    /**< Configuration settings. */
    uint8_t bExtSet;                    /**< Extended configuration settings. */
    uint8_t bChangeKeyPlain;            /**< Use plain transmission for ChangeKey operations in AV1 mode. */
    uint8_t pDFAid[3];                    /**< DESFire application ID. */
    uint8_t    bDFKeyNo;                    /**< DESFire key number. */
    uint8_t bKeyNoCEK;                  /**< Key Number of Change Entry Key. */
    uint8_t bKeyVCEK;                   /**< Key Version of Change Entry Key. */
    uint8_t bRefNoKUC;                    /**< Reference number of key usage counter. */
    uint8_t bKeyNoCKUC;                 /**< Key Number of Change KUC. */
    uint8_t bKeyVCKUC;                  /**< Key Version of Change KUC. */
    uint8_t bKeyNoMfDivA;                /**< Key Number used for MIFARE key A diversification (has to point to a DES key). */
    uint8_t bKeyVMfDivA;                /**< Key Version used for MIFARE key A diversification (has to point to a DES key). */
    uint8_t bKeyNoMfDivB;                /**< Key Number used for MIFARE key B diversification (has to point to a DES key). */
    uint8_t bKeyVMfDivB;                /**< Key Version used for MIFARE key B diversification (has to point to a DES key). */
    uint8_t b2K3DESOption;                /**< Option for single DES and 2 Key Triple DES keys. Can be set either to #PH_KEYSTORE_SAMAV2_DES_OPTION_DESFIRE4, #PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC16 or #PH_KEYSTORE_SAMAV2_DES_OPTION_ISO_CRC32 */
} phKeyStore_SamAV2_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phKeyStore_SamAV2_Init(
                               phKeyStore_SamAV2_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */ 
                               uint16_t wSizeOfDataParams,                      /**< [In] Specifies the size of the data parameter structure. */
                               phhalHw_SamAV2_DataParams_t * pHalDataParams     /**< [In] Pointer to the parameter structure of the underlying layer.*/
                               );

/** @} */
#endif /* NXPBUILD__PH_KEYSTORE_SAMAV2 */

#ifdef NXPBUILD__PH_KEYSTORE

/** \defgroup phKeyStore KeyStore
*
* \brief This is only a wrapper layer to abstract the different KeyStore implementations.
* @{
*/

#define PH_KEYSTORE_INVALID_ID      0xFFFFU  /**< ID used for various parameters as a invalid default **/
#define PH_KEYSTORE_DEFAULT_ID      0x0000   /**< ID used for various parameters as a default **/

/**
* \name Key Types
*/
/*@{*/
#define PH_KEYSTORE_KEY_TYPE_AES128         0x00U   /**< AES 128 Key [16]. */
#define PH_KEYSTORE_KEY_TYPE_AES192         0x01U   /**< AES 192 Key [24]. */
#define PH_KEYSTORE_KEY_TYPE_AES256         0x02U   /**< AES 256 Key [32]. */
#define PH_KEYSTORE_KEY_TYPE_DES            0x03U   /**< DES Single Key.   */
#define PH_KEYSTORE_KEY_TYPE_2K3DES         0x04U   /**< 2 Key Triple Des. */
#define PH_KEYSTORE_KEY_TYPE_3K3DES         0x05U   /**< 3 Key Triple Des. */
#define PH_KEYSTORE_KEY_TYPE_MIFARE         0x06U   /**< MIFARE (R) Key. */

#define PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE    0x0CU   /**< Size of an MIFARE Key. */
#define PH_KEYSTORE_KEY_TYPE_AES128_SIZE    0x10U   /**< Size of an AES128 Key. */
/*@}*/

/**
* \name KeyStoreConfigs
*/
/*@{*/
#define PH_KEYSTORE_CONFIG_SET_DEFAULT                    0xFFFFU     /**< Reset all bit of SET param. */
/*@}*/

/**
* \brief Format a key entry to a new KeyType.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_FormatKeyEntry(
                                     void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */                           
                                     uint16_t wKeyNo,       /**< [In] KeyEntry number to be Formatted. */
                                     uint16_t wNewKeyType   /**< [In] New Key type of the KeyEntry (predefined type of KeyType).*/ 
                                     );

/**
* \brief Change a key entry at a given version.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_SetKey(
                             void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */                           
                             uint16_t wKeyNo,           /**< [In] Key number of the key to be loaded. */                      
                             uint16_t wKeyVersion,      /**< [In] Key version of the key to be loaded. */
                             uint16_t wKeyType,         /**< [In] Key type of the key to be loaded.*/ 
                             uint8_t * pNewKey,         /**< [In] Pointer to the key itself.*/                                  
                             uint16_t wNewKeyVersion    /**< [In] New Key version of the key to be updated. */
                             );

/**
* \brief Change a key entry at the specified position.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_SetKeyAtPos(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */                           
                                  uint16_t wKeyNo,          /**< [In] Key number of the key to be loaded. */                      
                                  uint16_t wPos,            /**< [In] Key position to be updated. */
                                  uint16_t wKeyType,        /**< [In] Key type of the key to be loaded.*/ 
                                  uint8_t * pNewKey,        /**< [In] Pointer to the key itself.*/                                  
                                  uint16_t wNewKeyVersion   /**< [In] New Key version of the key to be updated. */
                                  );

/**
* \brief Change the KUC of a key entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_SetKUC(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */ 
                             uint16_t wKeyNo,       /**< [In] KeyEntry number. */                      
                             uint16_t wRefNoKUC     /**< [In] Reference Number of the key usage counter used together with that key.*/                                 
                             );

/**
* \brief Change a full key entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_SetFullKeyEntry(
                                      void * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
                                      uint16_t wNoOfKeys,           /**< [In] Number of keys in pNewKeys. */
                                      uint16_t wKeyNo,              /**< [In] Key number of the key to be loaded. */
                                      uint16_t wNewRefNoKUC,        /**< [In] Number of the key usage counter used together with that key. */
                                      uint16_t wNewKeyType,         /**< [In] Key type of the key (if the current keyType of KeyEntry is different, error). */
                                      uint8_t * pNewKeys,           /**< [In] Array of Keys to load. */
                                      uint16_t * pNewKeyVersionList /**< [In] KeyVersionList of the key to be loaded . */
                                      ); 

/**
* \brief Get a key entry information block.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_GetKeyEntry(
                                  void * pDataParams,               /**< [In] Pointer to this layer's parameter structure. */    
                                  uint16_t wKeyNo,                  /**< [In] Key number of the key entry of interest. */                            
                                  uint16_t wKeyVersionBufSize,      /**< [In] Buffer Size of wKeyVersion in Bytes. */
                                  uint16_t * wKeyVersion,           /**< [Out] Array for version information. */ 
                                  uint16_t * wKeyVersionLength,     /**< [Out] Length of valid data in wKeyVersion. */ 
                                  uint16_t * pKeyType               /**< [Out] Type of the key. */
                                  );

/**
* \brief Get a key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_GetKey(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint16_t wKeyNo,       /**< [In] Key number of the key to be retrieved. */
                             uint16_t wKeyVersion,  /**< [In] Key version of the key to be retrieved. */
                             uint8_t bKeyBufSize,   /**< [In] Size of the key buffer. */
                             uint8_t * pKey,        /**< [Out] Pointer to the key itself. */
                             uint16_t * pKeyType    /**< [Out] Type of the key. */
                             );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_SetConfig(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint16_t wConfig,       /**< [In] Configuration Identifier */
                                uint16_t wValue         /**< [In] Configuration Value */
                                );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_SetConfigStr(
                                   void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wConfig,       /**< [In] Configuration Identifier. */
                                   uint8_t *pBuffer,       /**< [In] Buffer containing the configuration string. */
                                   uint16_t wBufferLength  /**< [In] Length of configuration string. */
                                   );

/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_GetConfig(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint16_t wConfig,       /**< [In] Configuration Identifier */
                                uint16_t * pValue       /**< [Out] Configuration Value */
                                );

/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_GetConfigStr(
                                   void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wConfig,        /**< [In] Configuration Identifier */
                                   uint8_t ** ppBuffer,     /**< [Out] Pointer to the buffer containing the configuration string. */
                                   uint16_t * pBufferLength /**< [Out] Amount of valid bytes in the configuration string buffer. */
                                   );

/**
* \brief Change a key usage counter entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_ChangeKUC(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */                           
                                uint16_t wRefNoKUC,     /**< [In] Number of key entry.*/ 
                                uint32_t dwLimit        /**< [In] Limit of the Key Usage Counter*/
                                );

/**
* \brief Obtain a key usage counter entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phKeyStore_GetKUC(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */ 
                             uint16_t wRefNoKUC,    /**< [In] number of the key usage counter to be looked at (00h to 0Fh)*/                               
                             uint32_t * pdwLimit,   /**< [Out] Currently Set Limit in the KUC */
                             uint32_t * pdwCurVal   /**< [Out] Currently set value in the KUC */
                             );

/** @} */
#endif /* NXPBUILD__PH_KEYSTORE */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHKEYSTORE_H */
