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

#ifndef PHALMFDF_SW_INT_H
#define PHALMFDF_SW_INT_H

phStatus_t phalMfdf_Sw_Int_SendDataToPICC(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pCmd,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen,
    uint8_t * pResp,
    uint16_t * pRespLen
    );

phStatus_t phalMfdf_Sw_Int_GetData(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint8_t * pSendBuff,
                                   uint16_t wCmdLen,
                                   uint8_t ** pResponse,
                                   uint16_t * pRxlen
                                   );

phStatus_t phalMfdf_Sw_Int_ReadData_Plain(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    );

phStatus_t phalMfdf_Sw_Int_ReadData_Enc(
                                        phalMfdf_Sw_DataParams_t * pDataParams,
                                        uint8_t bPaddingOption,  /*  padding method 1 or 2 */
                                        uint8_t * bCmdBuff,
                                        uint16_t wCmdLen,
                                        uint8_t ** ppRxdata,
                                        uint16_t * pRxdataLen
                                        );

uint16_t phalMfdf_Sw_Int_DF4Decrypt(
                                    phalMfdf_Sw_DataParams_t * pDataParams,  /*  Pointer to this layers param structure */
                                    uint8_t * pPlainData,  /*  Plain data input */
                                    uint16_t wDataLen,  /*  Data length. Should be multiple of block size */
                                    uint8_t * pEncData  /*  Decrypted data out */
                                    );

phStatus_t phalMfdf_Sw_Int_Write_Plain(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t * bCmdBuff,
                                       uint16_t wCmdLen,
                                       uint8_t bCommOption,
                                       uint8_t * pData,
                                       uint16_t  wDataLen
                                       );
phStatus_t phalMfdf_Sw_Int_Write_Enc(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t * bCmdBuff,
                                     uint16_t wCmdLen,
                                     uint8_t bPaddingMode,  /*  Either PH_CRYPTOSYM_PADDING_MODE_1 or PH_CRYPTOSYM_PADDING_MODE_2 */
                                     uint8_t * pData,
                                     uint16_t wDataLen
                                     );

void phalMfdf_Sw_Int_ResetAuthStatus(phalMfdf_Sw_DataParams_t * pDataParams);

phStatus_t phalMfdf_Sw_Int_IsoRead(
                               phalMfdf_Sw_DataParams_t * pDataParams,
                               uint16_t wOption,
                               uint8_t * bCmdBuff,
                               uint16_t wCmdLen,
                               uint8_t ** ppRxBuffer,
                               uint16_t * pBytesRead
                               );

#endif /* PHALMFDF_SW_INT_H */
