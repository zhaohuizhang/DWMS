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
* Software specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:04:40 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHCRYPTORNG_STUB_H
#define PHCRYPTORNG_STUB_H

#include <ph_Status.h>
#include <phCryptoRng.h>

phStatus_t phCryptoRng_Stub_Seed(
                               phCryptoRng_Stub_DataParams_t * pDataParams,
                               uint8_t * bSeed,
                               uint8_t bSeedLength
                               );

phStatus_t phCryptoRng_Stub_Rnd(   
                            phCryptoRng_Stub_DataParams_t * pDataParams,
                            uint16_t  wNoOfRndBytes,
                            uint8_t * pRnd
                            );

#endif /* PHCRYPTORNG_STUB_H */
