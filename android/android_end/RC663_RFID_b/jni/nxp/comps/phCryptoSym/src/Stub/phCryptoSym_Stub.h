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
* Software specific Symmetric Cryptography Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:05 2011 $
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#ifndef PHCRYPTOSYM_STUB_H
#define PHCRYPTOSYM_STUB_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>



phStatus_t phCryptoSym_Stub_InvalidateKey(
									phCryptoSym_Stub_DataParams_t * pDataParams                           
									);

phStatus_t phCryptoSym_Stub_Encrypt(
									phCryptoSym_Stub_DataParams_t * pDataParams,
									uint16_t wOption,
									const uint8_t * pPlainBuffer,   
									uint16_t  wBufferLength, 
									uint8_t * pEncryptedBuffer
									);

phStatus_t phCryptoSym_Stub_Decrypt(
									phCryptoSym_Stub_DataParams_t * pDataParams,
									uint16_t wOption, 
									const uint8_t * pEncryptedBuffer, 
									uint16_t  wBufferLength,
									uint8_t * pPlainBuffer 
									);

phStatus_t phCryptoSym_Stub_CalculateMac(
									phCryptoSym_Stub_DataParams_t * pDataParams,
									uint16_t wOption,
									const uint8_t * pData, 
									uint16_t  wDataLength,                            
									uint8_t * pMac,
									uint8_t * pMacLength 
									);

phStatus_t phCryptoSym_Stub_LoadIv(  
									phCryptoSym_Stub_DataParams_t * pDataParams,
									const uint8_t * pIV,
									uint8_t bIVLength
									);

phStatus_t phCryptoSym_Stub_LoadKey(  
									phCryptoSym_Stub_DataParams_t * pDataParams,
									uint16_t wKeyNo,
									uint16_t wKeyVersion,
									uint16_t wKeyType
									);

phStatus_t phCryptoSym_Stub_LoadKeyDirect(  
                                    phCryptoSym_Stub_DataParams_t * pDataParams,
                                    const uint8_t * pKey,
                                    uint16_t wKeyType
                                    );

phStatus_t phCryptoSym_Stub_DiversifyKey(  
									phCryptoSym_Stub_DataParams_t * pDataParams, 
									uint16_t wOption,
									uint16_t wKeyNo,
									uint16_t wKeyVersion,
									uint8_t * pDivInput,
									uint8_t  bLenDivInput, 
									uint8_t * pDiversifiedKey
									);

phStatus_t phCryptoSym_Stub_DiversifyDirectKey(  
									phCryptoSym_Stub_DataParams_t * pDataParams,
									uint16_t wOption,
									uint8_t * pKey,    
									uint16_t wKeyType, 
									uint8_t * pDivInput, 
									uint8_t bLenDivInput,
									uint8_t * pDiversifiedKey
									);


phStatus_t phCryptoSym_Stub_SetConfig(
                                    phCryptoSym_Stub_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    );

phStatus_t phCryptoSym_Stub_GetConfig(
                                    phCryptoSym_Stub_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    );

#endif /* PHCRYPTOSYM_STUB_H */
