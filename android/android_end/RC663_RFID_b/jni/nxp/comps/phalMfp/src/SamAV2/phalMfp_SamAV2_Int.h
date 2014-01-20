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
* Internal functions of Software implementation of MIFARE (R) application layer.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:50 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHALMFP_SAMAV2_INT_H
#define PHALMFP_SAMAV2_INT_H

#include <ph_Status.h>
#include "phalMfp_SamAV2.h"

/** \addtogroup ph_Private
* @{
*/

/**
* \brief Perform a complete MIFARE Plus Authentication for either Security Level.
*
* Refer to the respective Authentication function for description.
* \see phalMfp_AuthenticateSL0
* \see phalMfp_AuthenticateSL1
* \see phalMfp_AuthenticateSL2
* \see phalMfp_AuthenticateSL3
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_SamAV2_AuthenticateGeneral(
    phalMfp_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bLayer4Comm,                        /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
    uint8_t bFirstAuth,                         /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
    uint16_t wBlockNr,                          /**< [In] Key Block number. */
    uint16_t wKeyNumber,                        /**< [In] Key Storage number. */
    uint16_t wKeyVersion,                       /**< [In] Key Storage version. */ 
    uint8_t bLenDivInput,                   /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
    uint8_t * pDivInput,                     /**< [In] Diversification Input used to diversify the key. */ 
    uint8_t bUseKdfSl2,                         /**< [In] Indicates if a Key derivation for SL2 should be performed. */
    uint8_t bLenPcdCap2,                        /**< [In] Lengh of the supplied PCDCaps. */
    uint8_t * pPcdCap2In,                       /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
    uint8_t * pPcdCap2Out,                      /**< [Out] Pointer to PCDCaps sent from the card (6 bytes). */
    uint8_t * pPdCap2                           /**< [Out] Pointer to PCDCaps sent from the card (6 bytes). */
    );

/**
* \brief Perform a Write command in all it's flavours.
*
* Refer to the respective Write function for description.
* \see phalMfp_Write
* \see phalMfp_Increment
* \see phalMfp_Decrement
* \see phalMfp_IncrementTransfer
* \see phalMfp_DecrementTransfer
* \see phalMfp_Transfer
* \see phalMfp_Restore
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_SamAV2_WriteExt(
                                   phalMfp_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bCmdCode,                            /**< [In] MIFARE Plus Command Code. */    
                                   uint16_t wSrcBnr,                            /**< [In] Source Block number. */
                                   uint16_t wDstBnr,                            /**< [In] Destination Block number. */
                                   uint8_t * pData,                             /**< [In] Payload data. */
                                   uint8_t bDataLength                          /**< [In] Length of payload data. */
                                   );

/** @}
* end of ph_Private group
*/

#endif /* PHALMFP_SAMAV2_INT_H */
