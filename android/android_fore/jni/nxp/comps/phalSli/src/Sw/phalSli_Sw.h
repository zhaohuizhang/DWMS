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
* Software ICode SLI Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:40 2011 $
*
* History:
*  CHu: Generated 10. November 2009
*
*/

#ifndef PHALSLI_SW_H
#define PHALSLI_SW_H

#include <ph_Status.h>

phStatus_t phalSli_Sw_GetRandomNumber(
                                      phalSli_Sw_DataParams_t * pDataParams,
                                      uint8_t * pRnd
                                      );

phStatus_t phalSli_Sw_SetPWD(
                             phalSli_Sw_DataParams_t * pDataParams,
                             uint8_t bPwdIdentifier,
                             uint8_t * pXorPwd
                             );

phStatus_t phalSli_Sw_WritePWD(
                               phalSli_Sw_DataParams_t * pDataParams,
                               uint8_t bPwdIdentifier,
                               uint8_t * pPwd
                               );

phStatus_t phalSli_Sw_LockPWD(
                              phalSli_Sw_DataParams_t * pDataParams,
                              uint8_t bPwdIdentifier
                              );

phStatus_t phalSli_Sw_64BitPWDProtection(
    phalSli_Sw_DataParams_t * pDataParams
    );

phStatus_t phalSli_Sw_ProtectPage(
                                  phalSli_Sw_DataParams_t * pDataParams,
                                  uint8_t bPageNo,
                                  uint8_t bProtectionStatus
                                  );

phStatus_t phalSli_Sw_LockPageProtectionCondition(
    phalSli_Sw_DataParams_t * pDataParams,
    uint8_t bPageNo
    );

phStatus_t phalSli_Sw_GetMultipleBlockProtStatus(
    phalSli_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t * pProtectionStates,
    uint8_t * pNumReceivedStates
    );

phStatus_t phalSli_Sw_Destroy(
                              phalSli_Sw_DataParams_t * pDataParams
                              );

phStatus_t phalSli_Sw_EnablePrivacy(
                                    phalSli_Sw_DataParams_t * pDataParams
                                    );

phStatus_t phalSli_Sw_SetEAS(
                             phalSli_Sw_DataParams_t * pDataParams
                             );

phStatus_t phalSli_Sw_ResetEAS(
                               phalSli_Sw_DataParams_t * pDataParams
                               );

phStatus_t phalSli_Sw_LockEAS(
                              phalSli_Sw_DataParams_t * pDataParams
                              );

phStatus_t phalSli_Sw_EASAlarm(
                               phalSli_Sw_DataParams_t * pDataParams,
                               uint8_t bOption,
                               uint8_t * pEasIdValue,
                               uint8_t bEasIdMaskLength,
                               uint8_t * pEas
                               );

phStatus_t phalSli_Sw_PwdProtectEAS(
                                    phalSli_Sw_DataParams_t * pDataParams
                                    );

phStatus_t phalSli_Sw_WriteEAS_ID(
                                  phalSli_Sw_DataParams_t * pDataParams,
                                  uint8_t * pEasIdValue
                                  );

phStatus_t phalSli_Sw_ReadEPC(
                              phalSli_Sw_DataParams_t * pDataParams,
                              uint8_t * pEpc
                              );

#endif /* PHALSLI_SW_H */
