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
* SamAV2 Virtual Card Architecture(R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:54 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#ifndef PHALVCA_SAMAV2_X_H
#define PHALVCA_SAMAV2_X_H

#include <ph_Status.h>

phStatus_t phalVca_SamAV2_X_VcSupport(
                                      phalVca_SamAV2_X_DataParams_t * pDataParams, 
                                      uint8_t * pIid,
                                      uint16_t wKeyEncNumber,                           
                                      uint16_t wKeyEncVersion,
                                      uint16_t wKeyMacNumber,                     
                                      uint16_t wKeyMacVersion
                                      );

phStatus_t phalVca_SamAV2_X_SelectVc(
                                     phalVca_SamAV2_X_DataParams_t * pDataParams, 
                                     uint16_t wValidIidIndex,
                                     uint16_t wKeyNumber,                      
                                     uint16_t wKeyVersion
                                     );

phStatus_t phalVca_SamAV2_X_VcSupportLast(
    phalVca_SamAV2_X_DataParams_t * pDataParams, 
    uint8_t * pIid,
    uint8_t bLenCap,
    uint8_t* pPcdCapabilities, 
    uint16_t wKeyEncNumber,               
    uint16_t wKeyEncVersion, 
    uint16_t wKeyMacNumber,                           
    uint16_t wKeyMacVersion  
    );

phStatus_t phalVca_SamAV2_X_StartCardSelection (
    phalVca_SamAV2_X_DataParams_t * pDataParams
    );

phStatus_t phalVca_SamAV2_X_FinalizeCardSelection(
    phalVca_SamAV2_X_DataParams_t * pDataParams, 
    uint16_t* pNumValidIids
    );

phStatus_t phalVca_SamAV2_X_GetIidInfo(
                                       phalVca_SamAV2_X_DataParams_t * pDataParams,
                                       uint16_t wValidIidIndex,
                                       uint16_t * pIidIndex,
                                       uint8_t * pVcUidSize,
                                       uint8_t * pVcUid,
                                       uint8_t * pInfo,
                                       uint8_t * pPdCapabilities
                                       );

phStatus_t phalVca_SamAV2_X_DeselectVc(
                                       phalVca_SamAV2_X_DataParams_t * pDataParams
                                       );

phStatus_t phalVca_SamAV2_X_ProximityCheck(
    phalVca_SamAV2_X_DataParams_t * pDataParams, 
    uint8_t     bGenerateRndC,
    uint8_t*    pRndC, 
    uint8_t     bPps1,
    uint8_t     bNumSteps, 
    uint16_t    wKeyNumber,        
    uint16_t    wKeyVersion,  
    uint8_t*    pUsedRndC
    );


phStatus_t phalVca_SamAV2_X_ResetSecMsgState(
    phalVca_SamAV2_X_DataParams_t * pDataParams
    );

#endif /* PHALVCA_SAMAV2_X_H */
