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
* Generic MIFARE(R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:19 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#ifndef PHALMFP_H
#define PHALMFP_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PHAL_MFP

/** \defgroup phalMfp MIFARE Plus (R)
* \brief These Components implement the MIFARE Plus (R) commands.
* @{
*/

/**
* \name Authentication Parameters
*/
/*@{*/
#define PHAL_MFP_KEYA           0x0AU   /**< MIFARE(R) Key A. */
#define PHAL_MFP_KEYB           0x0BU   /**< MIFARE(R) Key B. */
/*@}*/

/** \name Custom Error Codes
*/
/*@{*/
#define PHAL_MFP_ERR_AUTH           (PH_ERR_CUSTOM_BEGIN+0)     /*< MFP Authentication Error. */
#define PHAL_MFP_ERR_CMD_OVERFLOW   (PH_ERR_CUSTOM_BEGIN+1)     /*< MFP Command Overflow Error. */
#define PHAL_MFP_ERR_MAC_PCD        (PH_ERR_CUSTOM_BEGIN+2)     /*< MFP MAC Error. */
#define PHAL_MFP_ERR_BNR            (PH_ERR_CUSTOM_BEGIN+3)     /*< MFP Blocknumber Error. */
#define PHAL_MFP_ERR_EXT            (PH_ERR_CUSTOM_BEGIN+4)     /*< MFP Extension Error. */
#define PHAL_MFP_ERR_CMD_INVALID    (PH_ERR_CUSTOM_BEGIN+5)     /*< MFP Invalid Command Error. */
#define PHAL_MFP_ERR_FORMAT         (PH_ERR_CUSTOM_BEGIN+6)     /*< MFP Authentication Error. */
#define PHAL_MFP_ERR_GEN_FAILURE    (PH_ERR_CUSTOM_BEGIN+7)     /*< MFP Generic Error. */
/*@}*/

/**
* \name Sizes
*/
/*@{*/
#define PHAL_MFP_SIZE_TI            4U  /**< Size of Transaction Identifier. */
#define PHAL_MFP_SIZE_KEYMODIFIER   6U  /**< Size of MIFARE KeyModifier. */
#define PHAL_MFP_SIZE_MAC           16U /**< Size of (untruncated) MAC. */
/*@}*/

/** @} */
#endif /* NXPBUILD__PHAL_MFP */

#ifdef NXPBUILD__PHAL_MFP_SW

/** \defgroup phalMfp_Sw Component : Software
* @{
*/

#define PHAL_MFP_SW_ID          0x01    /**< ID for Software MIFARE Plus layer. */

/**
* \brief MIFARE Plus Software parameter structure
*/
typedef struct
{
    uint16_t wId;                                           /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                            /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;                             /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParamsEnc;                            /**< Pointer to the parameter structure of the Crypto layer for encryption. */
    void * pCryptoDataParamsMac;                            /**< Pointer to the parameter structure of the Crypto layer for macing. */
    void * pCryptoRngDataParams;                            /**< Pointer to the parameter structure of the CryptoRng layer. */
    void * pCryptoDiversifyDataParams;                      /**< Pointer to the parameter structure of the CryptoDiversify layer (can be NULL). */
    uint8_t bKeyModifier[PHAL_MFP_SIZE_KEYMODIFIER];        /**< Key Modifier for MIFARE Plus SL2 authentication. */
    uint16_t wRCtr;                                         /**< R_CTR (read counter); The PICC's read counter is used for a following authentication. */
    uint16_t wWCtr;                                         /**< W_CTR (write counter); The PICC's write counter is used for a following authentication. */
    uint8_t bTi[PHAL_MFP_SIZE_TI];                          /**< Transaction Identifier; unused if 'bFirstAuth' = 1; uint8_t[4]. */
    uint8_t bNumUnprocessedReadMacBytes;                    /**< Amount of data in the pUnprocessedReadMacBuffer. */
    uint8_t pUnprocessedReadMacBuffer[PHAL_MFP_SIZE_MAC];   /**< Buffer containing unprocessed bytes for read mac answer stream. */
    uint8_t pIntermediateMac[PHAL_MFP_SIZE_MAC];            /**< Intermediate MAC for Read Calculation. */
    uint8_t bFirstRead;                                     /**< Indicates whether the next read is a first read in a read (MACed) sequence or not. */
} phalMfp_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfp_Sw_Init(
                           phalMfp_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                           uint16_t wSizeOfDataParams,                  /**< [In] Specifies the size of the data parameter structure. */
                           void * pPalMifareDataParams,             /**< [In] Pointer to a palMifare component context. */
                           void * pKeyStoreDataParams,              /**< [In] Pointer to a KeyStore component context. */
                           void * pCryptoDataParamsEnc,             /**< [In] Pointer to a Crypto component context for encryption. */
                           void * pCryptoDataParamsMac,             /**< [In] Pointer to a Crypto component context for Macing. */
                           void * pCryptoRngDataParams,             /**< [In] Pointer to a CryptoRng component context. */
                           void * pCryptoDiversifyDataParams        /**< [In] Pointer to the parameter structure of the CryptoDiversify layer (can be NULL). */
                           );

/** @} */
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2

/** \defgroup phalMfp_SamAV2 Component : SamAV2
* @{
*/

#define PHAL_MFP_SAMAV2_ID      0x02    /**< ID for SamAV2 MIFARE Plus layer. */

/**
* \brief MIFARE Plus SamAV2 parameter structure
*/
typedef struct
{
    uint16_t wId;                                       /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                        /**< Pointer to the parameter structure of the palMifare component. */
    phhalHw_SamAV2_DataParams_t * pHalSamDataParams;    /**< Pointer to the parameter structure of the hal of the SAM layer. */
    uint8_t bKeyModifier[PHAL_MFP_SIZE_KEYMODIFIER];    /**< Key Modifier for MIFARE Plus SL2 authentication */    
} phalMfp_SamAV2_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfp_SamAV2_Init(
                               phalMfp_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                               uint16_t wSizeOfDataParams,                  /**< [In] Specifies the size of the data parameter structure. */
                               phhalHw_SamAV2_DataParams_t * pHalSamDataParams, /**< [In] Pointer to the parameter structure of the hal of the SAM layer. */                               
                               void * pPalMifareDataParams                  /**< [In] Pointer to the parameter structure of the palMifare component. */
                               );

/** @} */
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X

/** \defgroup phalMfp_SamAV2_X Component : SamAV2_X
* @{
*/

#define PHAL_MFP_SAMAV2_X_ID    0x03    /**< ID for SamAV2_X MIFARE Plus layer. */

/**
* \brief MIFARE Plus SamAV2_X parameter structure
*/
typedef struct
{
    uint16_t wId;                                               /**< Layer ID for this component, NEVER MODIFY! */
    phhalHw_SamAV2_DataParams_t * pHalDataParams;               /**< Pointer to the parameter structure of the HAL layer of the SAM. */
    phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams;   /**< Pointer to the parameter structure of the palMifare component. */
} phalMfp_SamAV2_X_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfp_SamAV2_X_Init(
                                 phalMfp_SamAV2_X_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                                 phhalHw_SamAV2_DataParams_t * pHalDataParams,             /**< [In] Pointer to the HAL parameter structure of the SAM. */
                                 phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams  /**< [In] Pointer to the parameter structure of the underlying palMFIARE layer. */
                                 );

/** @} */
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFP

/** \addtogroup phalMfp
* @{
*/

/**
* \name Security Level 0
*/
/*@{*/

/**
* \brief Performs a MIFARE Plus Write Perso command.
*
* The Write Perso command can be executed using the ISO14443-3 communication protocol (after layer 3 activation)\n
* or using the ISO14443-4 protocol (after layer 4 activation)\n
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_WritePerso(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint8_t bLayer4Comm,  /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol. */
                              uint16_t wBlockNr,    /**< [In] MIFARE Block number. */
                              uint8_t * pValue      /**< [In] Value (16 bytes). */
                              );

/**
* \brief Performs a MIFARE Plus Commit Perso command.
*
* The Commit Perso command can be executed using the ISO14443-3 communication protocol (after layer 3 activation)\n
* or using the ISO14443-4 protocol (after layer 4 activation).\n
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_CommitPerso(
                               void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bLayer4Comm  /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
                               );

/**
* \brief Performs a complete MIFARE Plus Authentication for Security Level 0.
*
* The high-level function performs a 2-step (in future applications also 3-step) authentication.\n
* See the section Evolution on the Main Page regarding future changes in the key parameter.\n\n
*
* The following table shows which parameter is relevant depending on the parameters bLayer4Comm and bFirstAuth.\n
* An "X" encodes that this parameter is relevant. A "-" encodes that this parameter is ignored (if it is an in-parameter) or that it shall be ignored (if it is an out-parameter).\n
*
\verbatim
+-------------+-------+-------+
| bFirstAuth  |   0   |   1   |
+-------------+---+---+---+---+
| bLayer4Comm | 0 | 1 | 0 | 1 |
+-------------+---+---+---+---+
| wBlockNr    | X | X | X | X |
| pKx         | X | X | X | X |
| pRndA       | X | X | X | X |
| bLenPcdCap2 | - | - | - | X |
| pPcdCap2    | - | - | - | X |
| pPdCap2     | - | - | - | X |
+-------------+---+---+---+---+
\endverbatim
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_AuthenticateSL0(
                                   void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bLayer4Comm,     /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
                                   uint8_t bFirstAuth,      /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
                                   uint16_t wBlockNr,       /**< [In] Key Block number. */
                                   uint16_t wKeyNumber,     /**< [In] Key Storage number. */
                                   uint16_t wKeyVersion,    /**< [In] Key Storage version. */      
                                   uint8_t bLenDivInput,    /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
                                   uint8_t * pDivInput,     /**< [In] Diversification Input used to diversify the key. */ 
                                   uint8_t bLenPcdCap2,     /**< [In] Length of the supplied PCDCaps. */
                                   uint8_t * pPcdCap2In,    /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
                                   uint8_t * pPcdCap2Out,   /**< [Out] Pointer to PCD Caps sent from the card (6 bytes). */
                                   uint8_t * pPdCap2        /**< [Out] Pointer to PDCaps sent from the card (6 bytes). */
                                   );

/** @} */

/**
* \name Security Level 1
* \see phalMfc
*/
/*@{*/

/**
* \brief Performs a complete MIFARE Plus Authentication for Security Level 1.
*
* The high-level function performs a 2-step (in future applications also 3-step) authentication.\n
* See the section Evolution on the Main Page regarding future changes in the key parameter.\n\n
*
* The following table shows which parameter is relevant depending on the parameters bLayer4Comm and bFirstAuth.\n
* An "X" encodes that this parameter is relevant. A "-" encodes that this parameter is ignored (if it is an in-parameter) or that it shall be ignored (if it is an out-parameter).\n
*
\verbatim
+-------------+-------+-------+
| bFirstAuth  |   0   |   1   |
+-------------+---+---+---+---+
| bLayer4Comm | 0 | 1 | 0 | 1 |
+-------------+---+---+---+---+
| wBlockNr    | X | X | X | X |
| pKx         | X | X | X | X |
| pRndA       | X | X | X | X |
| bLenPcdCap2 | - | - | - | X |
| pPcdCap2    | - | - | - | X |
| pPdCap2     | - | - | - | X |
+-------------+---+---+---+---+
\endverbatim
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_AuthenticateSL1(
                                   void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bLayer4Comm,     /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
                                   uint8_t bFirstAuth,      /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
                                   uint16_t wBlockNr,       /**< [In] Key Block number. */
                                   uint16_t wKeyNumber,     /**< [In] Key Storage number. */
                                   uint16_t wKeyVersion,    /**< [In] Key Storage version. */  
                                   uint8_t bLenDivInput,    /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
                                   uint8_t * pDivInput,     /**< [In] Diversification Input used to diversify the key. */ 
                                   uint8_t bLenPcdCap2,     /**< [In] Length of the supplied PCDCaps. */
                                   uint8_t * pPcdCap2In,    /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
                                   uint8_t * pPcdCap2Out,   /**< [Out] Pointer to PCD Caps sent from the card (6 bytes). */
                                   uint8_t * pPdCap2        /**< [Out] Pointer to PDCaps sent from the card (6 bytes). */
                                   );

/** @} */

/**
* \name Security Level 2
* \see phalMfc
*/
/*@{*/

/**
* \brief Performs a complete MIFARE Plus Authentication and Key Derivation for Security Level 2.
*
* The high-level function performs a 2-step (in future applications also 3-step) authentication.\n
* The function provides the resulting MIFARE Sector Key Modifier.\n
* If a MIFARE Classic authentication is performed afterwards, use the MIFARE Sector Key Modifier XOR the MIFARE Classic sector key as the key.\n
* See the section Evolution on the Main Page regarding future changes in the key parameter.\n\n
*
* The following table shows which parameter is relevant depending on the parameters bLayer4Comm and bFirstAuth.\n
* An "X" encodes that this parameter is relevant. A "-" encodes that this parameter is ignored (if it is an in-parameter) or that it shall be ignored (if it is an out-parameter).\n
*
\verbatim
+-------------+-------+-------+
| bFirstAuth  |   0   |   1   |
+-------------+---+---+---+---+
| bLayer4Comm | 0 | 1 | 0 | 1 |
+-------------+---+---+---+---+
| wBlockNr    | X | X | X | X |
| pKx         | X | X | X | X |
| pRndA       | X | X | X | X |
| bLenPcdCap2 | - | - | - | X |
| pPcdCap2    | - | - | - | X |
| pPdCap2     | - | - | - | X |
| pKmf        | X | - | - | - |
+-------------+---+---+---+---+
\endverbatim
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_AuthenticateSL2(
                                   void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bLayer4Comm,     /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
                                   uint8_t bFirstAuth,      /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
                                   uint16_t wBlockNr,       /**< [In] Key Block number. */
                                   uint16_t wKeyNumber,     /**< [In] Key Storage number. */
                                   uint16_t wKeyVersion,    /**< [In] Key Storage version. */
                                   uint8_t bLenDivInput,    /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
                                   uint8_t * pDivInput,     /**< [In] Diversification Input used to diversify the key. */ 
                                   uint8_t bLenPcdCap2,     /**< [In] Length of the supplied PCDCaps. */
                                   uint8_t * pPcdCap2In,    /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
                                   uint8_t * pPcdCap2Out,   /**< [Out] Pointer to PCD Caps sent from the card (6 bytes). */
                                   uint8_t * pPdCap2,       /**< [Out] Pointer to PDCaps sent from the card (6 bytes). */
                                   uint8_t * pKmf           /**< [Out] MIFARE Sector Key Modifier (6 bytes). */
                                   );

/**
* \brief Perform MIFARE(R) Authenticate command in Security Level 2 with MIFARE Picc.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_AuthenticateClassicSL2(
    void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bBlockNo,       /**< [In] Blocknumber on Card to authenticate to. */
    uint8_t bKeyType,       /**< [In] Either /ref PHHAL_HW_MFC_KEYA or /ref PHHAL_HW_MFC_KEYB. */
    uint16_t wKeyNumber,    /**< [In] Key number to be used in authentication. */
    uint16_t wKeyVersion,   /**< [In] Key version to be used in authentication. */
    uint8_t * pUid,         /**< [In] UID. */
    uint8_t bUidLength      /**< [In] UID length provided. */
    );

/**
* \brief Performs a Multi Block Read command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_MultiBlockRead(
                                  void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bBlockNr,     /**< [In] MIFARE block number. */
                                  uint8_t bNumBlocks,   /**< [In] Number of blocks to read (must not be more than 3). */
                                  uint8_t * pBlocks     /**< [Out] Block(s) (16*bNumBlocks bytes). */
                                  );

/**
* \brief Performs a Multi Block Write command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_MultiBlockWrite(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bBlockNr,    /**< [In] MIFARE block number. */
                                   uint8_t bNumBlocks,  /**< [In] Number of blocks to write (must not be more than 3). */
                                   uint8_t * pBlocks    /**< [In] Block(s) (16*bNumBlocks bytes). */
                                   );

/** @} */

/**
* \name Security Level 2 and 3
*/
/*@{*/

/**
* \brief Performs a Write / Write MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Write(
                         void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                         uint8_t bEncrypted,    /**< [In] \c 0: Plain communication; \c 1: Encrypted communication; */
                         uint8_t bWriteMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                         uint16_t wBlockNr,     /**< [In] MIFARE block number. */
                         uint8_t bNumBlocks,    /**< [In] Number of blocks to write (must not be more than 3). */
                         uint8_t * pBlocks      /**< [In] Block(s) (16*bNumBlocks bytes).  */
                         );

/**
* \brief Performs a Write / Write MACed command of a value.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_WriteValue(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint8_t bEncrypted,   /**< [In] \c 0: Plain communication; \c 1: Encrypted communication; */
                              uint8_t bWriteMaced,  /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                              uint16_t wBlockNr,    /**< [In] MIFARE block number. */
                              uint8_t * pValue,     /**< [In] pValue[4] containing value (LSB first) read from the MIFARE(R) card */
                              uint8_t bAddrData     /**< [In] bAddrData containing address written to the MIFARE(R) card value block. */
                              );

/**
* \brief Performs a Key Change of a MIFARE Plus key. Same as phalMfp_Write, but diversification input can be provided
* \return Status code 
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_ChangeKey(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bChangeKeyMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                             uint16_t wBlockNr,     /**< [In] MIFARE block number. */
                             uint16_t wKeyNumber,    /**< [In] Key number of key to be written to the card. */
                             uint16_t wKeyVersion,   /**< [In] Key version of key to be written to the card. */
                             uint8_t bLenDivInput,    /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
                             uint8_t * pDivInput      /**< [In] Diversification Input used to diversify the key. */ 
                             );

/** @} */

/**
* \name Security Level 3 - General
*/
/*@{*/

/**
* \brief Performs a complete MIFARE Plus Authentication and Key Derivation for Security Level 3.
*
* The high-level function performs a 2-step (in future applications also 3-step) authentication.\n
* The function computes the resulting session keys for encryption and MACing and stores them in the card state structure.\n
* See the section Evolution on the Main Page regarding future changes in the key parameter.\n\n
*
* The following table shows which parameter is relevant depending on the parameters bLayer4Comm and bFirstAuth.\n
* An "X" encodes that this parameter is relevant. A "-" encodes that this parameter is ignored (if it is an in-parameter) or that it shall be ignored (if it is an out-parameter).\n
*
\verbatim
+-------------+-------+-------+
| bFirstAuth  |   0   |   1   |
+-------------+---+---+---+---+
| wBlockNr    | X | X | X | X |
| pKx         | X | X | X | X |
| pRndA       | X | X | X | X |
| bLenPcdCap2 | - | - | - | X |
| pPcdCap2    | - | - | - | X |
| pPdCap2     | - | - | - | X |
+-------------+---+---+---+---+
\endverbatim
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_AuthenticateSL3(
                                   void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bFirstAuth,      /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
                                   uint16_t wBlockNr,       /**< [In] Key Block number. */
                                   uint16_t wKeyNumber,     /**< [In] Key Storage number. */
                                   uint16_t wKeyVersion,    /**< [In] Key Storage version. */   
                                   uint8_t bLenDivInput,    /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
                                   uint8_t * pDivInput,     /**< [In] Diversification Input used to diversify the key. */ 
                                   uint8_t bLenPcdCap2,     /**< [In] Length of the supplied PCDCaps. */
                                   uint8_t * pPcdCap2In,    /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
                                   uint8_t * pPcdCap2Out,   /**< [Out] Pointer to PCD Caps sent from the card (6 bytes). */
                                   uint8_t * pPdCap2        /**< [Out] Pointer to PDCaps sent from the card (6 bytes). */
                                   );

/**
* \brief Performs a Read / Read MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Read(
                        void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                        uint8_t bEncrypted,     /**< [In] \c 0: Plain communication; \c 1: Encrypted communication; */
                        uint8_t bReadMaced,     /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                        uint8_t bMacOnCmd,      /**< [In] \c 0: No MAC on command; \c 1: MAC on command; */
                        uint16_t wBlockNr,      /**< [In] MIFARE block number. */
                        uint8_t bNumBlocks,     /**< [In] Number of blocks to read. */
                        uint8_t * pBlocks       /**< [Out] Block(s) (16*bNumBlocks bytes).  */
                        );

/**
* \brief Performs a Read / Read MACed Value command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_ReadValue(
                             void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bEncrypted,     /**< [In] \c 0: Plain communication; \c 1: Encrypted communication; */
                             uint8_t bReadMaced,     /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                             uint8_t bMacOnCmd,      /**< [In] \c 0: No MAC on command; \c 1: MAC on command; */
                             uint16_t wBlockNr,      /**< [In] MIFARE block number. */
                             uint8_t * pValue,       /**< [Out] pValue[4] containing value (LSB first) read from the MIFARE(R) card */
                             uint8_t * pAddrData    /**< [Out] bAddrData containing address read from the MIFARE(R) card value block*/
                             );

/**
* \brief Performs a Reset Auth command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_ResetAuth(
                             void * pDataParams     /**< [In] Pointer to this layer's parameter structure. */
                             );

/** @} */

/**
* \name Security Level 3 - Value Operations (MIFARE Plus X only)
*/
/*@{*/

/**
* \brief Performs an Increment / Increment MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Increment(
                             void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bIncrementMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                             uint16_t wBlockNr,         /**< [In] MIFARE Source block number. */
                             uint8_t * pValue           /**< [In] pValue[4] containing value (LSB first) read from the MIFARE(R) card */
                             );

/**
* \brief Performs a Decrement / Decrement MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Decrement(
                             void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bDecrementMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                             uint16_t wBlockNr,         /**< [In] MIFARE Source block number. */
                             uint8_t * pValue           /**< [In] pValue[4] containing value (LSB first) read from the MIFARE(R) card */
                             );

/**
* \brief Performs an Increment Transfer / Increment Transfer MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_IncrementTransfer(
                                     void * pDataParams,                /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bIncrementTransferMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                                     uint16_t wSourceBlockNr,           /**< [In] MIFARE Source block number. */
                                     uint16_t wDestinationBlockNr,      /**< [In] MIFARE Destination block number. */
                                     uint8_t * pValue                   /**< [In] pValue[4] containing value (LSB first) read from the MIFARE(R) card */
                                     );

/**
* \brief Performs a Decrement Transfer / Decrement Transfer MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_DecrementTransfer(
                                     void * pDataParams,                /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bDecrementTransferMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                                     uint16_t wSourceBlockNr,           /**< [In] Source block number. */
                                     uint16_t wDestinationBlockNr,      /**< [In] Destination block number. */
                                     uint8_t * pValue                   /**< [In] pValue[4] containing value (LSB first) read from the MIFARE(R) card */
                                     );

/**
* \brief Performs a Transfer / Transfer MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Transfer(
                            void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
                            uint8_t bTransferMaced,     /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                            uint16_t wBlockNr           /**< [In] MIFARE Destination block number. */
                            );

/**
* \brief Performs a Restore / Restore MACed command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Restore(
                           void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                           uint8_t bRestoreMaced,   /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                           uint16_t wBlockNr        /**< [In] MIFARE Source block number. */
                           );

/** @} */

/**
* \name Proximity Check (MIFARE Plus X only)
*/
/*@{*/

/**
* \brief Performs the whole Proximity Check command chain.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_ProximityCheck(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bGenerateRndC,    /**< [In] \c 0: RndC provided; \c 1: generate RndC; */
                                  uint8_t * pRndC,          /**< [In] Provided RndC (7 bytes), ignored if bGenerateRndC == 1. */
                                  uint8_t bPps1,            /**< [In] Communication Speed byte (PPS1). */
                                  uint8_t bNumSteps,        /**< [In] Number of ProximityCheck cycles; RFU, must be 1. */
                                  uint8_t * pUsedRndC       /**< [Out] Used RndC (7 bytes), can be NULL. */
                                  );

/** @} */

/**
* \name Miscellaneous Functions
*/
/*@{*/

/**
* \brief Reset the libraries internal secure messaging state.
*
* This function must be called before interacting with the PICC to set the libraries internal card-state back to default.\n
* E.g. when an error occurred or after a reset of the field.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_ResetSecMsgState(
                                    void * pDataParams  /**< [In] Pointer to this layer's parameter structure. */
                                    );

/** @} */

/** @} */
#endif /* NXPBUILD__PHAL_MFP */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHMFPLIB_H */
