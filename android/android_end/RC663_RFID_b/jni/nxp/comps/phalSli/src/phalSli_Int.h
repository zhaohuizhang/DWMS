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
* Internal functions and defines of ISO15693 application layer.
* $Author: santosh.d.araballi $
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:28:03 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHALSLI_INT_H
#define PHALSLI_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \name SLI Command Codes
*/
/*@{*/
#define PHAL_SLI_CMD_SET_EAS                0xA2    /**< SLI-S/SLI-L This command enables the EAS mode if the EAS mode is not locked. */
#define PHAL_SLI_CMD_RESET_EAS              0xA3    /**< SLI-S/SLI-L This command disables the EAS mode if the EAS mode is not locked. */
#define PHAL_SLI_CMD_LOCK_EAS               0xA4    /**< SLI-S/SLI-L This command locks the current state of the EAS mode and the EAS ID. */
#define PHAL_SLI_CMD_EAS_ALARM              0xA5    /**< SLI-S/SLI-L EAS Alarm command. */
#define PHAL_SLI_CMD_PASSWORD_PROTECT_EAS   0xA6    /**< SLI-S/SLI-L This command enables the password protection for EAS. */
#define PHAL_SLI_CMD_WRITE_EAS_ID           0xA7    /**< SLI-S/SLI-L Write a new AES Identifier in the corresponding configuration memory. */
#define PHAL_SLI_CMD_READ_EPC               0xA8    /**< SLI-S Return the EPC data. */

#define PHAL_SLI_CMD_GET_RANDOM_NUMBER      0xB2    /**< SLI-S/SLI-L Receive a random number from the label IC. */
#define PHAL_SLI_CMD_SET_PASSWORD           0xB3    /**< SLI-S/SLI-L Set password to get access to different protected functionalities. */
#define PHAL_SLI_CMD_WRITE_PASSWORD         0xB4    /**< SLI-S/SLI-L Write a new password into the related memory. */
#define PHAL_SLI_CMD_LOCK_PASSWORD          0xB5    /**< SLI-S/SLI-L This command will lock the addressed password to prevent futur change. */
#define PHAL_SLI_CMD_PROTECT_PAGE           0xB6    /**< SLI-S Define the restriction to get access to the related page. */
#define PHAL_SLI_CMD_LOCK_PAGE_PROTECT_COND 0xB7    /**< SLI-S Lock the page protection condition. */
#define PHAL_SLI_CMD_GET_MULT_BLOCK_PROT_ST 0xB8    /**< SLI-S Return the block protection status of the requested blocks. */
#define PHAL_SLI_CMD_DESTROY_SLI            0xB9    /**< SLI-S/SLI-L This irreversible command destroys the label IC. */
#define PHAL_SLI_CMD_ENABLE_PRIVACY         0xBA    /**< SLI-S/SLI-L Enter into the privacy mode. */
#define PHAL_SLI_CMD_64BIT_PASSWORD_PROTECT 0xBB    /**< SLI-S Lead to require both Rd and Wr passwords to get access to password protected blocks. */
/*@}*/

/** @}
* end of ph_Private
*/

#endif /* PHALSLI_INT_H */
