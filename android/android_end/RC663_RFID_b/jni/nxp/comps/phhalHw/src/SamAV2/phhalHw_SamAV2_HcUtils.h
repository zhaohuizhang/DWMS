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
* Generic Tools Component of phHostComm component.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:59 2011 $
*
* History:
*  CTa: Generated 20. Aug 2009
*
*/

#ifndef PHHALHW_SAMAV2_HCUTILS_H
#define PHHALHW_SAMAV2_HCUTILS_H

#include <ph_Status.h>
#include <ph_TypeDefs.h>

/** \defgroup phhalHw_SamAV2_HcUtils Utils
*
* \brief  This is only a wrapper layer to abstract the different Host Communication implementations.
* With this wrapper it is possible to support more than one Host Communication
* in parallel, by adapting this wrapper.
* @{
*/

phStatus_t phhalHw_SamAV2_HcUtils_GetCheckLcLe(
    uint8_t * pCmd,         /**< [In]: address of buffer that contains the command. */
    uint16_t wCmdLength,    /**< [In]: length of the command. */
    uint8_t * isLcPresent,  /**< [Out]: LC byte presence indicator . */
    uint8_t * LcLength,     /**< [Out]: length of the payload of the command . */
    uint8_t * isLePresent   /**< [Out]: LE byte presence indicator . */
    );

/**
* \brief Truncate a 16-Bytes MAC buffer into a 8-Bytes Buffer
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_HcUtils_TruncateMacBuffer(
    uint8_t * pIoBuffer,
    uint8_t * MacLength
    );

/**
* \brief Parse Status Word Sw1Sw2 from the SAM.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_HcUtils_Cmd_ResolveErrorCode(
    uint8_t * pSw1Sw2,      /**< [In] Pointer to the status code sent by the SAM. */
    uint16_t wComponentID   /**< [In] ID of the component to be added. */
    );

/** @}
* end of phhalHw_SamAV2_HcUtils
*/

#endif /* PHHALHW_SAMAV2_HCUTILS_H */
