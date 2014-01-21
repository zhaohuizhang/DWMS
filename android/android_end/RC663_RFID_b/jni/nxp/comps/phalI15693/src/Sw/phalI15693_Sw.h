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
* $Date: Fri Apr 15 09:27:49 2011 $
*
* History:
*  CHu: Generated 10. November 2009
*
*/

#ifndef PHALI15693_SW_H
#define PHALI15693_SW_H

#include <ph_Status.h>

phStatus_t phalI15693_Sw_ReadSingleBlock (
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    );

phStatus_t phalI15693_Sw_ReadMultipleBlocks (
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    );

phStatus_t phalI15693_Sw_WriteSingleBlock (
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint8_t * pTxBuffer,
    uint16_t wTxLength
    );

phStatus_t phalI15693_Sw_WriteMultipleBlocks (
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t * pTxBuffer,
    uint16_t wTxLength
    );

phStatus_t phalI15693_Sw_LockBlock (
                                    phalI15693_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t bBlockNo
                                    );

phStatus_t phalI15693_Sw_WriteAFI (
                                   phalI15693_Sw_DataParams_t * pDataParams,
                                   uint8_t bOption,
                                   uint8_t bAfi
                                   );

phStatus_t phalI15693_Sw_LockAFI (
                                  phalI15693_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption
                                  );

phStatus_t phalI15693_Sw_WriteDSFID (
                                     phalI15693_Sw_DataParams_t * pDataParams,
                                     uint8_t bOption,
                                     uint8_t bDsfid
                                     );

phStatus_t phalI15693_Sw_LockDSFID (
                                    phalI15693_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption
                                    );

phStatus_t phalI15693_Sw_GetSystemInformation (
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    );

phStatus_t phalI15693_Sw_GetMultipleBlockSecurityStatus (
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    );

#endif /* PHALI15693_SW_H */
