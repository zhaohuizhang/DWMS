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
* SamAV2 (X-Mode) MIFARE(R) Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:57 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#ifndef PHALMFC_SAMAV2_X_H
#define PHALMFC_SAMAV2_X_H

#include <ph_Status.h>
#include <phalMfc.h>

phStatus_t phalMfc_SamAV2_X_Authenticate(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType, 
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    );

phStatus_t phalMfc_SamAV2_X_Read(
                                 phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                 uint8_t bBlockNo,
                                 uint8_t * pBlockData
                                 );

phStatus_t phalMfc_SamAV2_X_ReadValue(
                                      phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bBlockNo,
                                      uint8_t * pValue,
                                      uint8_t * pAddrData
                                      );

phStatus_t phalMfc_SamAV2_X_Write(
                                  phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                  uint8_t bBlockNo,
                                  uint8_t * pBlockData
                                  );

phStatus_t phalMfc_SamAV2_X_WriteValue(
                                       phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                       uint8_t bBlockNo,
                                       uint8_t * pValue,
                                       uint8_t bAddrData
                                       );

phStatus_t phalMfc_SamAV2_X_Increment(
                                      phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bBlockNo, 
                                      uint8_t * pValue
                                      );

phStatus_t phalMfc_SamAV2_X_Decrement(
                                      phalMfc_SamAV2_X_DataParams_t * pDataParams, 
                                      uint8_t bBlockNo,
                                      uint8_t * pValue
                                      );

phStatus_t phalMfc_SamAV2_X_Transfer(
                                     phalMfc_SamAV2_X_DataParams_t * pDataParams, 
                                     uint8_t bBlockNo
                                     );

phStatus_t phalMfc_SamAV2_X_Restore(
                                    phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                    uint8_t bBlockNo
                                    );

phStatus_t phalMfc_SamAV2_X_IncrementTransfer(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSrcBlockNo,
    uint8_t bDstBlockNo,
    uint8_t * pValue 
    );

phStatus_t phalMfc_SamAV2_X_DecrementTransfer(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSrcBlockNo,
    uint8_t bDstBlockNo,
    uint8_t * pValue
    );

phStatus_t phalMfc_SamAV2_X_RestoreTransfer(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSrcBlockNo,
    uint8_t bDstBlockNo
    );

phStatus_t phalMfc_SamAV2_X_PersonalizeUid(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bUidType
    );

#endif /* PHALMFC_SAMAV2_X_H */
