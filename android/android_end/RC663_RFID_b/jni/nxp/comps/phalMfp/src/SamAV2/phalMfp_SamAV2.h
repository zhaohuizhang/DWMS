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
* Software MIFARE Plus(R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:50 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#ifndef PHALMFP_SAMAV2_H
#define PHALMFP_SAMAV2_H

#include <ph_Status.h>

phStatus_t phalMfp_SamAV2_WritePerso(
                                     phalMfp_SamAV2_DataParams_t * pDataParams,
                                     uint8_t bLayer4Comm,
                                     uint16_t wBlockNr,
                                     uint8_t * pValue
                                     );

phStatus_t phalMfp_SamAV2_CommitPerso(
                                      phalMfp_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bLayer4Comm
                                      );

phStatus_t phalMfp_SamAV2_AuthenticateSL0(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bLayer4Comm,
    uint8_t bFirstAuth,
    uint16_t wBlockNr,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t bLenDivInput,
    uint8_t * pDivInput,
    uint8_t bLenPcdCap2,
    uint8_t * pPcdCap2In,
    uint8_t * pPcdCap2Out,
    uint8_t * pPdCap2
    );

phStatus_t phalMfp_SamAV2_AuthenticateSL1(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bLayer4Comm,
    uint8_t bFirstAuth,
    uint16_t wBlockNr,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t bLenDivInput,
    uint8_t * pDivInput,
    uint8_t bLenPcdCap2,
    uint8_t * pPcdCap2In,
    uint8_t * pPcdCap2Out,
    uint8_t * pPdCap2
    );

phStatus_t phalMfp_SamAV2_AuthenticateSL2(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bLayer4Comm,
    uint8_t bFirstAuth,
    uint16_t wBlockNr,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t bLenDivInput,
    uint8_t * pDivInput,
    uint8_t bLenPcdCap2,
    uint8_t * pPcdCap2In,
    uint8_t * pPcdCap2Out,
    uint8_t * pPdCap2,
    uint8_t * pKmf
    );

phStatus_t phalMfp_SamAV2_AuthenticateClassicSL2(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    );

phStatus_t phalMfp_SamAV2_MultiBlockRead(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNr,
    uint8_t bNumBlocks,
    uint8_t * pBlocks
    );

phStatus_t phalMfp_SamAV2_MultiBlockWrite(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNr,
    uint8_t bNumBlocks,
    uint8_t * pBlocks
    );

phStatus_t phalMfp_SamAV2_Write(
                                phalMfp_SamAV2_DataParams_t * pDataParams,
                                uint8_t bEncrypted,
                                uint8_t bWriteMaced,
                                uint16_t wBlockNr,
                                uint8_t bNumBlocks,
                                uint8_t * pBlocks
                                );

phStatus_t phalMfp_SamAV2_WriteValue(
                         phalMfp_SamAV2_DataParams_t * pDataParams,
                         uint8_t bEncrypted,
                         uint8_t bWriteMaced,
                         uint16_t wBlockNr,
                         uint8_t * pValue,
                         uint8_t bAddrData
                         );

phStatus_t phalMfp_SamAV2_ChangeKey(
                             phalMfp_SamAV2_DataParams_t * pDataParams,  
                             uint8_t bChangeKeyMaced, 
                             uint16_t wBlockNr,   
                             uint16_t wKeyNumber, 
                             uint16_t wKeyVersion,
                             uint8_t bLenDivInput,
                             uint8_t * pDivInput  
                             );

phStatus_t phalMfp_SamAV2_AuthenticateSL3(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bFirstAuth,
    uint16_t wBlockNr,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t bLenDivInput,
    uint8_t * pDivInput,
    uint8_t bLenPcdCap2,
    uint8_t * pPcdCap2In,
    uint8_t * pPcdCap2Out,
    uint8_t * pPdCap2
    );

phStatus_t phalMfp_SamAV2_Read(
                               phalMfp_SamAV2_DataParams_t * pDataParams,
                               uint8_t bEncrypted,
                               uint8_t bReadMaced,
                               uint8_t bMacOnCmd,
                               uint16_t wBlockNr,
                               uint8_t bNumBlocks,
                               uint8_t * pBlocks
                               );

phStatus_t phalMfp_SamAV2_ReadValue(
                        phalMfp_SamAV2_DataParams_t * pDataParams,
                        uint8_t bEncrypted,
                        uint8_t bReadMaced,
                        uint8_t bMacOnCmd,
                        uint16_t wBlockNr,
                        uint8_t * pValue,
                        uint8_t * pAddrData
                        );

phStatus_t phalMfp_SamAV2_ResetAuth(
                                    phalMfp_SamAV2_DataParams_t * pDataParams
                                    );

phStatus_t phalMfp_SamAV2_Increment(
                                    phalMfp_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bIncrementMaced,
                                    uint16_t wBlockNr,
                                    uint8_t * pValue
                                    );

phStatus_t phalMfp_SamAV2_Decrement(
                                    phalMfp_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bDecrementMaced,
                                    uint16_t wBlockNr,
                                    uint8_t * pValue
                                    );

phStatus_t phalMfp_SamAV2_IncrementTransfer(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bIncrementTransferMaced,
    uint16_t wSourceBlockNr,
    uint16_t wDestinationBlockNr,
    uint8_t * pValue
    );

phStatus_t phalMfp_SamAV2_DecrementTransfer(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bDecrementTransferMaced,
    uint16_t wSourceBlockNr,
    uint16_t wDestinationBlockNr,
    uint8_t * pValue
    );

phStatus_t phalMfp_SamAV2_Transfer(
                                   phalMfp_SamAV2_DataParams_t * pDataParams,
                                   uint8_t bTransferMaced,
                                   uint16_t wBlockNr
                                   );

phStatus_t phalMfp_SamAV2_Restore(
                                  phalMfp_SamAV2_DataParams_t * pDataParams,
                                  uint8_t bRestoreMaced,
                                  uint16_t wBlockNr
                                  );

phStatus_t phalMfp_SamAV2_ProximityCheck(
    phalMfp_SamAV2_DataParams_t * pDataParams,
    uint8_t bGenerateRndC,
    uint8_t * pRndC,
    uint8_t bPps1,
    uint8_t bNumSteps,
    uint8_t * pUsedRndC
    );

phStatus_t phalMfp_SamAV2_ResetSecMsgState(
    phalMfp_SamAV2_DataParams_t * pDataParams
    );

#endif /* PHALMFP_SAMAV2_H */
