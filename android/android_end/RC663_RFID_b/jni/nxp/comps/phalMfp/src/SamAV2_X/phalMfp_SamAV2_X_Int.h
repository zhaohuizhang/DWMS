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
* $Date: Fri Apr 15 09:27:01 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHALMFP_SAMAV2_X_INT_H
#define PHALMFP_SAMAV2_X_INT_H

#include <ph_Status.h>
#include <phalMfp.h>

/** \addtogroup ph_Private
* @{
*/

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
phStatus_t phalMfp_SamAV2_X_WriteExt(
                                     phalMfp_SamAV2_X_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bCmdCode,                              /**< [In] MIFARE Plus Command Code. */    
                                     uint16_t wSrcBnr,                              /**< [In] Source Block number. */
                                     uint16_t wDstBnr,                              /**< [In] Destination Block number. */
                                     uint8_t * pData,                               /**< [In] Payload data. */
                                     uint8_t bDataLength                            /**< [In] Length of payload data. */
                                     );

/**
* \brief Convert HAL Nak codes into palMifare NAK codes.
* \return Status code
* \retval #PHPAL_MIFARE_ERR_NAK0 NAK 0.
* \retval #PHPAL_MIFARE_ERR_NAK1 NAK 1.
* \retval #PHPAL_MIFARE_ERR_NAK4 NAK 4.
* \retval #PHPAL_MIFARE_ERR_NAK5 NAK 5.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfp_SamAV2_X_ConvertNak(
                                       phStatus_t status    /**< [In] HAL NAK Code. */
                                       );

/** @}
* end of ph_Private group
*/

#endif /* PHALMFP_SAMAV2_X_INT_H */
