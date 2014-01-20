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
* Generic Virtual Card Architecture (R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.5 $
* $Date: Fri Apr 15 09:27:19 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#ifndef PHALVCA_H
#define PHALVCA_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PHAL_VCA_SW

/** \defgroup phalVca_Sw Component : Software
* @{
*/

#define PHAL_VCA_SW_ID  0x01    /**< ID for Software Virtual Card Architecture layer. */

/**
* \brief Structure for Card table
*/
typedef struct
{
    uint16_t wIidIndex;     /**< Index of the associated IID. */
    uint8_t bValid;         /**< Indicates if an entry is valid or not */
    uint8_t pCardData[16];  /**< Card Data received from the Card. */
} phalVca_Sw_CardTableEntry_t;

/**
* \brief Structure for IID table
*/
typedef struct
{
    uint16_t wIidIndex;         /**< Index of the associated IID. */
    uint16_t wKeyEncNumber;     /**< Key number for Encryption key. */
    uint16_t wKeyEncVersion;    /**< Key version for Encryption key. */
    uint16_t wKeyMacNumber;     /**< Key number for MAC key. */
    uint16_t wKeyMacVersion;    /**< Key version for MAC key. */
} phalVca_Sw_IidTableEntry_t;

/**
* \brief VCA Software parameter structure
*/
typedef struct
{
    uint16_t wId;                               /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;                 /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParams;                   /**< Pointer to the parameter structure of the Crypto layer. */
    void * pCryptoRngDataParams;                /**< Pointer to the parameter structure of the CryptoRng layer. */
    phalVca_Sw_CardTableEntry_t * pCardTable;   /**< Pointer to the Card Table of the layer. */
    phalVca_Sw_IidTableEntry_t * pIidTable;     /**< Pointer to the Iid Table of the layer. */
    uint16_t wCurrentCardTablePos;              /**< Position of current entry in the CardTable. */
    uint16_t wNumCardTableEntries;              /**< Number of Card table entries in the table. */
    uint16_t wNumIidTableEntries;               /**< Number of Iid table entries in the table. */
    uint16_t wCurrentIidIndex;                  /**< Current index of the Iid sequence. */
    uint16_t wCurrentIidTablePos;               /**< Position of current entry in the Iid Table. */
} phalVca_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalVca_Sw_Init(
                           phalVca_Sw_DataParams_t * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
                           uint16_t wSizeOfDataParams,                      /**< [In] Specifies the size of the data parameter structure. */
                           void * pPalMifareDataParams,                     /**< [In] Pointer to the parameter structure of the palMifare layer. */
                           void * pKeyStoreDataParams,                      /**< [In] Pointer to the parameter structure of the keyStore layer. */
                           void * pCryptoDataParams,                        /**< [In] Pointer to the parameter structure of the Crypto layer. */
                           void * pCryptoRngDataParams,                     /**< [In] Pointer to the parameter structure of the CryptoRng layer. */
                           phalVca_Sw_IidTableEntry_t * pIidTableStorage,   /**< [In] Pointer to the Iid Table storage for the layer. */
                           uint16_t wNumIidTableStorageEntries,             /**< [In] Number of possible Iid table entries in the storage. */
                           phalVca_Sw_CardTableEntry_t * pCardTableStorage, /**< [In] Pointer to the Card Table storage for the layer. */
                           uint16_t wNumCardTableStorageEntries             /**< [In] Number of possible Card table entries in the storage. */
                           );

/** @} */
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2

/** \defgroup phalVca_SamAV2_X Component : SamAV2_X
* @{
*/

#define PHAL_VCA_SAMAV2_ID  0x02    /**< ID for SamAV2 Virtual Card Architecture layer. */

/**
* \brief Structure for IID table
* IMPORTANT: Structure is reused to store RNDQ! 
*/
typedef struct
{
    uint16_t wIidIndex;     /**< index of the corresponding IID. */
    uint8_t bNumKeyDuos;    /**< Amount of key duos provided in that VCSL sequence */
    uint8_t pCardData[10];  /**< Card Data received from the Card. */  
} phalVca_SamAV2_X_CardTableEntry_t;

typedef struct 
{
    uint8_t pIid[16];   /**< Iid sent to the card. */      
}phalVca_SamAV2_X_IidTable_t;

typedef struct 
{
    uint8_t pKeys[4];   /**< Keys sent to the card. */    
}phalVca_SamAV2_X_KeysTable_t;

/**
* \brief Sw AL-VCA parameter structure
*/
typedef struct
{
    uint16_t wId;                                               /**< Layer ID for this component, NEVER MODIFY! */
    phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams;   /**< Pointer to the parameter structure of the palMifare layer. */
    phhalHw_SamAV2_DataParams_t * pSamHal;                      /**< Pointer to the parameter structure of the Sam HAL. */
    phalVca_SamAV2_X_CardTableEntry_t * pCardTable;             /**< Pointer to the Card Table of the layer. */
    phalVca_SamAV2_X_IidTable_t * pIidTable;                    /**< Pointer to the IidTable of the layer. */
    phalVca_SamAV2_X_KeysTable_t * pKeysTable;                  /**< Pointer to the IidTable of the layer. */
    uint16_t wCurrentCardTablePos;                              /**< Position of current entry in the CardTable. */
    uint16_t wNumCardTableEntries;                              /**< Number of Card table entries in the table. */
    uint16_t wCurrentIidKeysTablePos;                           /**< Current position in IID array. */
    uint16_t wNumIidKeysTableEntries;                           /**< Indicates the size of the key table. */
    uint8_t pPcdCaps[4];                                        /**< LenPcdCaps || PCD Caps. */
} phalVca_SamAV2_X_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalVca_SamAV2_X_Init(
                                 phalVca_SamAV2_X_DataParams_t * pDataParams,               /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                                /**< [In] Specifies the size of the data parameter structure. */
                                 phhalHw_SamAV2_DataParams_t * pSamHal,                     /**< [In] Pointer to the parameter structure of the Sam HAL.  */
                                 phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams,  /**< [In] Pointer to the parameter structure of the palMifare layer. */
                                 phalVca_SamAV2_X_IidTable_t * pIidTable,                   /**< [In] Pointer to the IidTable of the layer */
                                 phalVca_SamAV2_X_KeysTable_t * pKeysTable,                 /**< [In] Pointer to the KeysTable of the layer  */
                                 uint16_t wNumIidKeysTableEntries,                          /**< [In] Indicates the size of the key/Iid table. */  
                                 phalVca_SamAV2_X_CardTableEntry_t * pCardTableStorage,     /**< [In] Pointer to the Card Table storage for the layer. */
                                 uint16_t wNumCardTableStorageEntries                       /**< [In] Number of possible Card table entries in the storage. */
                                 );

/** @} */
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X

/** \defgroup phalVca_SamAV2 Component : SamAV2
* @{
*/

#define PHAL_VCA_SAMAV2_X_ID    0x03    /**< ID for SamAV2_X Virtual Card Architecture layer. */

/**
* \brief Structure for IID table
*/
typedef struct
{
    uint16_t wIidIndex;     /**< Index of the corresponding IID. */
    uint8_t bNumKeyDuos;    /**< Amount of key duos provided in that VCSL sequence */
    uint8_t pCardData[10];  /**< Card Data received from the Card. */  
} phalVca_SamAV2_CardTableEntry_t;

/**
* \brief Structure for KeyDuo Storage
*/
typedef struct
{
    uint8_t bKeyEncNumber;  /**< Number of encryption Key. */  
    uint8_t bKeyEncVersion; /**< Version of encryption Key. */  
    uint8_t bKeyMacNumber;  /**< Number of MAC Key. */  
    uint8_t bKeyMacVersion; /**< Version of MAC Key. */  
} phalVca_SamAV2_KeyDuos_t;

/**
* \brief Sw AL-VCA parameter structure
*/
typedef struct
{
    uint16_t wId;                                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                    /**< Pointer to the parameter structure of the palMifare layer. */
    phhalHw_SamAV2_DataParams_t * pSamHal;          /**< Pointer to the parameter structure of the Sam HAL. */
    phalVca_SamAV2_CardTableEntry_t * pCardTable;   /**< Pointer to the Card Table of the layer */
    uint16_t wCurrentCardTablePos;                  /**< Position of current entry in the CardTable */
    uint16_t wNumCardTableEntries;                  /**< Number of Card table entries in the table */
    uint8_t * pRndq;                                /**< The memory storage is used to store RNDQ - Size: wNumCardTableEntries*12 */
    phalVca_SamAV2_KeyDuos_t * pKeyDuos;            /**< The memory storage is used to store Key duos. */
    uint8_t bNumKeyDuos;                            /**< Indicates the size of the key duos array. */
    uint8_t pPcdCaps[4];                            /**< LenPcdCaps || PCD Caps */
} phalVca_SamAV2_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalVca_SamAV2_Init(
                               phalVca_SamAV2_DataParams_t * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
                               uint16_t wSizeOfDataParams,                          /**< [In] Specifies the size of the data parameter structure. */
                               phhalHw_SamAV2_DataParams_t * pSamHal,               /**< [In] Pointer to the parameter structure of the Sam HAL.  */
                               void * pPalMifareDataParams,                         /**< [In] Pointer to the parameter structure of the palMifare layer. */
                               uint8_t * pRndq,                                     /**< [In] RndQ buffer. */
                               phalVca_SamAV2_KeyDuos_t * pKeyDuos,                 /**< [In] KeyDuo buffer. */
                               uint8_t bNumKeyDuos,                                 /**< [In] Number of KeyDuos. */
                               phalVca_SamAV2_CardTableEntry_t * pCardTableStorage, /**< [In] Pointer to the Card Table storage for the layer. */
                               uint16_t wNumCardTableStorageEntries                 /**< [In] Number of possible Card table entries in the storage. */
                               );

/** @} */
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

#ifdef NXPBUILD__PHAL_VCA

/** \defgroup phalVca Virtual Card Architecture (R)
* \brief These Components implement the Virtual Card Architecture (R) commands.
* @{
*/

/** \name Custom Error Codes
*/
/*@{*/
#define PHAL_VCA_ERR_CMD_INVALID    (PH_ERR_CUSTOM_BEGIN+0) /**< VCA Invalid Command Error. */
#define PHAL_VCA_ERR_FORMAT         (PH_ERR_CUSTOM_BEGIN+1) /**< VCA Format Error. */
#define PHAL_VCA_ERR_AUTH           (PH_ERR_CUSTOM_BEGIN+2) /**< VCA MAC Error. */
#define PHAL_VCA_ERR_GEN            (PH_ERR_CUSTOM_BEGIN+3) /**< VCA GEN Error. */
/*@}*/

/**
* \name Virtual Card
*/
/*@{*/

/**
* \brief Performs a Virtual Card Support command (MIFARE Plus X only).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_VcSupport(
                             void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t * pIid,            /**< [In] Installation Identifier (16 bytes). */
                             uint16_t wKeyEncNumber,    /**< [In] Key number of the ENC key associated to the Iid. */
                             uint16_t wKeyEncVersion,   /**< [In] Key version of the ENC key associated to the Iid. */
                             uint16_t wKeyMacNumber,    /**< [In] Key number of the MAC key associated to the Iid. */
                             uint16_t wKeyMacVersion    /**< [In] Key version of the MAC key associated to the Iid. */
                             );

/**
* \brief Perform a Virtual Card Support (Last) command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_VcSupportLast(
                                 void * pDataParams,            /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t * pIid,                /**< [In] Installation Identifier (16 bytes). */
                                 uint8_t bLenCap,               /**< [In] Length of the PCDcaps (0-6). */
                                 uint8_t * pPcdCapabilities,    /**< [In] PCD Capabilities (bLenCap bytes), ignored if bLenCap == 0. */
                                 uint16_t wKeyEncNumber,        /**< [In] Key number of the ENC key used in VCSL command. */
                                 uint16_t wKeyEncVersion,       /**< [In] Key version of the ENC key used in VCSL command. */
                                 uint16_t wKeyMacNumber,        /**< [In] Key number of the MAC key used in VCSL command. */
                                 uint16_t wKeyMacVersion        /**< [In] Key version of the MAC key used in VCSL command. */
                                 );

/**
* \brief Start Card Selection.
*
* This command is used to start a VCS/VCSL sequence.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_StartCardSelection (
                                       void * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                                       );

/**
* \brief finalize Card Selection.
*
* This command is used to finalize a VCS/VCSL sequence. The number of sucessfully detected IIDs is returned
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_FinalizeCardSelection (
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint16_t * pNumValidIids    /**< [Out] Number of valid IIDs detected during VCS/VCSL sequence execution. */
    );

/**
* \brief Retrieve card information.
*
* This command retrieves card information like UID and Info byte and the associated IID
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_GetIidInfo(
                              void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wValidIidIndex,  /**< [In] index of valid IID, needs to be < pNumValidIids in FinalizeCardSelection call. */
                              uint16_t * pIidIndex,     /**< [Out] corresponding IID to the key pair where the MAC was matching in VCSL command call. */
                              uint8_t * pVcUidSize,     /**< [Out] Size of the VC UID (4 or 7 bytes). */
                              uint8_t * pVcUid,         /**< [Out] VC UID (pVcUidSize bytes). */
                              uint8_t * pInfo,          /**< [Out] Info byte. */
                              uint8_t * pPdCapabilities /**< [Out] PD Capabilities (2 bytes). */
                              );

/**
* \brief Performs a Select Virtual Card command (MIFARE Plus X only).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_SelectVc(
                            void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
                            uint16_t wValidIidIndex,    /**< [In] index of valid IID, needs to be < pNumValidIids in FinalizeCardSelection call. */
                            uint16_t wKeyNumber,        /**< [In] Key number of the MAC key used in SVC command. */
                            uint16_t wKeyVersion        /**< [In] Key version of the MAC key used in SVC command. */
                            );

/**
* \brief Performs a Deselect Virtual Card command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_DeselectVc(
                              void * pDataParams    /**< [In] Pointer to this layer's parameter structure. */
                              );

/** @} */

/**
* \name Proximity Check
*/
/*@{*/

/**
* \brief Performs the whole Proximity Check command chain.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_ProximityCheck(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bGenerateRndC,    /**< [In] \c 0: RndC provided; \c 1: generate RndC; */
                                  uint8_t * pRndC,          /**< [In] Provided RndC (7 bytes), ignored if bGenerateRndC == 1. */
                                  uint8_t bPps1,            /**< [In] Communication Speed byte (PPS1). */
                                  uint8_t bNumSteps,        /**< [In] Number of ProximityCheck cycles; RFU, must be 1. */
                                  uint16_t wKeyNumber,      /**< [In] Proximity Check MAC Key number. */
                                  uint16_t wKeyVersion,     /**< [In] Proximity Check MAC Key version. */
                                  uint8_t * pUsedRndC       /**< [Out] Used RndC (7 bytes), can be NULL. */
                                  );

/** @} */

/** @} */
#endif /* NXPBUILD__PHAL_VCA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALVCA_H */
