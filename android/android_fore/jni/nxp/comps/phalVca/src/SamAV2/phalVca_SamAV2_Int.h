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
* Internal functions of SamAV2 implementation of Virtual Card Architecture (R) Application Component.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:03 2011 $
*
* History:
*  CHu: Generated 3. November 2009
*
*/

#ifndef PHALVCA_SAMAV2_INT_H
#define PHALVCA_SAMAV2_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \brief Resolve the LidIndex position.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_SamAV2_Int_ResolveValidIndex(
    phalVca_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wIidIndex,                         /**< [In] The valid LidIndex to search for. */
    uint16_t * pValidIndex                      /**< [Out] The index within the card table with which the LidIndex matches. */
    );

/** @}
* end of ph_Private group
*/

#endif /* PHALVCA_SAMAV2_INT_H */
