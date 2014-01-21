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
* SAMAV2 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:27:59 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_SAMAV2_H
#define PHHALHW_SAMAV2_H

#include <ph_Status.h>
#include <phhalHw.h>

#define PHHAL_HW_SAMAV2_FIFOSIZE                0xFFU   /**< Size of RC internal FiFo buffer. */
#define PHHAL_HW_SAMAV2_DEFAULT_TIMEOUT         150U    /**< Default timeout in microseconds. */

#define PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING      0x00U
#define PHHAL_HW_SAMAV2_HC_AV2_CHAINING_NO_SM   0x0FU
#define PHHAL_HW_SAMAV2_HC_AV2_CHAINING         0xFFU
#define PHHAL_HW_SAMAV2_HC_AV2_NO_SM            0x00U
#define PHHAL_HW_SAMAV2_HC_AV2_MAC              0x0FU
#define PHHAL_HW_SAMAV2_HC_AV2_ENC              0xF0U

#define PHHAL_HW_SAMAV2_KEYENTRY_DESFIRE_AID_POS    48
#define PHHAL_HW_SAMAV2_KEYENTRY_DESFIRE_KEYNUM_POS 51
#define PHHAL_HW_SAMAV2_KEYENTRY_REFNUM_CEK_POS     52
#define PHHAL_HW_SAMAV2_KEYENTRY_KEYVER_CEK_POS     53
#define PHHAL_HW_SAMAV2_KEYENTRY_REFNUM_KUC_POS     54
#define PHHAL_HW_SAMAV2_KEYENTRY_CONFIG_SET_POS     55
#define PHHAL_HW_SAMAV2_KEYENTRY_KEY_A_VERSION_POS  57
#define PHHAL_HW_SAMAV2_KEYENTRY_KEY_B_VERSION_POS  58
#define PHHAL_HW_SAMAV2_KEYENTRY_KEY_C_VERSION_POS  59
#define PHHAL_HW_SAMAV2_KEYENTRY_CONFIG_SET2_POS    60

phStatus_t phhalHw_SamAV2_Exchange(
                                   phhalHw_SamAV2_DataParams_t * pDataParams,
                                   uint16_t wOption,
                                   uint8_t * pTxBuffer, 
                                   uint16_t wTxLength,
                                   uint8_t ** ppRxBuffer,
                                   uint16_t * pRxLength
                                   );

phStatus_t phhalHw_SamAV2_WriteRegister(
                                        phhalHw_SamAV2_DataParams_t * pDataParams,
                                        uint8_t bAddress,                        
                                        uint8_t bValue                
                                        );

phStatus_t phhalHw_SamAV2_ReadRegister(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bAddress,  
                                       uint8_t * pValue   
                                       );

phStatus_t phhalHw_SamAV2_ApplyProtocolSettings(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint8_t bCardType
    );

phStatus_t phhalHw_SamAV2_MfcAuthenticateKeyNo(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    );

phStatus_t phhalHw_SamAV2_MfcAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
    );

phStatus_t phhalHw_SamAV2_SetConfig(
                                    phhalHw_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    );

phStatus_t phhalHw_SamAV2_GetConfig(
                                    phhalHw_SamAV2_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    );

phStatus_t phhalHw_SamAV2_FieldOn(
                                  phhalHw_SamAV2_DataParams_t * pDataParams 
                                  );

phStatus_t phhalHw_SamAV2_FieldOff(
                                   phhalHw_SamAV2_DataParams_t * pDataParams 
                                   );

phStatus_t phhalHw_SamAV2_FieldReset(
                                     phhalHw_SamAV2_DataParams_t * pDataParams
                                     );

phStatus_t phhalHw_SamAV2_Wait(
                               phhalHw_SamAV2_DataParams_t * pDataParams,
                               uint8_t bUnit, 
                               uint16_t wTimeout  
                               );

#endif /* PHHALHW_SAMAV2_H */
