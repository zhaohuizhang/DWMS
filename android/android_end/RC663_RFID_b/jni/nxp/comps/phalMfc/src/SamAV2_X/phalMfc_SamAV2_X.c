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
* SamAV2 (X-Mode) MIFARE(R) Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:27:57 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phalMfc.h>
#include <phpalMifare.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFC_SAMAV2_X

#include "phalMfc_SamAV2_X.h"
#include "phalMfc_SamAV2_X_Int.h"
#include "../phalMfc_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phalMfc_SamAV2_X_Init(
                                 phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                 uint16_t wSizeOfDataParams, 
                                 phhalHw_SamAV2_DataParams_t * pHalDataParams,
                                 phpalMifare_SamAV2_X_DataParams_t * pPalMifareDataParams
                                 )
{
    if (sizeof(phalMfc_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFC);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pHalDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFC | PHAL_MFC_SAMAV2_X_ID;
    pDataParams->pHalDataParams         = pHalDataParams;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_SamAV2_X_Authenticate(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    )
{       
    return phpalMifare_MfcAuthenticateKeyNo(
        pDataParams->pPalMifareDataParams,
        bBlockNo,
        bKeyType,
        wKeyNo,
        wKeyVersion,
        &pUid[bUidLength - 4]);
}

phStatus_t phalMfc_SamAV2_X_Read(
                                 phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                 uint8_t bBlockNo,
                                 uint8_t * pBlockData
                                 )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxBufferLength;

    /* perform read operation */
    status = phhalHw_SamAV2_Cmd_MfRead(
        pDataParams->pHalDataParams,
        PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_SINGLEBLOCK,
        &bBlockNo, 
        1,
        &pRxBuffer,
        &wRxBufferLength
        );

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_SamAV2_X_ConvertNak(status));

    memcpy(pBlockData, pRxBuffer, wRxBufferLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_SamAV2_X_ReadValue(
                                      phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bBlockNo,
                                      uint8_t * pValue,
                                      uint8_t * pAddrData
                                      )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aBuffer[PHAL_MFC_DATA_BLOCK_LENGTH];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxBufferLength;

    /* perform read operation */
    status = phhalHw_SamAV2_Cmd_MfRead(
        pDataParams->pHalDataParams,
        PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_SINGLEBLOCK,
        &bBlockNo, 
        1,
        &pRxBuffer, 
        &wRxBufferLength);

    /* Convert HAL status codes to palMifare status codes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_SamAV2_X_ConvertNak(status));

    memcpy(aBuffer, pRxBuffer, wRxBufferLength); /* PRQA S 3200 */

    /* check format of value block */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Int_CheckValueBlockFormat(aBuffer));

    /* return received contents */
    *pAddrData = aBuffer[12];
    memcpy(pValue, aBuffer, PHAL_MFC_VALUE_BLOCK_LENGTH);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_SamAV2_X_Write(
                                  phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                  uint8_t bBlockNo,
                                  uint8_t * pBlockData
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT,
        &bBlockNo,
        1));

    /* perform the actual write operation */
    status = phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_LAST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT,
        pBlockData,
        PHAL_MFC_DATA_BLOCK_LENGTH);

    /* Convert HAL status codes to palMifare status codes */
    return phalMfc_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfc_SamAV2_X_WriteValue(
                                       phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                       uint8_t bBlockNo,
                                       uint8_t * pValue,
                                       uint8_t bAddrData
                                       )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bBlockData[PHAL_MFC_DATA_BLOCK_LENGTH];

    /* build a valid value block */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Int_CreateValueBlock(pValue, bAddrData, bBlockData ));

    PH_CHECK_SUCCESS_FCT(status, phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT,
        &bBlockNo,
        1));

    /* perform the actual write operation */
    status = phhalHw_SamAV2_Cmd_MfWrite(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_LAST | PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT,
        bBlockData,
        PHAL_MFC_DATA_BLOCK_LENGTH);

    /* Convert HAL status codes to palMifare status codes */
    return phalMfc_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfc_SamAV2_X_Increment(
                                      phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bBlockNo,
                                      uint8_t * pValue
                                      )
{
    return phalMfc_Int_Value(pDataParams->pPalMifareDataParams, PHAL_MFC_CMD_INCREMENT, bBlockNo, pValue);
}

phStatus_t phalMfc_SamAV2_X_Decrement(
                                      phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bBlockNo,
                                      uint8_t * pValue
                                      )
{
    return phalMfc_Int_Value(pDataParams->pPalMifareDataParams, PHAL_MFC_CMD_DECREMENT, bBlockNo, pValue);
}

phStatus_t phalMfc_SamAV2_X_Transfer(
                                     phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                     uint8_t bBlockNo
                                     )
{
    /* transmit the command frame */
    return phalMfc_Int_Transfer(
        pDataParams->pPalMifareDataParams,
        bBlockNo
        );
}

phStatus_t phalMfc_SamAV2_X_Restore(
                                    phalMfc_SamAV2_X_DataParams_t * pDataParams,
                                    uint8_t bBlockNo
                                    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aBuffer[PHAL_MFC_VALUE_BLOCK_LENGTH];

    /* restore needs four dummy bytes */
    memset(aBuffer, 0x00, PHAL_MFC_VALUE_BLOCK_LENGTH);  /* PRQA S 3200 */

    status = phalMfc_Int_Value(pDataParams->pPalMifareDataParams, PHAL_MFC_CMD_RESTORE, bBlockNo, aBuffer);

    /* Convert HAL status codes to palMifare status codes */
    return phalMfc_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfc_SamAV2_X_IncrementTransfer(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSrcBlockNo,
    uint8_t bDstBlockNo,
    uint8_t * pValue
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aBuffer[2 + PHAL_MFC_VALUE_BLOCK_LENGTH];

    /* the first two bytes are the block numbers */
    aBuffer[0] = bSrcBlockNo;
    aBuffer[1] = bDstBlockNo;

    /* the following bytes are the data */
    memcpy(&aBuffer[2], pValue, PHAL_MFC_VALUE_BLOCK_LENGTH);  /* PRQA S 3200 */

    /* perform actual operation */
    status = phhalHw_SamAV2_Cmd_MfIncrement(pDataParams->pHalDataParams, aBuffer, (2 + PHAL_MFC_VALUE_BLOCK_LENGTH));

    /* Convert HAL status codes to palMifare status codes */
    return phalMfc_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfc_SamAV2_X_DecrementTransfer(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSrcBlockNo,
    uint8_t bDstBlockNo,
    uint8_t * pValue
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aBuffer[2 + PHAL_MFC_VALUE_BLOCK_LENGTH];

    /* the first two bytes are the block numbers */
    aBuffer[0] = bSrcBlockNo;
    aBuffer[1] = bDstBlockNo;

    /* the following bytes are the data */
    memcpy(&aBuffer[2], pValue, PHAL_MFC_VALUE_BLOCK_LENGTH);  /* PRQA S 3200 */

    /* perform actual operation */
    status = phhalHw_SamAV2_Cmd_MfDecrement(pDataParams->pHalDataParams, aBuffer, (2 + PHAL_MFC_VALUE_BLOCK_LENGTH));

    /* Convert HAL status codes to palMifare status codes */
    return phalMfc_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfc_SamAV2_X_RestoreTransfer(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSrcBlockNo,
    uint8_t bDstBlockNo                                  
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aBuffer[2];

    /* the first two bytes are the block numbers */
    aBuffer[0] = bSrcBlockNo;
    aBuffer[1] = bDstBlockNo;

    /* perform actual operation */
    status = phhalHw_SamAV2_Cmd_MfRestore(pDataParams->pHalDataParams, aBuffer, 2);

    /* Convert HAL status codes to palMifare status codes */
    return phalMfc_SamAV2_X_ConvertNak(status);
}

phStatus_t phalMfc_SamAV2_X_ConvertNak(
                                       phStatus_t status
                                       )
{
    switch (status & PH_ERR_MASK)
    {
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK0:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK0, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK1:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK1, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK4:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK4, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK5:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK5, PH_COMP_PAL_MIFARE);
    case PH_ERR_SUCCESS_INCOMPLETE_BYTE:
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_MIFARE);
    default:
        return status;
    }
}

phStatus_t phalMfc_SamAV2_X_PersonalizeUid(
    phalMfc_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bUidType
    )
{
    uint8_t     PH_MEMLOC_REM aCommand[2];
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint16_t    PH_MEMLOC_REM wRxlen;

    /* build command frame */
    aCommand[0] = PHAL_MFC_CMD_PERSOUID;
    aCommand[1] = bUidType;

    return phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        &pRecv,
        &wRxlen
        );
}

#endif /* NXPBUILD__PHAL_MFC_SAMAV2_X */
