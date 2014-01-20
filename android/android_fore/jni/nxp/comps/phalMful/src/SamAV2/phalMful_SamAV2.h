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
* SamAV2 MIFARE(R) Ultralight Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:04:54 2011 $
*
* History:
*  CHu: Generated 25. November 2009
*
*/

#ifndef PHALMFUL_SAMAV2_H
#define PHALMFUL_SAMAV2_H

#include <ph_Status.h>

phStatus_t phalMful_SamAV2_UlcAuthenticate(
                                       phalMful_SamAV2_DataParams_t * pDataParams,
                                       uint16_t wKeyNumber,
                                       uint16_t wKeyVersion
                                       );

phStatus_t phalMful_SamAV2_Read(
                            phalMful_SamAV2_DataParams_t * pDataParams,
                            uint8_t bAddress,
                            uint8_t * pData
                            );

phStatus_t phalMful_SamAV2_Write(
                             phalMful_SamAV2_DataParams_t * pDataParams,
                             uint8_t bAddress,
                             uint8_t * pData
                             );

phStatus_t phalMful_SamAV2_CompatibilityWrite(
    phalMful_SamAV2_DataParams_t * pDataParams,
    uint8_t bAddress,
    uint8_t * pData
    );

#endif /* PHALMFUL_SAMAV2_H */
