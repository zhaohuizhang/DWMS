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
* $Revision: 1.5 $
* $Date: Fri Apr 15 09:26:42 2011 $
*
* History:
*  CHu: Generated 27. July 2009
*
*/

#ifndef PHHALHW_SAMAV2_HC_AV2_H
#define PHHALHW_SAMAV2_HC_AV2_H

#include <ph_Status.h>

/** \defgroup phhalHw_SamAV2_Hc_AV2 SamAV2
* \brief Provides a Secure Messaging interface for HostComm AV2.
* @{
*/

#define PHHALHW_SAMAV2_HC_MODE_PLAIN        0x00    /* Plain mode. */
#define PHHALHW_SAMAV2_HC_MODE_MAC          0x01    /* MAC mode. */
#define PHHALHW_SAMAV2_HC_MODE_FULL         0x02    /* Full protection mode. */
#define PHHALHW_SAMAV2_HC_MODE_NOT_DEFINED  0xFF    /* Not initialized. */


/**
* \brief Perform Host Authentication Commands and responses using SamAV2 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV2_AuthenticateHost(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t  bAuthType,                         /**< [In] The type of Authentication to perform. */
    uint16_t wRdKeyRefNum,                      /**< [In] The key reference number for readerLib KeyStorage. */
    uint16_t wRdKeyRefVer,                      /**< [In] The key reference version for readerLib KeyStorage. */
    uint8_t  bSamKeyRefNum,                     /**< [In] The key reference number for SAM KeyStorage. */
    uint8_t  bSamKeyRefVer                      /**< [In] The key reference version for SAM KeyStorage. */
    );

/**
* \brief Perform Encryption using SamAV2 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV2_Int_Encrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data to encrypt. */
    uint16_t wTxLength,                         /**< [In] Length of data to encrypt. */
    uint16_t wBufferSize,                       /**< [In] Size of the buffer. */
    uint16_t * pTxLength,                       /**< [Out] Number of encrypted data bytes. */
    uint8_t bFirst,                             /**< [In] Whether this is the first block. */
    uint8_t bLast                               /**< [In] Whether this is the last block. */
    );

/**
* \brief Perform Decryption using SamAV2 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV2_Int_Decrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data to decrypt. */
    uint16_t wRxLength,                         /**< [In] Length of data to decrypt. */
    uint16_t * pRxLength,                       /**< [Out] Number of decrypted data bytes. */
    uint8_t bFirst,                             /**< [In] Whether this is the first block. */
    uint8_t bLast                               /**< [In] Whether this is the last block. */
    );

/**
* \brief Append MAC to a data stream using SamAV2 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV2_Int_AppendMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data to mac. */
    uint16_t wTxLength,                         /**< [In] Length of data to mac. */
    uint16_t wBufferSize,                       /**< [In] Size of the buffer. */
    uint16_t * pTxLength,                       /**< [Out] Number of data bytes incl. MAC. */
    uint8_t bFirst,                             /**< [In] Whether this is the first block. */
    uint8_t bLast                               /**< [In] Whether this is the last block. */
    );

/**
* \brief Remove Mac and verify it using SamAV2 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV2_Int_VerifyRemoveMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] data to unmac. */
    uint16_t wRxLength,                         /**< [In] length of data to unmac. */
    uint16_t * pRxLength,                       /**< [Out] number of unmaced data bytes. */
    uint8_t bFirst,                             /**< [In] Whether this is the first block. */
    uint8_t bLast                               /**< [In] Whether this is the last block. */
    );


phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GetFirstLastCommand(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bCmd,                               /**< [In] Command code. */
    uint8_t bP1,                                /**< [In] P1 of command. */
    uint8_t bP2,                                /**< [In] P2 of command. */
    uint8_t * pFirstCmd,                        /**< [Out] Whether this is the first block. */
    uint8_t * pLastCmd                          /**< [Out] Whether this is the last block. */
    );

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GetFirstLastResponse(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bSw1,                               /**< [In] SW1 of response. */
    uint8_t bSw2,                               /**< [In] SW2 of response. */
    uint8_t * pFirstResponse,                   /**< [Out] Whether this is the first block. */
    uint8_t * pLastResponse                     /**< [Out] Whether this is the last block. */
    );

/** @}
* end of phhalHw_SamAV2_Hc_AV2 group
*/

#endif /* PHHALHW_SAMAV2_HC_AV2_H */
