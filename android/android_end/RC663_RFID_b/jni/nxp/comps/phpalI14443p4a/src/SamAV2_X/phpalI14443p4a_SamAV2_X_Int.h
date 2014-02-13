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
* Internal definitions for SamAV2 ISO14443-4A Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:48 2011 $
*
* History:
*  CHu: Generated 03. December 2009
*
*/

#ifndef PHPALI14443P4A_SAMAV2_X_INT_H
#define PHPALI14443P4A_SAMAV2_X_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/** PPS Command code. */
#define PHPAL_I14443P4A_SAMAV2_X_PPSS   0xD0U

/** PPS0 code. */
#define PHPAL_I14443P4A_SAMAV2_X_PPS0   0x11U

/** Position of LEN within ATS. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_LEN    0

/** Position of T0 within ATS. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_T0     1

/** Bit which signals TA(1) presence. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_TA1_PRESENT    0x10U

/** Bit which signals TB(1) presence. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_TB1_PRESENT    0x20U

/** Bit which signals TC(1) presence. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_TC1_PRESENT    0x40U

/** Bit within TC(1) which signals NAD support. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_TC1_NAD_SUPPORT    0x01U

/** Bit within TC(1) which signals CID support. */
#define PHPAL_I14443P4A_SAMAV2_X_ATS_TC1_CID_SUPPORT    0x02U

/** FWI default value. */
#define PHPAL_I14443P4A_SAMAV2_X_FWI_DEFAULT    0x04U

/** FSCI default value. */
#define PHPAL_I14443P4A_SAMAV2_X_FSCI_DEFAULT   0x02U

/** @}
* end of ph_Private
*/

#endif /* PHPALI14443P4A_SAMAV2_X_INT_H */
