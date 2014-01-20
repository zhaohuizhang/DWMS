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
* Software specific Crypto-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:04:40 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phCryptoRng.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PH_CRYPTORNG_STUB

#include "phCryptoRng_Stub.h"

phStatus_t phCryptoRng_Stub_Init(
                               phCryptoRng_Stub_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams
                               )
{
    if (sizeof(phCryptoRng_Stub_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTORNG);
    }
	PH_ASSERT_NULL (pDataParams);

    /* Init. private data */
    pDataParams->wId                = PH_COMP_CRYPTORNG | PH_CRYPTORNG_STUB_ID;    

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTORNG);
}

phStatus_t phCryptoRng_Stub_Seed(  
                               phCryptoRng_Stub_DataParams_t * pDataParams,
                               uint8_t * pSeed,
                               uint8_t bSeedLength
                               )
{
	/* satisfy compiler */
	if (pDataParams || pSeed || bSeedLength);

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTORNG);
}

phStatus_t phCryptoRng_Stub_Rnd(  
                              phCryptoRng_Stub_DataParams_t * pDataParams,
                              uint16_t  wNoOfRndBytes, 
                              uint8_t * pRnd           
                              )
{   
	/* satisfy compiler */
	if (pDataParams || wNoOfRndBytes || pRnd);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTORNG);
}

#endif /* NXPBUILD__PHCRYPTORNG_STUB */
