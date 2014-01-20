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
* $Date: Fri Apr 15 09:27:05 2011 $
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

#ifdef NXPBUILD__PH_CRYPTOSYM_STUB

#include "phCryptoSym_Stub.h"

phStatus_t phCryptoSym_Stub_Init(
                               phCryptoSym_Stub_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams
                               )
{
    phStatus_t statusTmp;
    if (sizeof(phCryptoSym_Stub_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
    }
	PH_ASSERT_NULL (pDataParams);

    /* Init. private data */
    pDataParams->wId = PH_COMP_CRYPTOSYM | PH_CRYPTOSYM_STUB_ID;

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Stub_InvalidateKey(pDataParams))

        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_InvalidateKey(
                                        phCryptoSym_Stub_DataParams_t * pDataParams
                                        )
{
	/* satisfy compiler */
	if (pDataParams);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_Encrypt(
                                  phCryptoSym_Stub_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  const uint8_t * pPlainBuffer,
                                  uint16_t  wBufferLength,
                                  uint8_t * pEncryptedBuffer
                                  )
{ 
	/* satisfy compiler */
	if (pDataParams || wOption || pPlainBuffer || wBufferLength || pEncryptedBuffer);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);   
}

phStatus_t phCryptoSym_Stub_Decrypt(
                                  phCryptoSym_Stub_DataParams_t * pDataParams,
                                  uint16_t wOption, 
                                  const uint8_t * pEncryptedBuffer,
                                  uint16_t  wBufferLength,
                                  uint8_t * pPlainBuffer
                                  )
{
	/* satisfy compiler */
	if (pDataParams || wOption || pEncryptedBuffer || wBufferLength || pPlainBuffer);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);   
}


phStatus_t phCryptoSym_Stub_CalculateMac(
                                       phCryptoSym_Stub_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       const uint8_t * pData,
                                       uint16_t  wDataLength,
                                       uint8_t * pMac,
                                       uint8_t * pMacLength
                                       )
{
	/* satisfy compiler */
	if (pDataParams || wOption || pData || wDataLength || pMac || pMacLength);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);   
}

phStatus_t phCryptoSym_Stub_LoadIv(  
                                 phCryptoSym_Stub_DataParams_t * pDataParams,
                                 const uint8_t * pIV,   
                                 uint8_t bIVLength
                                 )
{
	/* satisfy compiler */
	if (pDataParams || pIV || bIVLength);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_LoadKey(  
                                  phCryptoSym_Stub_DataParams_t * pDataParams,
                                  uint16_t wKeyNo,
                                  uint16_t wKeyVersion,
                                  uint16_t wKeyType
                                  )
{
	/* satisfy compiler */
	if (pDataParams || wKeyNo || wKeyVersion || wKeyType);

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}


phStatus_t phCryptoSym_Stub_LoadKeyDirect(  
                                        phCryptoSym_Stub_DataParams_t * pDataParams,
                                        const uint8_t * pKey,
                                        uint16_t wKeyType
                                        )
{
	/* satisfy compiler */
	if (pDataParams || pKey || wKeyType);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_DiversifyKey(  
                                       phCryptoSym_Stub_DataParams_t * pDataParams, 
                                       uint16_t wOption,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVersion,
                                       uint8_t * pDivInput,
                                       uint8_t  bLenDivInput, 
                                       uint8_t * pDiversifiedKey
                                       )
{
	/* satisfy compiler */
	if (pDataParams || wOption || wKeyNo || wKeyVersion || pDivInput || bLenDivInput || pDiversifiedKey);

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_DiversifyDirectKey(  
										phCryptoSym_Stub_DataParams_t * pDataParams,
										uint16_t wOption,
										uint8_t * pKey,    
										uint16_t wKeyType, 
										uint8_t * pDivInput, 
										uint8_t bLenDivInput,
										uint8_t * pDiversifiedKey
										)
{
	/* satisfy compiler */
	if (pDataParams || wOption || pKey || wKeyType || pDivInput || bLenDivInput || pDiversifiedKey);

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_SetConfig(
                                    phCryptoSym_Stub_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    )
{
	/* satisfy compiler */
	if (pDataParams || wConfig || wValue);

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}

phStatus_t phCryptoSym_Stub_GetConfig(
                                    phCryptoSym_Stub_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    )
{
	/* satisfy compiler */
	if (pDataParams || wConfig || pValue);

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CRYPTOSYM);
}


#endif /* NXPBUILD__PH_CRYPTOSYM_STUB */
