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

/*
* \file
* Software MIFARE DESFire Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:18 2011 $
*
* History:
*/

#ifndef PHALMFDF_SAMAV2_H
#define PHALMFDF_SAMAV2_H

phStatus_t phalMfdf_SamAV2_Authenticate(
                                        phalMfdf_SamAV2_DataParams_t *pDataParams,
                                        uint16_t wOption,
                                        uint16_t wKeyNo,
                                        uint16_t wKeyVer,
                                        uint8_t bKeyNoCard,
                                        uint8_t * pDivInput,
                                        uint8_t bDivLen
                                        );

phStatus_t phalMfdf_SamAV2_AuthenticateISO(
    phalMfdf_SamAV2_DataParams_t *pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    );

phStatus_t phalMfdf_SamAV2_AuthenticateAES(
    phalMfdf_SamAV2_DataParams_t *pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    );

phStatus_t phalMfdf_SamAV2_ChangeKeySettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeySettings
    );

phStatus_t phalMfdf_SamAV2_GetKeySettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pKeySettings
    );

phStatus_t phalMfdf_SamAV2_ChangeKey(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint16_t wOldKeyNo,
                                     uint16_t wOldKeyVer,
                                     uint16_t wNewKeyNo,
                                     uint16_t wNewKeyVer,
                                     uint8_t bKeyNoCard,
                                     uint8_t * pDivInput,
                                     uint8_t bDivLen
                                     );

phStatus_t phalMfdf_SamAV2_GetKeyVersion(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t * pKeyVersion
    );

phStatus_t phalMfdf_SamAV2_CreateApplication(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pAid, 
    uint8_t bKeySettings1,
    uint8_t bKeySettings2,
    uint8_t * pISOFileId,
    uint8_t * pISODFName,
    uint8_t bISODFNameLen
    );

phStatus_t phalMfdf_SamAV2_DeleteApplication(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pAid
    );

phStatus_t phalMfdf_SamAV2_GetApplicationIDs(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pAidBuff,
    uint8_t * pNumAid
    );

phStatus_t phalMfdf_SamAV2_GetDFNames(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bOption,
                                      uint8_t * pDFBuffer,
                                      uint8_t * bDFLen
                                      );

phStatus_t phalMfdf_SamAV2_SelectApplication(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pAid
    );

phStatus_t phalMfdf_SamAV2_FormatPICC(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams
                                      );

phStatus_t phalMfdf_SamAV2_GetVersion(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t * pVerInfo
                                      );

phStatus_t phalMfdf_SamAV2_FreeMem(
                                   phalMfdf_SamAV2_DataParams_t * pDataParams,
                                   uint8_t * pMemInfo
                                   );

phStatus_t phalMfdf_SamAV2_SetConfiguration(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pData,
    uint8_t bDataLen
    );

phStatus_t phalMfdf_SamAV2_GetCardUID(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t * pUid
                                      );

phStatus_t phalMfdf_SamAV2_GetFileIDs(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t * pFid,
                                      uint8_t * bNumFid
                                      );

phStatus_t phalMfdf_SamAV2_GetISOFileIDs(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pFidBuffer,
    uint8_t * bNumFid
    );

phStatus_t phalMfdf_SamAV2_GetFileSettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bFileNo,
    uint8_t * pFSBuffer,
    uint8_t * bBufferLen
    );

phStatus_t phalMfdf_SamAV2_ChangeFileSettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t * pAccessRights
    );

phStatus_t phalMfdf_SamAV2_CreateStdDataFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    );

phStatus_t phalMfdf_SamAV2_CreateBackupDataFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    );

phStatus_t phalMfdf_SamAV2_CreateValueFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pLowerLmit,
    uint8_t * pUpperLmit,
    uint8_t * pValue,
    uint8_t bLimitedCredit
    );

phStatus_t phalMfdf_SamAV2_CreateLinearRecordFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    );

phStatus_t phalMfdf_SamAV2_CreateCyclicRecordFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    );

phStatus_t phalMfdf_SamAV2_DeleteFile(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bFileNo
                                      );

phStatus_t phalMfdf_SamAV2_ReadData(
                                    phalMfdf_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t bFileNo,
                                    uint8_t * pOffset,
                                    uint8_t * pLength,
                                    uint8_t ** ppRxdata,
                                    uint16_t * pRxdataLen
                                    );	

phStatus_t phalMfdf_SamAV2_WriteData(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint8_t bCommOption,
                                     uint8_t bFileNo,
                                     uint8_t * pOffset,
                                     uint8_t * pTxData,
                                     uint8_t * pTxDataLen
                                     );

phStatus_t phalMfdf_SamAV2_GetValue(
                                    phalMfdf_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bCommOption,
                                    uint8_t bFileNo,
                                    uint8_t * pValue
                                    );

phStatus_t phalMfdf_SamAV2_Credit(
                                  phalMfdf_SamAV2_DataParams_t * pDataParams,
                                  uint8_t bCommOption,
                                  uint8_t bFileNo,
                                  uint8_t * pValue
                                  );

phStatus_t phalMfdf_SamAV2_Debit(
                                 phalMfdf_SamAV2_DataParams_t * pDataParams,
                                 uint8_t bCommOption,
                                 uint8_t bFileNo,
                                 uint8_t * pValue
                                 );

phStatus_t phalMfdf_SamAV2_LimitedCredit(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bCommOption,
    uint8_t bFileNo,
    uint8_t * pValue
    );

phStatus_t phalMfdf_SamAV2_WriteRecord(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bCommOption,
                                       uint8_t bFileNo,
                                       uint8_t * pOffset,
                                       uint8_t * pData,
                                       uint8_t * pDataLen
                                       );

phStatus_t phalMfdf_SamAV2_ReadRecords(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bCommOption,
                                       uint8_t bFileNo,
                                       uint8_t * pOffset,
                                       uint8_t * pNumRec,
                                       uint8_t * pRecSize,
                                       uint8_t ** ppRxdata,
                                       uint16_t * pRxdataLen
                                       );

phStatus_t phalMfdf_SamAV2_ClearRecordFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bFileNo
    );

phStatus_t phalMfdf_SamAV2_CommitTransaction(
    phalMfdf_SamAV2_DataParams_t * pDataParams
    );

phStatus_t phalMfdf_SamAV2_AbortTransaction(
    phalMfdf_SamAV2_DataParams_t * pDataParams
    );

phStatus_t phalMfdf_SamAV2_IsoSelectFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bSelector,
    uint8_t * pFid,
    uint8_t * pDFname,
    uint8_t bDFnameLen,
    uint8_t ** ppFCI,
    uint16_t * pwFCILen
    );

phStatus_t phalMfdf_SamAV2_IsoReadBinary(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bOffset,
    uint8_t bSfid,
    uint8_t bBytesToRead,
    uint8_t ** ppRxBuffer,
    uint16_t * pBytesRead
    );

phStatus_t phalMfdf_SamAV2_IsoUpdateBinary(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOffset,
    uint8_t bSfid,
    uint8_t * pData,
    uint8_t bDataLen
    );

phStatus_t phalMfdf_SamAV2_IsoReadRecords(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bRecNo,
    uint8_t bReadAllFromP1,
    uint8_t bSfid,
    uint8_t bBytesToRead,
    uint8_t ** pRxBuffer,
    uint16_t * wBytesRead
    );

phStatus_t phalMfdf_SamAV2_IsoAppendRecord(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bSfid,
    uint8_t * pData,
    uint8_t bDataLen
    );

phStatus_t phalMfdf_SamAV2_IsoGetChallenge(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bLe,
    uint8_t * pRPICC1
    );

phStatus_t phalMfdf_SamAV2_IsoExternalAuthenticate(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pInput,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    );

phStatus_t phalMfdf_SamAV2_IsoInternalAuthenticate(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pInput,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    );

phStatus_t phalMfdf_SamAV2_IsoAuthenticate(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t bIsPICCKey
    );

phStatus_t phalMfdf_SamAV2_GetConfig(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wConfig,
                                     uint16_t * pValue
                                     );

phStatus_t phalMfdf_SamAV2_SetConfig(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wConfig,
                                     uint16_t wValue
                                     );
#endif /* #define PHALMFDF_SAMAV2_H */
