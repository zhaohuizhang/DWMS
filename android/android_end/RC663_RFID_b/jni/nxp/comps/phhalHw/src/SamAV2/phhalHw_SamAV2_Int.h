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
* Internal definitions for SAMAV2 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.5 $
* $Date: Fri Apr 15 09:27:58 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_SAMAV2_INT_H
#define PHHALHW_SAMAV2_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
*  @{
*/

#define PHHAL_HW_SAMAV2_TIMER_FREQ                      13.56f      /**< RC internal timer frequency */
#define PHHAL_HW_SAMAV2_ETU_106                         9.44f       /**< Duration of one ETU at 106 kBit/s in [us] */

#define PHHAL_HW_SAMAV2_RC523_TIMER_MAX_VALUE_US        6596996     /**< (RC523) Maximum timer value in microseconds. */
#define PHHAL_HW_SAMAV2_RC523_TIMER_SHIFT               4.8f        /**< (RC523) Shift of the internal RC timer in ETUs. */

#define PHHAL_HW_SAMAV2_RC663_TIMER_RELOAD_MAX          0xFFFFU     /**< (RC663) Maximum reload value of internal timers. */
#define PHHAL_HW_SAMAV2_RC663_I14443_ADD_DELAY_US       15          /**< (RC663) Additional digital timeout delay for ISO14443. */
#define PHHAL_HW_SAMAV2_RC663_DIGI_DELAY_TXWAIT_DBFREQ  8U          /**< (RC663) Chip digital delay in ETUs for TxWait. */

/** \name RXTX settings for Rc663
*/
/*@{*/
#define PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_106  0x00U   /**< ISO14443A Operating mode at 106kbit/s. */
#define PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_212  0x01U   /**< ISO14443A Operating mode at 212kbit/s. */
#define PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_424  0x02U   /**< ISO14443A Operating mode at 414kbit/s. */
#define PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_848  0x03U   /**< ISO14443A Operating mode at 848kbit/s. */
/*@}*/

#define PHHAL_HW_SAMAV2_AUTHMODE_KEYA 0x60  /**< MFC KeyA Authentication Mode. */
#define PHHAL_HW_SAMAV2_AUTHMODE_KEYB 0x61  /**< MFC KeyA Authentication Mode. */

/**
* \brief Update LC byte according to data already available in Buffer
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_UpdateLc( 
                                   phhalHw_SamAV2_DataParams_t * pDataParams    /**< [In] Pointer to this layer's parameter structure. */
                                   );

/**
* \brief Update P1byte according to data already available in Buffer
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_UpdateP2( 
                                   phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t p2                                   /**< [In] Value for P1. */
                                   );
/**
* \brief Apply card mode according to given parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc663_SetCardMode(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wTxDataRate,                       /**< [In] TxDataRate HAL config. */
    uint16_t wRxDataRate                        /**< [In] RxDataRate HAL config. */
    );

/**
* \brief Set configuration parameter.
*
* <b> Rc523 Specific </b>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_PARAMETER_OVERFLOW Setting the parameter value would lead to an overflow.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_SamAV2_Rc523_SetConfig(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,                           /**< [In] Configuration Identifier. */
    uint16_t wValue                             /**< [In] Configuration Value. */
    );

/**
* \brief Blocks until the time given by \b wTimeout elapsed.
*
* <b> Rc523 Specific </b>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b bUnit is invalid.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_SamAV2_Rc523_Wait(
                                     phhalHw_SamAV2_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bUnit,                             /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
                                     uint16_t wTimeout                          /**< [In] Timeout value. */
                                     );

/**
* \brief Sets the Frame Delay Time (Timeout).
*
* <b> Rc523 Specific </b>
* \b Note: Frame Delay Time is defined between the last transmitted bit and the first received bit.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc523_SetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bUnit,                               /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
                                       uint16_t wTimeout                            /**< [In] Timeout value. */
                                       );

/**
* \brief Retrieves the Frame Delay Time of the last command.
*
* <b> Rc523 Specific </b>
* \b Note: Frame Delay Time is defined between the last transmitted bit and the first received bit.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc523_GetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                       uint32_t * pTime                             /**< [Out] Calculated time in microseconds from timer contents. */
                                       );

/**
* \brief Set configuration parameter.
*
* <b> Rc663 Specific </b>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_PARAMETER_OVERFLOW Setting the parameter value would lead to an overflow.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_SamAV2_Rc663_SetConfig(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,                           /**< [In] Configuration Identifier. */
    uint16_t wValue                             /**< [In] Configuration Value. */
    );

/**
* \brief Blocks until the time given by \b wTimeout elapsed.
*
* <b> Rc663 Specific </b>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b bUnit is invalid.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_SamAV2_Rc663_Wait(
                                     phhalHw_SamAV2_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bUnit,                             /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
                                     uint16_t wTimeout                          /**< [In] Timeout value. */
                                     );

/**
* \brief Sets the RxWait time.
*
* <b> Rc663 Specific </b>
* \b Note: RxWait is defined between the last transmitted bit and the activation of the receiver.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc663_SetRxWait(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wTimeEtu                           /**< [In] RxWait time in ETUs. */
    );

/**
* \brief Sets the TxWait time in microseconds.
*
* <b> Rc663 Specific </b>
* \b Note: TxWait is defined between the last received bit and the next transmitted bit.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc663_SetTxWait(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wTimeUs                            /**< [In] TxWait time in microseconds. */
    );

/**
* \brief Sets the Frame Delay Time (Timeout).
*
* <b> Rc663 Specific </b>
* \b Note: Frame Delay Time is defined between the last transmitted bit and the first received bit.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc663_SetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bUnit,                               /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
                                       uint16_t wTimeout                            /**< [In] Timeout value. */
                                       );

/**
* \brief Retrieves the Frame Delay Time of the last command.
*
* <b> Rc663 Specific </b>
* \b Note: Frame Delay Time is defined between the last transmitted bit and the first received bit.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Rc663_GetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                       phStatus_t wExchangeStatus,                  /**< [In] Status code returned by exchange function. */
                                       uint32_t * pTime                             /**< [Out] Calculated time in microseconds from timer contents. */
                                       );

/**
* \brief Returns the delay of the digital circutry for the current protocol.
*
* <b> Rc663 Specific </b>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_Rc663_GetDigiDelay(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bIsTimeout,                         /**< [In] Wheter the returned delay is added to a timeout (#PH_ON) or is used for measurement (#PH_OFF). */
    uint16_t * pDelayUs                         /**< [Out] The digital delay in microseconds. */
    );

/** @}
* end of ph_Private group
*/

#endif /* PHHALHW_SAMAV2_INT_H */
