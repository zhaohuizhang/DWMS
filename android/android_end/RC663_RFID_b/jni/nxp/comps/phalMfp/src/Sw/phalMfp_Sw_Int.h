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
* $Date: Fri Apr 15 09:27:39 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHALMFP_SW_INT_H
#define PHALMFP_SW_INT_H

#include <ph_Status.h>
#include <phalMfp.h>

/** \addtogroup ph_Private
* @{
*/

#define PHAL_MFP_TAPEOUT_VERSION            30

#define PHAL_MFP_ORIGINALITY_KEY_0          0x8000
#define PHAL_MFP_ORIGINALITY_KEY_1          0x8001
#define PHAL_MFP_ORIGINALITY_KEY_2          0x8002
#define PHAL_MFP_ORIGINALITY_KEY_FIRST      PHAL_MFP_ORIGINALITY_KEY_0
#define PHAL_MFP_ORIGINALITY_KEY_LAST       PHAL_MFP_ORIGINALITY_KEY_2

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
phStatus_t phalMfp_Sw_AuthenticateGeneral(
    phalMfp_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bLayer4Comm,                    /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
    uint8_t bFirstAuth,                     /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
    uint16_t wBlockNr,                      /**< [In] Key Block number. */
    uint16_t wKeyNumber,                    /**< [In] Key Storage number. */
    uint16_t wKeyVersion,                   /**< [In] Key Storage version. */
    uint8_t bLenDivInput,                   /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
    uint8_t * pDivInput,                     /**< [In] Diversification Input used to diversify the key. */ 
    uint8_t bUseKdfSl2,                     /**< [In] Indicates if a Key derivation for SL2 should be performed. */
    uint8_t bLenPcdCap2,                    /**< [In] Lengh of the supplied PCDCaps. */
    uint8_t * pPcdCap2In,                   /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
    uint8_t * pPcdCap2Out,                   /**< [In] Pointer to PCDCaps sent from the card (6 bytes). */
    uint8_t * pPdCap2                       /**< [Out] Pointer to PDCaps sent from the card (6 bytes). */
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
phStatus_t phalMfp_Sw_WriteExt(
                               phalMfp_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bCmdCode,                        /**< [In] MIFARE Plus Command Code. */    
                               uint16_t wSrcBnr,                        /**< [In] Source Block number. */
                               uint16_t wDstBnr,                        /**< [In] Destination Block number. */
                               uint8_t * pData,                         /**< [In] Payload data. */
                               uint16_t wDataLength,                    /**< [In] Length of payload data. */
                               uint8_t bEncrypted                       /**< [In] indicates whether the data should be encrypted or not. */
                               );

/**
* \brief Calculate the Init-Vector for encryption from input data.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfp_Sw_Int_ComputeIv(
                                    uint8_t bIsResponse,    /**< [In] Whether this is a response or command or not. */
                                    uint8_t * pTi,          /**< [In] Transaction Identifier; uint8_t[4]. */
                                    uint16_t wRCtr,         /**< [In] R_CTR (read counter). */
                                    uint16_t wWCtr,         /**< [In] W_CTR (write counter). */
                                    uint8_t * pIv           /**< [Out] Initvector; uint8_t[16]. */
                                    );

/**
* \brief Perform a SL2 Key Derivation Function
*
* This function derives the MIFARE Sector Key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Sw_Int_KdfSl2(
                                 phalMfp_Sw_DataParams_t * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t * pRndA,                           /**< [In] Pointer to RndA Buffer. */
                                 uint8_t * pRndB                            /**< [In] Pointer to RndB Buffer. */
                                 );

/**
* \brief Perform a SL3 Key Derivation Function
*
* This function derives the MIFARE Sector Key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_Sw_Int_KdfSl3(
                                 phalMfp_Sw_DataParams_t * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t * pRndA,                           /**< [In] Pointer to RndA Buffer. */
                                 uint8_t * pRndB                            /**< [In] Pointer to RndB Buffer. */
                                 );

/**
* \brief Perform MIFARE Plus specific MAC truncation.
*/
void phalMfp_Sw_Int_TruncateMac(
                                uint8_t * pMac,             /**< [In] MAC; uint8_t[16]. */
                                uint8_t * pTruncatedMac     /**< [Out] Truncated MAC; uint8_t[8]. */
                                );

/** @}
* end of ph_Private group
*/

#endif /* PHALMFP_SW_INT_H */
