/*
* Copyright (c), NXP Semiconductors Bangalore / India
*
*               (C)NXP Semiconductors
* All rights are reserved. Reproduction in whole or in part is
* prohibited without the written consent of the copyright owner.
* NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
* arising from its use.
*/

/** \file
* Generic MIFARE DESFire(R) EV1 Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Wed May 11 15:13:08 2011 $
*
* History:
*  Santosh Araballi: Generated 31. August 2010
*
*/

#ifndef PHALMFDF_H
#define PHALMFDF_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>
#include <ph_TypeDefs.h>
#include <ph_RefDefs.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef NXPBUILD__PHAL_MFDF

/** \defgroup phalMfdf MIFARE DESFire (R)
* \brief These Functions implement the MIFARE DESFire(R) EV1 commands.
* @{
*/

/**
* \name Authentication Modes
*/
/** @{ */
#define PHAL_MFDF_AUTHENTICATE      0x0AU   /**< Backwards compatible Authentication; Mode = 0x0A. */
#define PHAL_MFDF_AUTHENTICATEISO   0x1AU   /**< ISO Authentication; 0x1A. */
#define PHAL_MFDF_AUTHENTICATEAES   0xAAU   /**< AES Authentication; 0xAA. */
/** @} */

/**
* \name Diversification options to be used with ChangeKey and Authenticate
*/
/** @{ */
#define PHAL_MFDF_NO_DIVERSIFICATION            0xFFFFU /**< No diversification. */
#define PHAL_MFDF_CHGKEY_DIV_NEW_KEY            0x0002U /**< Bit 1. Indicating diversification of new key requred. */
#define PHAL_MFDF_CHGKEY_DIV_OLD_KEY            0x0004U /**< Bit 2 indicating old key was diversified. */
#define PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND     0x0008U /**< Bit 3 indicating new key diversification using one rnd. Default is two rounds. */
#define PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND     0x0010U /**< Bit 4 indicating old key diversification using one rnd. Default is two rounds. */
#define PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC        0x0020U /**< Bit 5 indicating key diversification method based on CMAC. Default is Encryption method */

#define PHAL_MFDF_DIV_METHOD_ENCR               PH_CRYPTOSYM_DIV_MODE_DESFIRE       /**< Encryption based method of diversification. */
#define PHAL_MFDF_DIV_METHOD_CMAC               PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS   /**< CMAC based method of diversification. */
#define PHAL_MFDF_DIV_OPTION_2K3DES_FULL        PH_CRYPTOSYM_DIV_OPTION_2K3DES_FULL /**< Encryption based method, full key diversification. */
#define PHAL_MFDF_DIV_OPTION_2K3DES_HALF        PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF /**< Encryption based method, half key diversification. */
/** @} */

/**
* \name Other Options for various Functions
*/
/** @{ */
#define PHAL_MFDF_NOT_AUTHENTICATED     0xFFU   /**< No authentication. */
#define PHAL_MFDF_SET_CONFIG_OPTION1    0x00U   /**< Option 1 Data is the configuration byte. */
#define PHAL_MFDF_SET_CONFIG_OPTION2    0x01U   /**< Option 2 Data is default key version and key. */
#define PHAL_MFDF_SET_CONFIG_OPTION3    0x02U   /**< Option 3 Data is user defined ATS. */
#define PHAL_MFDF_COMMUNICATION_PLAIN   0x00U   /**< Plain mode of communication. */
#define PHAL_MFDF_COMMUNICATION_MACD    0x10U   /**< MAC mode of communication. */
#define PHAL_MFDF_COMMUNICATION_ENC     0x30U   /**< Enciphered mode of communication. */
#define PHAL_MFDF_ENABLE_LIMITEDCREDIT  0x01U   /**< Bit 0 set to 1 to enable Limited credit. */
#define PHAL_MFDF_ENABLE_FREE_GETVALUE  0x02U   /**< Bit 1 set to 1 to enable free getvalue. */
#define PHAL_MFDF_ADDITIONAL_INFO       0x00A1  /**< Option for getconfig to get additional info of a generic error. */
/** @} */

/** \name phalMfdf Custom Error Codes
*/
/** @{ */
#define PHAL_MFDF_NO_CHANGES                    (PH_ERR_CUSTOM_BEGIN+0)     /**< MF DF Response - No changes done to backup files. */
#define PHAL_MFDF_ERR_OUT_OF_EEPROM_ERROR       (PH_ERR_CUSTOM_BEGIN+1)     /**< MF DF Response - Insufficient NV-Memory. */
#define PHAL_MFDF_ERR_NO_SUCH_KEY               (PH_ERR_CUSTOM_BEGIN+2)     /**< MF DF Invalid key number specified. */
#define PHAL_MFDF_ERR_PERMISSION_DENIED         (PH_ERR_CUSTOM_BEGIN+3)     /**< MF DF Current configuration/status does not allow the requested command. */
#define PHAL_MFDF_ERR_APPLICATION_NOT_FOUND     (PH_ERR_CUSTOM_BEGIN+4)     /**< MF DF Requested AID not found on PICC. */
#define PHAL_MFDF_ERR_BOUNDARY_ERROR            (PH_ERR_CUSTOM_BEGIN+5)     /**< MF DF Attempt to read/write data from/to beyond the files/record's limits. */
#define PHAL_MFDF_ERR_COMMAND_ABORTED           (PH_ERR_CUSTOM_BEGIN+6)     /**< MF DF Previous cmd not fully completed. Not all frames were requested or provided by the PCD. */
#define PHAL_MFDF_ERR_COUNT                     (PH_ERR_CUSTOM_BEGIN+7)     /**< MF DF Num. of applns limited to 28. No additional applications possible. */
#define PHAL_MFDF_ERR_DUPLICATE                 (PH_ERR_CUSTOM_BEGIN+8)     /**< MF DF File/Application with same number already exists. */
#define PHAL_MFDF_ERR_FILE_NOT_FOUND            (PH_ERR_CUSTOM_BEGIN+9)     /**< MF DF Specified file number does not exist. */
#define PHAL_MFDF_ERR_PICC_CRYPTO               (PH_ERR_CUSTOM_BEGIN+10)    /**< MF DF Crypto error returned by PICC. */
#define PHAL_MFDF_ERR_PARAMETER_ERROR           (PH_ERR_CUSTOM_BEGIN+11)    /**< MF DF Parameter value error returned by PICC. */
#define PHAL_MFDF_ERR_DF_GEN_ERROR              (PH_ERR_CUSTOM_BEGIN+12)    /**< MF DF DesFire Generic error. Check additional Info. */
#define PHAL_MFDF_ERR_DF_7816_GEN_ERROR         (PH_ERR_CUSTOM_BEGIN+13)    /**< MF DF ISO 7816 Generic error. Check Additional Info. */
/** @} */

/** @} */

#endif /* NXPBUILD__PHAL_MFDF */

#ifdef NXPBUILD__PHAL_MFDF_SW

/**
* \defgroup phalMfdf_Sw Component : Software
*/
/*@{*/

#define PHAL_MFDF_SW_ID 0x01U   /**< ID for Software MF DesFire layer. */

/**
* \brief struct phalMfdf_Sw_DataParams_t
*
*/
typedef struct
{
    uint16_t wId;                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;    /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;     /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParamsEnc;    /**< Pointer to the parameter structure of the Crypto layer for encryption. */
    void * pCryptoRngDataParams;    /**< Pointer to the parameter structure of the CryptoRng layer. */
    void * pHalDataParams;          /**< Pointer to the HAL parameters structure. */
    uint8_t bSessionKey[24];        /**< Session key for this authentication */
    uint8_t bKeyNo;                 /**< key number against which this authentication is done */
    uint8_t bIv[16];                /**< Max size of IV can be 16 bytes */
    uint8_t bAuthMode;              /**< Authenticate (0x0A), AuthISO (0x1A), AuthAES (0xAA) */
    uint8_t pAid[3];                /**< Aid of the currently selected application */
    uint8_t bCryptoMethod;          /**< DES,3DES, 3K3DES or AES */
    uint8_t bWrappedMode;           /**< Wrapped APDU mode. All native commands need to be sent wrapped in ISO 7816 APDUs. */
    uint16_t wCrc;                  /**< 2 Byte CRC initial value in Authenticate mode. */
    uint32_t dwCrc;                 /**< 4 Byte CRC initial value in 0x1A, 0xAA mode. */
    uint16_t wAdditionalInfo;       /**< Specific error codes for Desfire generic errors. */
} phalMfdf_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfdf_Sw_Init(
                            phalMfdf_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                            uint16_t wSizeOfDataParams,             /**< [In] Specifies the size of the data parameter structure */
                            void * pPalMifareDataParams,            /**< [In] Pointer to a palMifare component context. */
                            void * pKeyStoreDataParams,             /**< [In] Pointer to Key Store data parameters. */
                            void * pCryptoDataParamsEnc,            /**< [In] Pointer to a Crypto component context for encryption. */
                            void * pCryptoRngDataParams,            /**< [In] Pointer to a CryptoRng component context. */
                            void * pHalDataParams                   /**< [In] Pointer to the HAL parameters structure. */
                            );

/** @} */

#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2

/**
* \defgroup phalMfdf_SamAV2 Component : SamAV2
*/
/*@{*/

#define PHAL_MFDF_SAMAV2_ID 0x02U   /**< ID for SamAV2 MF DesFire layer. */

/**
* \brief struct phalMfdf_SamAV2_DataParams_t
*
*/
typedef struct{
    uint16_t wId;                                       /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                        /**< Pointer to the parameter structure of the palMifare component. */
    phhalHw_SamAV2_DataParams_t * pHalSamDataParams;    /**< Pointer to the parameter structure of the hal of the SAM layer. */
    void * pHalDataParams;                              /**< Pointer to the HAL layer of the contact less reader. */
    uint8_t bAuthMode;                                  /**< Auth. Mode 0x0A or Ox1A or OxAA. */
    uint8_t bKeyNo;                                     /**< Key number against which authenticated. */
    uint8_t bWrappedMode;                               /**< Wrapped APDU mode. All native commands need to be sent wrapped in ISO 7816 apdus */
    uint8_t pAid[3];                                    /**< Currently selected application Id. */
    uint16_t wAdditionalInfo;                           /**< Specific error codes for Desfire generic errors. */
} phalMfdf_SamAV2_DataParams_t;

/**
* \brief Initialise SamAV2 layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfdf_SamAV2_Init(
                                phalMfdf_SamAV2_DataParams_t * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
                                uint16_t wSizeOfDataParams,                         /**< [In] Specifies the size of the data parameter structure */
                                phhalHw_SamAV2_DataParams_t * pHalSamDataParams,    /**< [In] Pointer to the HAL parameter structure of the SAM. */
                                void * pHalDataParams,                              /**< [In] Pointer to HAL param structure of contactless reader. */
                                void * pPalMifareDataParams                         /**< [In] Pointer to the parameter structure of the underlying palMIFARE layer. */
                                );
/** @} */

#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X

/**
* \defgroup phalMfdf_SamAV2_X Component : SamAV2_X
*/
/*@{*/

#define PHAL_MFDF_SAMAV2_X_ID   0x03U   /**< ID for SamAV2_X MF DesFire layer. */

/**
* \brief SAMAV2_X parameters structure
*
*/
typedef struct{
    uint16_t wId;                                       /**< Layer ID for this component, NEVER MODIFY! */
    phhalHw_SamAV2_DataParams_t * pHalSamDataParams;    /**< Pointer to the parameter structure of the hal of the SAM layer. */
    uint8_t bAuthMode;                                  /**< Auth Mode. 0x0A or 0x1A or 0xAA. */
    uint8_t bKeyNo;                                     /**< Key number against which authenticated. */
    uint8_t bWrappedMode;                               /**< Wrapped APDU mode. All native commands need to be sent wrapped in ISO 7816 apdus. */
    uint8_t pAid[3];                                    /**< Currently selected application Id. */
    uint16_t wAdditionalInfo;                           /**< Specific error codes for Desfire generic errors. */
} phalMfdf_SamAV2_X_DataParams_t;

/**
* \brief Initialise SamAV2_X layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfdf_SamAV2_X_Init(
                                  phalMfdf_SamAV2_X_DataParams_t * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wSizeOfDataParams,                       /**< [In] Specifies the size of the data parameter structure. */
                                  phhalHw_SamAV2_DataParams_t * pHalSamDataParams  /**< [In] Pointer to the HAL parameter structure of the SAM. */
                                  );
/** @} */

#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF

/** \addtogroup phalMfdf
* @{
*/

/**
* \name Security related Commands
*/
/** @{ */

/**
* \brief Performs an Authentication with the specified key number.
*
* The command can be used with DES and 2K3DES keys and performs DESFire4 native
* authentication.
*
* Diversification option (wOption) can be one of \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR  OR \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR | #PHAL_MFDF_DIV_OPTION_2K3DES_HALF OR \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR | #PHAL_MFDF_DIV_OPTION_2K3DES_FULL OR \n
* \li #PHAL_MFDF_DIV_METHOD_CMAC OR \n
* \li #PHAL_MFDF_NO_DIVERSIFICATION \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_Authenticate(
                                 void *pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wOption,      /**< [In] Diversification option. */
                                 uint16_t wKeyNo,       /**< [In] key number in keystore to authenticate with. */
                                 uint16_t wKeyVer,      /**< [In] Key version in the key store. */
                                 uint8_t bKeyNoCard,    /**< [In] Key number on card. */
                                 uint8_t * pDivInput,   /**< [In] Diversification input. Can be NULL. */
                                 uint8_t bDivLen        /**< [In] Length of diversification input max 31B. */
                                 );

/**
* \brief Performs an DES Authentication in ISO CBS send mode.
*
* The command can be used with DES,3DES and 3K3DES keys
*
* Diversification option (wOption) can be one of \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR  OR \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR | #PHAL_MFDF_DIV_OPTION_2K3DES_HALF OR \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR | #PHAL_MFDF_DIV_OPTION_2K3DES_FULL OR \n
* \li #PHAL_MFDF_DIV_METHOD_CMAC OR \n
* \li #PHAL_MFDF_NO_DIVERSIFICATION \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_AuthenticateISO(
                                    void *pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wOption,       /**< [In] Diversification option. */
                                    uint16_t wKeyNo,        /**< [In] key number in keystore to authenticate with. */
                                    uint16_t wKeyVer,       /**< [In] Key version in the key store. */
                                    uint8_t bKeyNoCard,     /**< [In] Key number on card. */
                                    uint8_t * pDivInput,    /**< [In] Diversification input. Can be NULL. */
                                    uint8_t bDivLen         /**< [In] Length of diversification input max 31B. */
                                    );

/**
* \brief Performs an AES Authentication.
*
* The command should be used with AES128 keys
* 
* Diversification option (wOption) can be one of \n
* \li #PHAL_MFDF_DIV_METHOD_ENCR OR \n
* \li #PHAL_MFDF_DIV_METHOD_CMAC OR \n
* \li #PHAL_MFDF_NO_DIVERSIFICATION \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_AuthenticateAES(
                                    void *pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wOption,       /**< [In] Diversification option. */
                                    uint16_t wKeyNo,        /**< [In] key number in keystore to authenticate with. */
                                    uint16_t wKeyVer,       /**< [In] Key version in the key store. */
                                    uint8_t bKeyNoCard,     /**< [In] Key number on card. */
                                    uint8_t * pDivInput,    /**< [In] Diversification input. Can be NULL. */
                                    uint8_t bDivLen         /**< [In] Length of diversification input max 31B. */
                                    );

/**
* \brief Changes the master key settings on PICC and application level.
*
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_ChangeKeySettings(
                                      void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                      uint8_t bKeySettings  /**< [In] New key settings. */
                                      );

/**
* \brief Gets information on the PICC and application master key
* settings. 
*
* \remarks
* In addition it returns the maximum number of keys
* which are configured for the selected application.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetKeySettings(
                                   void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t * pKeySettings   /**< [Out]: 2 bytes key settings. */
                                   );

/**
* \brief Changes any key on the PICC
*
* \remarks
* The key on the PICC is changed to the new key.
* The key type of the application keys cannot be changed. The key type of only the PICC master key
* can be changed.
*
* Diversification option can be one or combination of of \n
* \li #PHAL_MFDF_NO_DIVERSIFICATION OR \n
* \li #PHAL_MFDF_CHGKEY_DIV_NEW_KEY | #PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC \n
* \li #PHAL_MFDF_CHGKEY_DIV_OLD_KEY | #PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC \n
* \li #PHAL_MFDF_CHGKEY_DIV_NEW_KEY | #PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND \n
* \li #PHAL_MFDF_CHGKEY_DIV_OLD_KEY | #PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND \n
* \li #PHAL_MFDF_CHGKEY_DIV_NEW_KEY | #PHAL_MFDF_CHGKEY_DIV_OLD_KEY \n
* \li #PHAL_MFDF_CHGKEY_DIV_NEW_KEY | #PHAL_MFDF_CHGKEY_DIV_OLD_KEY | #PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC \n
* \li #PHAL_MFDF_CHGKEY_DIV_NEW_KEY | #PHAL_MFDF_CHGKEY_DIV_OLD_KEY | #PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND | #PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_ChangeKey(
        void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,       /**< [In] Diversification option. */
        uint16_t wOldKeyNo,     /**< [In] Old key number in keystore. */
        uint16_t wOldKeyVer,    /**< [In] Old key version in keystore. */
        uint16_t wNewKeyNo,     /**< [In] New key number in keystore. */
        uint16_t wNewKeyVer,    /**< [In] New key version in keystore. */
        uint8_t bKeyNoCard,     /**< [In] One byte Card number to be changed. */
        uint8_t * pDivInput,    /**< [In] Diversification input. Can be NULL. */
        uint8_t bDivLen         /**< [In] Length of diversification input max 31B. */
        );
/**
* \brief Reads out the current key version of any key stored on the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetKeyVersion(
                                  void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bKeyNo,       /**< [In]: 1 byte Card key number. */
                                  uint8_t * pKeyVersion /**< [Out]: 1 byte key version. */
                                  );

/** @} */

/**
* \name PICC level Commands
*/
/** @{ */

/**
* \brief Creates new applications on the PICC
*
* bOption value can be \n
* \li 01 meaning wISOFileId is supplied \n
* \li 02 meaning pISODFName is present \n
* \li 03 meaning both wISOFileId and pISODFName are present \n
* \li 00 meaning both not present
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateApplication(
                                      void * pDataParams,       /**< [In] Pointer to this layers param structure */
                                      uint8_t bOption,          /**< [In] Option parameter. */                                                                
                                      uint8_t * pAid,           /**< [In] array of 3 bytes. */
                                      uint8_t bKeySettings1,    /**< [In] 1 byte. */
                                      uint8_t bKeySettings2,    /**< [In] 1 byte. */
                                      uint8_t * pISOFileId,     /**< [In] 2 btyes ISO File ID. */
                                      uint8_t * pISODFName,     /**< [In] 1 to 16 Bytes. Can also be NULL. */
                                      uint8_t bISODFNameLen     /**< [In] Size of pISODFName if that is present. */
                                      );

/**
* \brief Permanently deactivates applications on the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_DeleteApplication(
                                      void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                      uint8_t * pAid        /**< [In] 3 byte array. LSB First. */
                                      );

/**
* \brief Returns application identifiers of all applications on the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetApplicationIDs(
                                      void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                      uint8_t * pAidBuff,   /**< [Out] should be = 96B. (3 * 28) */
                                      uint8_t * pNumAid     /**< [Out] Number of AIDs read. */
                                      );

/**
* \brief Returns the Dedicated File(DF) names
*
* \remarks
* The pDFBuffer will be filled with 3 byte AID + 2 byte ISO Fid + one DF Name 
* at a time.If there are more DFs, then status PH_ERR_SUCCESS_CHAINING is returned.
* The caller should call this again with bOption = PH_EXCHANGE_RXCHAINING.
* 
* CAUTION: This should not be called with AES or ISO authentication
* DOING SO WILL DAMAGE THE DESFIRE Card
* \n
* bOption can be one of \n
* \li #PH_EXCHANGE_DEFAULT \n
* \li #PH_EXCHANGE_RXCHAINING
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING More DF Names to be returned
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetDFNames(
                               void * pDataParams,      /**< [In] Pointer to this layers param structure. */
                               uint8_t bOption,         /**< [In] PH_EXCHANGE_DEFAULT or PH_EXCHANGE_RXCHAINING. */
                               uint8_t * pDFBuffer,     /**< [Out] One DF Name at a time. Should be 21(3+2+16) bytes long. */
                               uint8_t * bDFInfoLen     /**< [Out] The size of the DF info returned in this frame. */
                               );

/**
* \brief Selects one particular application on the PICC for
* further access
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_SelectApplication(
                                      void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                      uint8_t * pAid        /**< [In] 3 byte AID. LSB First. */
                                      );

/**
* \brief Releases the PICC user memory
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_FormatPICC(
                               void * pDataParams   /**< [In] Pointer to this layers param structure. */
                               );

/**
* \brief Returns manufacturing related data of the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetVersion(
                               void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                               uint8_t * pVerInfo   /**< [Out] 28bytes of version info. User has to parse this. */
                               );

/**
* \brief Returns free memory available on the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_FreeMem(
                               void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                               uint8_t * pMemInfo   /**< [Out] 3 bytes memory info. LSB first. */
                               );

/**
* \brief Configures the card and pre personalizes the card with a key, defines
* if the UID or the random ID is sent back during communication setup and
* configures the ATS string.
*
* \remarks
* bOption can be one of \n
* \li #PHAL_MFDF_SET_CONFIG_OPTION1 \n
* \li #PHAL_MFDF_SET_CONFIG_OPTION2 \n
* \li #PHAL_MFDF_SET_CONFIG_OPTION3 \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phalMfdf_SetConfiguration(
                                     void * pDataParams,    /**< [In] Pointer to this layers param structure. */
                                     uint8_t bOption,       /**< [In] Option parameter. */
                                     uint8_t * pData,       /**< [In] max predictible is 24 bytes + 1 byte for
                                                            option 0x01. Minimum size is 1 byte for option 0x00
                                                            Unspecified size for option 0x02, if ATS is
                                                            set longer than 16 bytes. */
                                     uint8_t bDataLen       /**< [In] 8, 16 or 24 bytes. */
                                     );

/**
* \brief Returns the Unique ID of the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetCardUID(
                               void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                               uint8_t * pUid       /**< [Out] UID of the card. Buffer size should be 7 bytes. */
                               );
/** @} */

/**
* \name Application level Commands
*/
/** @{ */

/**
* \brief Returns the file IDs of all active files within the
* currently selected application
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetFileIDs(
                               void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                               uint8_t * pFid,      /**< [Out] size = 32 bytes. */
                               uint8_t * bNumFid    /**< [Out] Number of fidS read. */
                               );

/**
* \brief Get the ISO File IDs.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetISOFileIDs(
                                  void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                  uint8_t * pFidBuffer, /**< [Out] size = 64 bytes. */
                                  uint8_t * bNumFid     /**< [Out] Number of fidS read. */
                                  );

/**
* \brief Get informtion on the properties of a specific file
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetFileSettings(
                                    void * pDataParams,     /**< [In] Pointer to this layers param structure. */
                                    uint8_t bFileNo,        /**< [In] file number. */
                                    uint8_t * pFSBuffer,    /**< [Out] size = 17 bytes. */
                                    uint8_t * bBufferLen    /**< [Out] size of data put in pFSBuffer. */
                                    );

/**
* \brief Changes the access parameters of an existing file
* \remarks
* bOption can be one of \n
* \li #PHAL_MFDF_COMMUNICATION_ENC OR \n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
*
* bCommSett can be one of \n
* \li #PHAL_MFDF_COMMUNICATION_ENC OR \n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN or \n
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_ChangeFileSettings(
                                       void * pDataParams,      /**< [In] Pointer to this layers param structure. */
                                       uint8_t bOption,         /**< [In] Indicates whether the settings to be sent enciphered or plain. */
                                       uint8_t bFileNo,         /**< [In] file number. */
                                       uint8_t bCommSett,       /**< [In] new communication settings for the file. */
                                       uint8_t * pAccessRights  /**< [In] 2 byte access rights. */
                                       );

/**
* \brief Creates files for storage of plain unformatted user data within
* an existing application on the PICC
*
* \remarks
* If bOption==1, it means pIsoFileId is present and is valid. 
* If bOption=0, it means pIsoFileId is not present. \n
*
* Communication option (bCommSett) Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateStdDataFile(
                                      void * pDataParams,       /**< [In] Pointer to this layers param structure. */
                                      uint8_t bOption,          /**< [In] option parameter. 0x00 means wISOFileId is not provided. 0x01 means wISOFileId is provided and is valid */
                                      uint8_t bFileNo,          /**< [In] file number. */
                                      uint8_t * pISOFileId,     /**< [In] ISO File ID. */
                                      uint8_t bCommSett,        /**< [In] communication settings. */
                                      uint8_t * pAccessRights,  /**< [In] 2 byte access rights. Sent LSB first to PICC. */
                                      uint8_t * pFileSize       /**< [In] 3bytes. Sent LSB first to PICC. */
                                      );

/**
* \brief Creates files for the storage of plain unformatted user data within
* an existing application, additionally supporting the feature of an integrated
* backup mechanism.
*
* \remarks
* If bOption==1, it means pIsoFileId is present and is valid. 
* If bOption=0, it means pIsoFileId is not present. \n
*
* Communication option (bCommSett) Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateBackupDataFile(
                                         void * pDataParams,        /**< [In] Pointer to this layers param structure. */
                                         uint8_t bOption,           /**< [In] option parameter. 0x00 means wISOFileId is not provided. 0x01 means wISOFileId is provided and is valid. */
                                         uint8_t bFileNo,           /**< [In] file number. */
                                         uint8_t * pISOFileId,      /**< [In] ISO File ID. */
                                         uint8_t bCommSett,         /**< [In] communication settings. */
                                         uint8_t * pAccessRights,   /**< [In] 2 byte access rights. Sent LSB first to PICC. */
                                         uint8_t * pFileSize        /**< [In] 3bytes. Sent LSB first to PICC. */
                                         );

/**
* \brief CreateValueFile
*
* Creates files for the storage and manipulation of 32bit
* signed integer values within an existing application on the PICC.
* User provides the entire information in the valInfo buffer.
*
* Communication option (bCommSett) Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* bLimitedCredit values are one or combination of the following: \n
* \li #PHAL_MFDF_ENABLE_LIMITEDCREDIT
* \li #PHAL_MFDF_ENABLE_FREE_GETVALUE
* OR can be set to zero if none of above are desired.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateValueFile(
                                    void * pDataParams,         /**< [In] Pointer to this layers param structure. */
                                    uint8_t bFileNo,            /**< [In] file number. */
                                    uint8_t bCommSett,          /**< [In] communication settings. */
                                    uint8_t * pAccessRights,    /**< [In] 2 byte access rights. Sent LSB first to PICC. */
                                    uint8_t * pLowerLmit,       /**< [In] 4 byte Lower limit value. Sent LSB first to PICC. */
                                    uint8_t * pUpperLmit,       /**< [In] 4 byte Upper limit value. Sent LSB first to PICC. */
                                    uint8_t * pValue,           /**< [In] 4 byte Value. Sent LSB first to PICC. */
                                    uint8_t bLimitedCredit      /**< [In] Limited Credit and free getvalue setting. */
                                    );

/**
* \brief Creates files for multiple storage of structural similar
* data, for example for layalty programs within an existing application.
* Once the file is filled, further writing is not possible unless it is
* cleared.
*
* \remarks
* If bOption==1, it means pIsoFileId is present and is valid. 
* If bOption==0, it means pIsoFileId is not present. \n
*
* Communication option (bCommSett) Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateLinearRecordFile(
                                          void * pDataParams,       /**< [In] Pointer to this layers param structure */
                                          uint8_t bOption,          /**< [In] Indicates ISO file ID is present or not. */
                                          uint8_t  bFileNo,         /**< [In] Linear record file Number. */
                                          uint8_t  *pIsoFileId,     /**< [In] 2 Byte IsoFileId. Sent LSB first to PICC. */
                                          uint8_t bCommSett,        /**< [In] communication settings. */
                                          uint8_t * pAccessRights,  /**< [In] 2 byte access rights. Sent LSB first to PICC. */
                                          uint8_t * pRecordSize,    /**< [In] 3 byte Record Size. Sent LSB first to PICC. */
                                          uint8_t * pMaxNoOfRec     /**< [In] 3 byte Max Number of Records. Sent LSB first to PICC. */
                                          );

/**
* \brief Creates files for multiple storage of structural similar
* data, for example for logging transactions, within an existing application.
* Once the file is filled, the PICC automatically overwrites the oldest record
* with the latest written one.
*
* \remarks
* If bOption==1, it means pIsoFileId is present and is valid. 
* If bOption==0, it means pIsoFileId is not present. \n
*
* Communication option (bCommSett) Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateCyclicRecordFile(
                                           void * pDataParams,      /**< [In] Pointer to this layers param structure */
                                           uint8_t bOption,         /**< [In] Indicates ISO file ID is present or not. */
                                           uint8_t  bFileNo,        /**< [In] Linear record File No. */
                                           uint8_t  *pIsoFileId,    /**< [In] 2 Byte IsoFileId. Sent LSB first to PICC. */
                                           uint8_t bCommSett,       /**< [In] communication settings. */
                                           uint8_t * pAccessRights, /**< [In] 2 byte access rights. Sent LSB first to PICC */
                                           uint8_t * pRecordSize,   /**< [In] 2 byte Record Size. Sent LSB first to PICC */
                                           uint8_t * pMaxNoOfRec    /**< [In] 3 byte Max Number of Records. Sent LSB first to PICC */
                                          );

/**
* \brief Permanently deactivates a file within the file directory of the
* currently selected application.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_DeleteFile(
                               void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                               uint8_t bFileNo      /**< [In] 1 byte file number. */
                               );

/** @} */

/**
* \name Data Manipulation Commands
*/
/** @{ */

/**
* \brief Reads data from standard data files or backup data files
*
* \remarks
*
* Chaining upto the size of the HAL Rx buffer is handled within this function.
* If more data is to be read, the user has to call this function again with
* bOption = PH_EXCHANGE_RXCHAINING | [one of the communication options below]
*
* \c Communication option (bOption) can be one of:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
* \li #PH_EXCHANGE_RXCHAINING | #PHAL_MFDF_COMMUNICATION_ENC 
* (or OR'd with other two options when re-calling the API if PHAL_MFDF_INFO_MOREDATA is
* received)
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING indicating more data to be read.
* \retval Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_ReadData(
                             void * pDataParams,    /**< [In] Pointer to this layers param structure. */
                             uint8_t bOption,       /**< [In] Is either plain or encrypted or MAC'd. */
                             uint8_t bFileNo,       /**< [In] 1 byte file number. */
                             uint8_t * pOffset,     /**< [In] 3 bytes offset. LSB First. */
                             uint8_t * pLength,     /**< [In] 3 bytes. length of data to be read. If 00, entire file will be read. */
                             uint8_t ** ppRxdata,   /**< [Out] Pointer to HAL Rx buffer returned back to user. */
                             uint16_t * pRxdataLen  /**< [Out] Pointer to Length of RxData. */
                             );

/**
* \brief Writes data to standard data files or backup data files
*
* \remarks
* Implements chaining to the card.
*
* \c Communication option (bCommOption) can be one of:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_WriteData(
                              void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                              uint8_t bCommOption,  /**< [In] Communication Mode. Plain, Mac'd or encrypted. */
                              uint8_t bFileNo,      /**< [In] 1 byte file number. */
                              uint8_t * pOffset,    /**< [In] 3 bytes offset. LSB First. */
                              uint8_t * pTxData,    /**< [in] Data to be written. */
                              uint8_t * pTxDataLen  /**< [in] 3 bytes. length of data to be written. */
                              );

/**
* \brief Reads the currently stored value from value files.
*
* Communication option  Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetValue(
        void * pDataParams,     /**< [In] Pointer to this layers param structure. */
        uint8_t bCommOption,    /**< [In] Communication option. */
        uint8_t bFileNo,        /**< [In] 1 byte file number. */
        uint8_t * pValue        /**< [Out] 4 Byte array to store the value read out. LSB First. */
        );

/**
* \brief Increases a value stored in a Value File
*
* Communication option  Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_Credit(
                           void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                           uint8_t bCommOption, /**< [In] Communication option. */
                           uint8_t bFileNo,     /**< [In] 1 byte file number. */
                           uint8_t * pValue     /**< [In] 4 byte value array. LSB first. */
                           );

/**
* \brief Decreases a value stored in a Value File
*
* Communication option  Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_Debit(
                          void * pDataParams,     /**< [In] Pointer to this layers param structure. */
                          uint8_t bCommOption,    /**< [In] communication option. Plain, Mac'd or encrypted. */
                          uint8_t bFileNo,        /**< [In] 1 byte file number. */
                          uint8_t * pValue        /**< [In] 4 byte value array. LSB first. */
                          );

/**
* \brief Allows a limited increase of a value stored in a Value File
* without having full credit permissions to the file.
*
* Communication option  Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_LimitedCredit(
                          void * pDataParams,    /**< [In] Pointer to this layers param structure. */
                          uint8_t bCommOption,   /**< [In] communication option. Plain, Mac'd or encrypted. */
                          uint8_t bFileNo,       /**< [In] 1 byte file number. */
                          uint8_t * pValue       /**< [In] 4 byte value array. LSB first. */
                          );

/**
* \brief Writes data to a record in a Cyclic or Linear Record File.
*
* \remarks
* Implements chaining to the card.
* The data provided on pData will be chained to the card
* by sending data upto the frame size of the DESFire PICC, at a time.
*
* Communication option  Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_WriteRecord(
                                void * pDataParams,     /**< [In] Pointer to this layers param structure. */
                                uint8_t bCommOption,    /**< [In] Communication option. Plain, Mac'd or enc. */
                                uint8_t bFileNo,        /**< [In] 1 byte file number. */
                                uint8_t * pOffset,      /**< [In] 3 bytes offset. LSB First. */
                                uint8_t * pData,        /**< [In] data to be written. */
                                uint8_t * pDataLen      /**< [In] 3 bytes. length of data to be written. */
                                );

/**
* \brief Reads out a set of complete records from a Cyclic or Linear Record File.
*
* \remarks
* The readrecords command reads and stores data in the rxbuffer upto the rxbuffer size before returning
* to the user. The rxbuffer is configured during the HAL init and this is specified by the user.
*
* Chaining upto the size of the HAL Rx buffer is handled within this function.
* If more data is to be read, the user has to call this function again with
* bCommOption = PH_EXCHANGE_RXCHAINING | [one of the communication options below]
*
* \c Communication option (bCommOption) can be one of:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
* \li #PH_EXCHANGE_RXCHAINING | #PHAL_MFDF_COMMUNICATION_ENC 
* (or OR'd with other two options when re-calling the API if PH_ERR_SUCCESS_CHAINING is
* received)
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING indicating more data to be read.
* \retval Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_ReadRecords(
                                void * pDataParams,     /**< [In] Pointer to this layers param structure. */
                                uint8_t bCommOption,    /**< [In] communcation option. */
                                uint8_t bFileNo,        /**< [In] 1 byte file number */
                                uint8_t * pOffset,      /**< [In] 3 bytes offset to the record. LSB First. */
                                uint8_t * pNumRec,      /**< [In] 3 bytes LSB first. Number of records to be read. If 0x00 00 00, then all the records are read. */
                                uint8_t * pRecSize,     /**< [In] Record size. 3Bytes LSB first. */
                                uint8_t ** ppRxdata,    /**< [Out] pointer to the HAL buffer that stores the read data. */
                                uint16_t * pRxdataLen   /**< [Out] number of bytes read (= number of records read * size of record). */
                                );

/**
* \brief Resets a Cyclic or Linear Record File.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_ClearRecordFile(
                                    void * pDataParams, /**< [In] Pointer to this layers param structure. */
                                    uint8_t bFileNo     /**< [In] 1 byte file number. */
                                    );

/**
* \brief Validates all previous write access' on Backup Data files, value
* files and record files within one application.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CommitTransaction(
                                      void * pDataParams    /**< [In] Pointer to this layers param structure. */
                                      );

/**
* \brief Invalidates all previous write access' on Backup Data files, value
* files and record files within one application.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_AbortTransaction(
                                     void * pDataParams /**< [In] Pointer to this layers param structure. */
                                     );
/** @} */

/**
* \name ISO 7816 COMMANDS
*/
/** @{ */

/**
* \brief ISO Select
*
* \remarks
* bSelector = 0x00 => Selection by 2 byte file Id. \n
* bSelector = 0x02 => Select EF under current DF. Fid = EF id \n
* bSelector = 0x04 => Selection by DF Name. DFName and len is then valid \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phalMfdf_IsoSelectFile(
                                  void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                  uint8_t bOption,      /**< [In] If bOption == 00 FCI is returned. If 0x0C no FCI returned. */
                                  uint8_t bSelector,    /**< [In] bSelector equals either 0x00 or 0x02 or 0x04. */
                                  uint8_t * pFid,       /**< [In] two byte file id. Send LSB first. */
                                  uint8_t * pDFname,    /**< [In] DFName upto 16 bytes. valid only when bOption = 0x04. */
                                  uint8_t bDFnameLen,   /**< [In] Length of DFName string provided by the user. */
                                  uint8_t ** ppFCI,     /**< [Out] File control information. */
                                  uint16_t * pwFCILen   /**< [Out] Length of FCI returned. */
                                  );

/**
* \brief ISO Read Binary
* \c wOption can be one of:\n
* \li #PH_EXCHANGE_DEFAULT
* \li #PH_EXCHANGE_RXCHAINING
*
* If status of #PH_ERR_SUCCESS_CHAINING is returned
* Recall this function with wOption PH_EXCHANGE_RXCHAINING to
* get remaining data.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING operation success with chaining.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoReadBinary(
                                  void * pDataParams,       /**< [In] Pointer to this layers param structure. */
                                  uint16_t wOption,         /**< [In] #PH_EXCHANGE_DEFAULT or #PH_EXCHANGE_RXCHAINING. */
                                  uint8_t bOffset,          /**< [In] Offset from where to read. */
                                  uint8_t bSfid,            /**< [In] Short ISO File Id.
                                                                      Bit 7 should be 1 to indicate Sfid is supplied. 
                                                                      Else it is treated as MSB of 2Byte offset. */
                                  uint8_t bBytesToRead,     /**< [In] number of bytes to read. If 0, then entire file to be read. */
                                  uint8_t ** ppRxBuffer,    /**< [Out] buffer where the read bytes will be stored. */
                                  uint16_t * pBytesRead     /**< [Out] number of bytes read. */
                                  );

/**
* \brief Iso Update Binary
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoUpdateBinary(
                                    void * pDataParams, /**< [In] Pointer to this layers param structure. */
                                    uint8_t bOffset,    /**< [In] Offset from where to write. */
                                    uint8_t bSfid,      /**< [In] Short ISO File Id.
                                                        Bit 7 should be 1 to indicate Sfid is supplied
                                                        Else it is treated as MSB of 2Byte offset. */
                                    uint8_t * pData,    /**< [In] data to be written. */
                                    uint8_t bDataLen    /**< [In] number of bytes to write. */
                                    );

/**
* \brief Iso Read Records
*
* \c wOption can be one of:\n
* \li #PH_EXCHANGE_DEFAULT
* \li #PH_EXCHANGE_RXCHAINING
*
* If status of #PH_ERR_SUCCESS_CHAINING is returned
* Recall this function with wOption PH_EXCHANGE_RXCHAINING to
* get remaining data.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoReadRecords(
        void * pDataParams,                     /**< [In] Pointer to this layers param structure. */
        uint16_t wOption,                       /**< [In] #PH_EXCHANGE_DEFAULT or #PH_EXCHANGE_RXCHAINING. */
        uint8_t bRecNo,                         /**< [In] Record to read / from where to read. */
        uint8_t bReadAllFromP1,                 /**< [In] Whether to read all records from P1 or just one. */
        uint8_t bSfid,                          /**< [In] Short ISO File Id bits 0..4 only code this value. */
        uint8_t bBytesToRead,                   /**< [In] number of bytes to read. Multiple of record size. */
        uint8_t ** pRxBuffer,                   /**< [Out] buffer where the read bytes will be stored. */
        uint16_t * pBytesRead                   /**< [Out] number of bytes read. */
        );

/**
* \brief Iso Append record
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoAppendRecord(
                                    void * pDataParams,     /**< [In] Pointer to this layers param structure. */
                                    uint8_t bSfid,          /**< [In] Short Iso File Id bits 0..4 only code this value. Either 0 or sfid. */
                                    uint8_t * pData,        /**< [In] data to write. */
                                    uint8_t bDataLen        /**< [In] number of bytes to write. */
                                    );

/**
* \brief GetChallenge
*
* \remarks
*
* THIS COMMAND IS NOT SUPPORTED IN SAM-X Configuration.
*
* Returns the random number from the PICC. Size depends on the key type
* referred by wKeyNo and wKeyVer
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoGetChallenge(
                                    void * pDataParams, /**< [In] Pointer to this layers param structure. */
                                    uint16_t wKeyNo,    /**< [In] Key number in key store. */
                                    uint16_t wKeyVer,   /**< [In] Key version in key store. */
                                    uint8_t bLe,        /**< [In] Length of expected challenge RPICC1. */
                                    uint8_t * pRPICC1   /**< [Out] RPICC1 returned from PICC. */
                                    );

/**
* \brief Iso External Authenticate
*
* \remarks
*
* THIS COMMAND IS NOT SUPPORTED IN SAM-X Configuration.
*
* pInput should have \n
* \li Reference to crypto algorigthm - 1 Byte 00 => context defined, 02=>2K3DES, 04=>3k3DES, 09=>AES128
* \li Card master key flag - 1 Byte:  0x00 if card master key, 0x01 otherwise.
* \li key number on card - 1 Byte: 0x0 to 0xD
* \li length of random number : 1 Byte
* \li Random number generated by PCD : 8 or 16 bytes. Not required for Sam non X mode.
* \li Random number returned by GetChallenge command : 8 Bytes or 16 Bytes
* \li key number : 2 bytes
* \li key version : 2 bytes
* 
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoExternalAuthenticate(
    void * pDataParams, /**< [In] Pointer to this layers param structure. */
    uint8_t * pInput,   /**< [In] Input data. */
    uint8_t bInputLen,  /**< [In] Length of pInput. */
    uint8_t * pDataOut, /**< [Out] Returns Rnd number PCD2 in sam non x mode. Nothing in S/W mode. */
    uint8_t * pOutLen   /**< [Out] Length of data returned in pDataOut. */
    );


/**
* \brief Iso Internal Authenticate
*
* \remarks
*
* THIS COMMAND IS NOT SUPPORTED IN SAM-X Configuration.
*
* pInput should have \n
* \li Reference to crypto algorigthm - 1 Byte. 02 = 2kDES, 03 = 3k3des, 09=AES, 00 = context defined.
* \li Card master key flag - 1 Byte:  0x00 if card master key, 0x01 otherwise.
* \li key number on card - 1 Byte: 0x0 to 0xD
* \li length of random number : 1 Byte
* \li Random number Rpcd2 : 8 Bytes or 16 Bytes
* \li key number : 2 bytes
* \li key version : 2 bytes
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoInternalAuthenticate(
    void * pDataParams, /**< [In] Pointer to this layers param structure. */
    uint8_t * pInput,   /**< [In] Input data. */
    uint8_t bInputLen,  /**< [In] Length of pInput. */
    uint8_t * pDataOut, /**< [Out] RRPICC2||RPCD2 after decryption in S/W mode. Nothing in Sam non x mode. */
    uint8_t * pOutLen   /**< [Out] Length of data returned in pDataOut. */
    );

/**
* \brief Perform Iso authentication GetChallenge, External Authenticate &
* Internal Authenticate of a DESFire PICC
*
* Internally performs the three pass Iso authentication by calling
* GetChallenge \n
* External Authenticate \n
* Internal Authenticate \n
* Generates and stores the session key \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoAuthenticate(
                                    void * pDataParams,     /**< [In] Pointer to this layers param structure. */
                                    uint16_t wKeyNo,        /**< [In] DESFire key number or SAM Key entry number. */
                                    uint16_t wKeyVer,       /**< [In] Key version. */
                                    uint8_t bKeyNoCard,     /**< [In] Key number on card. 0x0 to 0xD. */
                                    uint8_t bIsPICCkey      /**< [In] Is it PICC Master key? 1=YES. */
                                    );

/** @} */

/**
* \name Miscellaneous functions
*/
/** @{ */

/**
* \brief Perform a GetConfig command.
*
* \c wConfig can be one of:\n
* \li #PHAL_MFDF_ADDITIONAL_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalMfdf_GetConfig(
                              void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wConfig,    /**< [In] Item to read. */
                              uint16_t * pValue    /**< [Out] Read value. */
                              );

/**
* \brief Perform a SetConfig command.
*
* \c wConfig can be one of:\n
* \li #PHAL_MFDF_ADDITIONAL_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalMfdf_SetConfig(
                              void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wConfig,     /**< [In] Item to set. */
                              uint16_t wValue       /**< [In] Value to set. */
                              );
/** @} */

/** @} */
#endif /* NXPBUILD__PHAL_MFDF */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALMFDF_H */
