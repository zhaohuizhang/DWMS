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
* $Date: Fri Apr 15 09:26:33 2011 $
*
* History:
*  CHu: Generated 27. July 2009
*
*/

#ifndef PHHALHW_SAMAV2_HC_AV1_H
#define PHHALHW_SAMAV2_HC_AV1_H

#include <ph_Status.h>
#include <phhalHw.h>

/** \defgroup phhalHw_SamAV2_Hc_AV1 SamAV1
* \brief Provides a Secure Messaging interface for HostComm AV1.
* @{
*/

phStatus_t phhalHw_SamAV2_Hc_AV1_AuthenticateHost(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bAuthType,                          /**< [In] The type of Authentication to perform. */
    uint16_t wRdKeyRefNum,                      /**< [In] The key reference number for readerLib KeyStorage. */
    uint16_t wRdKeyRefVer,                      /**< [In] The key reference version for readerLib KeyStorage. */
    uint8_t bSamKeyRefNum,                      /**< [In] The key reference number for SAM KeyStorage. */
    uint8_t bSamKeyRefVer,                      /**< [In] The key reference version for SAM KeyStorage. */
    uint8_t * pDivInput,                        /**< [In] Diversification input data. */
    uint8_t bLenDivInput                        /**< [In] Diversification input data length.*/
    );

/**
* \brief Perform Encryption using SamAV1 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV1_Int_Encrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data to encrypt. */
    uint16_t wLengthIn,                         /**< [In] Length of data to encrypt. */
    uint16_t wBufferSize,                       /**< [In] Size of the buffer. */
    uint16_t * pLengthOut                       /**< [Out] Number of encrypted data bytes. */                           
    );

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_Decrypt(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data to decrypt. */
    uint16_t wLengthIn,                         /**< [In] Length of data to decrypt. */
    uint16_t * pLengthOut                       /**< [Out] Number of decrypted data bytes. */   
    );

/**
* \brief Append MAC to a data stream using SamAV1 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV1_Int_CalculateMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data for mac calculation. */
    uint16_t wLength                            /**< [In] Length of data. */
    );

/**
* \brief Remove Mac and verify it using SamAV1 Host Protocol
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV1_Int_VerifyRemoveMac(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pBuffer,                          /**< [In] Data to unmac. */
    uint16_t wLengthIn,                         /**< [In] Length of data to unmac. */
    uint16_t * pLengthOut                       /**< [Out] Number of unmaced data bytes. */
    );

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_GetCmdProtection(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCmd,
    uint8_t bP1,
    uint8_t bP2,
    uint8_t bIsResponse,
    uint8_t * bEncrypted,
    uint8_t * bMaced
    );

/** @}
* end of phhalHw_SamAV2_Hc_AV1 group
*/

#endif /* PHHALHW_SAMAV2_HC_AV1_H */
