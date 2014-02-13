/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

#ifndef PHALMFDF_SAMAV2_INT_H
#define PHALMFDF_SAMAV2_INT_H

#include <phalMfdf.h>
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalMfdf_SamAV2_Int_Authenticate_General(
    phalMfdf_SamAV2_DataParams_t *pDataParams,  /* Pointer to this layer's parameter structure. */
    uint16_t wDivOption,                        /* whether NEW, OLD or NO diversification is used */
    uint16_t wKeyNo,                            /* 1 byte key number in key store to authenticate with */
    uint16_t wKeyVer,                           /* Key version in the key store */
    uint8_t bKeyNoCard,                         /* Key number on card */
    uint8_t * pDivInput,                        /* Diversification input. Can be NULL */
    uint8_t bDivLen                             /* Length of diversification input max 31B */
    );

phStatus_t phalMfdf_SamAV2_Int_Write_Plain(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t bCommOption,
    uint8_t * pData,
    uint16_t wDataLen
    );

phStatus_t phalMfdf_SamAV2_Int_Write_Enc(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen
    );
/*
* Reset authentication status.
*/
void phalMfdf_SamAV2_Int_ResetAuthStatus(phalMfdf_SamAV2_DataParams_t * pDataParams);

/*
* Utility function
*/
phStatus_t phalMfdf_SamAV2_Int_SendDataToPICC(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pCmd,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen,
    uint8_t * pResp,
    uint16_t * pRespLen );

phStatus_t phalMfdf_SamAV2_Int_GetData(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint8_t * pSendBuff,
                                       uint16_t wCmdLen,
                                       uint8_t ** pResponse,
                                       uint16_t * pRxlen
                                       );

phStatus_t phalMfdf_SamAV2_Int_ReadData_Plain(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    );

phStatus_t phalMfdf_SamAV2_Int_ReadData_Enc(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t * pLength, /* Length of data to be read. 3 Byte array, LSB First */
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    );

phStatus_t phalMfdf_SamAV2_Int_IsoRead(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t ** ppRxBuffer,
    uint16_t * pBytesRead
    );

#endif /* PHALMFDF_SAMAV2_INT_H */
