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
* Internal functions of Software implementation of ICode SLI (R) application layer.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:40 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHALSLI_SW_INT_H
#define PHALSLI_SW_INT_H

#include <ph_Status.h>

/**
* \brief Sets or clears the option bit in the \ref phpalSli15693 layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalSli_Sw_Int_SetOptionBit(
                                       phalSli_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bOption                          /**< [In] Option flag; \c #PH_OFF: disable; \c #PH_ON: enable. */
                                       );

#endif /* PHALSLI_SW_INT_H */
