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
* Configuration for SamAV2 (Rc663-specific) HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:09 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_SAMAV2_RC663_CONFIG_H
#define PHHALHW_SAMAV2_RC663_CONFIG_H

#include <ph_Status.h>
#include <phhalHw.h>

/** \defgroup phhalHw_SamAV2_Rc663_Config Rc663 User Configuration
* \brief Custom Configuration of hardware related settings.
* @{
*/

/** \name RxThreshold values
*/
/*@{*/
#define PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_106   0x32U   /**< RxThreshold for ISO14443A @ 106kbit/s. */
#define PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_212   0x32U   /**< RxThreshold for ISO14443A @ 212kbit/s. */
#define PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_424   0x32U   /**< RxThreshold for ISO14443A @ 424kbit/s. */
#define PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_848   0x32U   /**< RxThreshold for ISO14443A @ 848kbit/s. */
/*@}*/

/** @}
* end of phhalHw_Rc663_Config group
*/

#endif /* PHHALHW_SAMAV2_RC663_CONFIG_H */
