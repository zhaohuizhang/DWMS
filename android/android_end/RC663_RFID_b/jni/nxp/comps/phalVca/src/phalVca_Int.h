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
* Internal functions of Virtual Card Architecture (R) Application Component.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:24 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#ifndef PHALVCA_INT_H
#define PHALVCA_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \name Virtual Card Architecture Commands
*/
/*@{*/
#define PHAL_VCA_CMD_PPC    0xF0    /**< VCA Prepare Proximity Check command code. */
#define PHAL_VCA_CMD_PC     0xF2    /**< VCA Proximity Check command code. */
#define PHAL_VCA_CMD_VPC    0xFD    /**< VCA Verify Proximity Check command code. */
#define PHAL_VCA_CMD_VCS    0x42    /**< VCA Virtual Card Support command code. */
#define PHAL_VCA_CMD_VCSL   0x4B    /**< VCA Virtual Card Support Last command code. */
#define PHAL_VCA_CMD_SVC    0x40    /**< VCA Select Virtual Card command code. */
#define PHAL_VCA_CMD_DVC    0x48    /**< VCA DeselectVC command code. */
/*@}*/

/**
* \name Virtual Card Architecture Response Codes
*/
/*@{*/
#define PHAL_VCA_RESP_ACK_ISO4          0x90U   /*< VCA ACK (in ISO14443-4 mode). */
#define PHAL_VCA_RESP_ERR_CMD_INVALID   0x0BU   /*< VCA Invalid Command Error. */
#define PHAL_VCA_RESP_ERR_FORMAT        0x0CU   /*< VCA Format Error. */
#define PHAL_VCA_RESP_ERR_GEN           0x0FU   /*< VCA Generic Error. */
/*@}*/

#define PHAL_VCA_IID_SIZE       16  /**< Size of the Installation Identifier. */
#define PHAL_VCA_PC_RND_LEN     7   /**< Size of the Proximity Check Random numbers. */

/**
* \brief Evaluate if the received response is erroneus.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_ComputeErrorResponse(
    uint16_t wNumBytesReceived,     /**< [In] Number of bytes received from the card. */
    uint8_t bStatus                 /**< [In] Status byte received from the card. */
    );

/**
* \brief Perform a Prepare Proximity Check command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_PrepareProximityCheck(
    void * pPalMifareDataParams     /**< [In] Pointer to a palMifare component context. */
    );

/**
* \brief Perform a Proximity Check command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_ProximityCheck(
                                      void * pPalMifareDataParams,  /**< [In] Pointer to a palMifare component context. */
                                      uint8_t bNumSteps,            /**< [In] Number of steps for the PC (1..7). */
                                      uint8_t * pRndC,              /**< [In] RndC[7]. */
                                      uint8_t * pRndRC              /**< [Out] RndRC[14]. */
                                      );

/** @}
* end of ph_Private group
*/

#endif /* PHALVCA_INT_H */
