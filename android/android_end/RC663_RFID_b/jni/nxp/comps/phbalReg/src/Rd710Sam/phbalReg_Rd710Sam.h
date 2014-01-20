/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* RD710Sam BAL Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:52 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHBALREG_RD710SAM_H
#define PHBALREG_RD710SAM_H

#include <ph_Status.h>

phStatus_t phbalReg_Rd710Sam_GetPortList(
    phbalReg_Rd710Sam_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    );

phStatus_t phbalReg_Rd710Sam_SetPort(
                                     phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                     uint8_t * pPortName                        
                                     );

phStatus_t phbalReg_Rd710Sam_Exchange(
                                      phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength,
                                      uint16_t wRxBufSize,
                                      uint8_t * pRxBuffer,
                                      uint16_t * pRxLength
                                      );

phStatus_t phbalReg_Rd710Sam_OpenPort(
                                      phbalReg_Rd710Sam_DataParams_t * pDataParams
                                      );

phStatus_t phbalReg_Rd710Sam_ClosePort(
                                       phbalReg_Rd710Sam_DataParams_t * pDataParams
                                       );

phStatus_t phbalReg_Rd710Sam_SetConfig(
                                       phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t wValue
                                       );

phStatus_t phbalReg_Rd710Sam_GetConfig(
                                       phbalReg_Rd710Sam_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t * pValue
                                       );

#endif /* PHBALREG_RD710SAM_H */
