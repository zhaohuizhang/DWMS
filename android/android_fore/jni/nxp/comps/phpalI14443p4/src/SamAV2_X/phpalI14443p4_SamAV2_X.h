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
* Software ISO14443-4 SamAV2 Component of Basic Function Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:49 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHPALI14443P4_SAMAV2_X_H
#define PHPALI14443P4_SAMAV2_X_H

#include <ph_Status.h>

#define I14443P4_SAMAV2_X_FREE_CID                          0x01   /** < force CID to be freed in any case */
#define I14443P4_SAMAV2_X_LFI_NO_DATA                       0x00   /**<  Last Frame Indicator: A00h if data field holds last frame to be sent to the PICC */
#define I14443P4_SAMAV2_X_LFI_MORE_DATA                     0xAF   /**<  Last Frame Indicator: AFh if more data shall be sent to the PICC */
#define I14443P4_SAMAV2_X_DATA_RESPONSE_MAX                 0xF8   /**<  Last Frame Indicator: AFh if more data shall be sent to the PICC */

phStatus_t phpalI14443p4_SamAV2_X_SetProtocol(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint8_t   bCidEnable,
    uint8_t   bCid,
    uint8_t   bNadEnable, 
    uint8_t   bNad,
    uint8_t   bFwi,
    uint8_t   bFsdi,
    uint8_t   bFsci
    );

phStatus_t phpalI14443p4_SamAV2_X_ResetProtocol(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams
    );

phStatus_t phpalI14443p4_SamAV2_X_Deselect(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams
    );

phStatus_t phpalI14443p4_SamAV2_X_PresCheck(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams
    );

phStatus_t phpalI14443p4_SamAV2_X_Exchange(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    );

phStatus_t phpalI14443p4_SamAV2_X_SetConfig(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    );

phStatus_t phpalI14443p4_SamAV2_X_GetConfig(
    phpalI14443p4_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
    );

#endif /* PHPALI14443P4_SAMAV2_X_H */
